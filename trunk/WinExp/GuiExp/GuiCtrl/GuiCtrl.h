// Copyright 2011, 木头云
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////////////
// GuiCtrl - 控件对象
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-08-11
// Version:	1.0.0013.1730
//
// History:
//	- 1.0.0001.2236(2011-05-23)	+ IGuiCtrl添加效果对象相关接口
//								+ 添加IGuiCtrl::IsUpdated()接口
//	- 1.0.0002.1517(2011-05-25)	# 当控件无效时IGuiCtrl::SetFocus()应该直接返回
//	- 1.0.0003.1746(2011-05-26)	# IGuiCtrl::SetFocus()应该同时递归设置父控件焦点
//								# IGuiCtrl::GetFocus()应该直接返回最底层的焦点控件
//	- 1.0.0004.1709(2011-06-03)	= 调整IGuiCtrl区域控制接口的命名
//								+ 添加IGuiCtrl::GetClientRect()接口
//	- 1.0.0005.2304(2011-06-08)	+ 添加IGuiCtrl::UpdateState()接口
//								+ 在IGuiCtrl::SetFocus()接口实现中添加更新状态逻辑
//	- 1.0.0006.1010(2011-06-17)	= 将IGuiCtrlBase接口移动到GuiCtrl.h中,使外部可以使用此接口
//	- 1.0.0007.1732(2011-06-24)	+ 添加静态IGuiCtrl::SetFocus()接口
//	- 1.0.0008.2000(2011-07-16)	+ 添加IGuiCtrl::GetClipRect()接口,用于绘图时动态获取当前剪切区下的绘图区域
//	- 1.0.0009.2111(2011-07-31)	= IGuiCtrl::Init()将判断控件是否可见,并发送WM_SHOWWINDOW消息
//	- 1.0.0010.1720(2011-08-03)	+ IGuiCtrl添加Scroll系列接口,支持事件响应获取滚动偏移并处理绘图
//	- 1.0.0011.1714(2011-08-08)	+ IGuiCtrl添加Scroll控件绑定接口
//	- 1.0.0012.1715(2011-08-09)	# 当IGuiCtrl没有父对象时忽略消息传递
//								^ 优化Scroll控件绑定接口的消息传递
//	- 1.0.0013.1730(2011-08-11)	= IGuiCtrl在SetFocus时先发送焦点失去消息,再发送焦点获得消息
//////////////////////////////////////////////////////////////////

#ifndef __GuiCtrl_h__
#define __GuiCtrl_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiCommon/GuiCommon.h"
#include "GuiBoard/GuiBoard.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI 控件对象接口
EXP_INTERFACE IGuiCtrl : public IGuiBase
{
	EXP_DECLARE_DYNAMIC_MULT(IGuiCtrl, IGuiBase)

public:
	EXP_STRUCT state_t : IPoolTypeT<state_t, EXP_MEMORY_ALLOC>
	{
		CString			sta_typ;
		CArrayT<void*>	sta_arr;
	};

protected:
	static IGuiCtrl* m_Focus;
	IGuiEffect* m_Effect;
	CSize m_szScroll;
	IGuiCtrl* m_Scroll;

public:
	IGuiCtrl()
		: m_Effect(NULL)
		, m_Scroll(NULL)
	{}

protected:
	void Init(IGuiComp* pComp)
	{
		EXP_BASE::Init(pComp);
		if (IsVisible())
		{
			Send(ExDynCast<IGuiObject>(this), WM_SHOWWINDOW, 1);
			SetFocus();
		}
		else
			Send(ExDynCast<IGuiObject>(this), WM_SHOWWINDOW, 0);
	}
	void Fina()
	{
		m_Focus = NULL;
		EXP_BASE::Fina();
	}

public:
	void Send(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		if (GetParent())
			IGuiSender::Send(pGui, nMessage, wParam, lParam);
	}

	// 获得控件状态
	virtual state_t* GetState(const CString& sType, CGC* pGC = NULL) = 0;
	virtual bool SetState(const CString& sType, void* pState) = 0;
	virtual void UpdateState(bool bRefreshSelf = true) = 0;
	virtual bool IsUpdated() = 0;

	// 设置效果对象
	void SetEffect(IGuiEffect* pEff)
	{
		m_Effect = pEff;
		Refresh(false);
	}
	IGuiEffect* GetEffect()
	{
		return m_Effect;
	}

	// 获得绘图板
	virtual IGuiBoard* GetBoard()
	{
		if (m_Pare)
		{
			IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(m_Pare);
			if (ctrl) return ctrl->GetBoard();
			IGuiBoard* board = ExDynCast<IGuiBoard>(m_Pare);
			if (board) return board;
		}
		return NULL;
	}
	virtual wnd_t GethWnd()
	{
		IGuiBoard* board = GetBoard();
		return board ? board->GethWnd() : NULL;
	}

	// 区域控制
	virtual bool P2C(CRect& rc) = 0;
	virtual bool C2P(CRect& rc) = 0;
	virtual bool B2C(CRect& rc) = 0;
	virtual bool C2B(CRect& rc) = 0;
	virtual bool SetWindowRect(const CRect& rc) = 0;
	virtual bool GetWindowRect(CRect& rc) = 0;
	virtual bool SetRealRect(const CRect& rc) = 0;
	virtual bool GetRealRect(CRect& rc) = 0;
	virtual bool GetClientRect(CRect& rc) = 0;
	bool GetClipRect(CRect& rc)
	{
		CRect rc_clp;
		GetClipBox(rc_clp);
		if (!GetClientRect(rc)) return false;
		rc.Offset(-rc_clp.pt1);
		return true;
	}
	bool GetScrollSize(CSize& sz) const
	{
		sz = m_szScroll;
		return true;
	}
	bool SetScrollSize(const CSize& sz, bool bWheel = false)
	{
		if (m_Scroll && bWheel)
		{
			m_Scroll->Send(
				ExDynCast<IGuiObject>(m_Scroll), WM_MOUSEWHEEL, 
				ExMakeLong(m_szScroll.cx - sz.cx, m_szScroll.cy - sz.cy), 
				ExMakeLong(-1, -1));
		}
		else
		{
			m_szScroll = sz;
			CRect rc;
			GetWindowRect(rc);
			Send(ExDynCast<IGuiObject>(this), WM_SIZE, SIZE_RESTORED, 
				(LPARAM)ExMakeLong(rc.Width(), rc.Height()));
			Refresh(false);
		}
		return true;
	}
	IGuiCtrl* GetScroll() const
	{
		return m_Scroll;
	}
	void SetScroll(const IGuiCtrl* pScroll)
	{
		if (m_Scroll = (IGuiCtrl*)pScroll)
			SetScrollSize(m_szScroll, true);
	}

	// 刷新绘图
	virtual void Refresh(bool bSelf = true) = 0;

	// 设置可用性
	virtual bool SetEnable(bool bEnable = true) = 0;
	virtual bool IsEnabled() const = 0;

	// 设置可见性
	virtual bool SetVisible(bool bVisible = true) = 0;
	virtual bool IsVisible() const = 0;

	// 判断有效性
	static bool IsEffect(IGuiCtrl* pCtrl)
	{ return (pCtrl && pCtrl->IsEnabled() && pCtrl->IsVisible()); }

	static IGuiCtrl* SetFocus(IGuiCtrl* pFoc)
	{
		if (pFoc && !IsEffect(pFoc)) return NULL;
		// 设置控件焦点
		IGuiCtrl* old_fc = m_Focus;
		m_Focus = pFoc;
		if (old_fc == m_Focus) return NULL;
		// 设置窗口焦点
		if (m_Focus)
		{
			IGuiBoard* board = m_Focus->GetBoard();
			if (board) board->SetFocus();
		}
		// 发送焦点改变消息
		if (old_fc)
		{
			old_fc->Send(ExDynCast<IGuiObject>(old_fc), WM_KILLFOCUS, 0, (LPARAM)(m_Focus));
			old_fc->UpdateState();
		}
		if (m_Focus)
		{
			m_Focus->Send(ExDynCast<IGuiObject>(m_Focus), WM_SETFOCUS, 0, (LPARAM)old_fc);
			m_Focus->UpdateState();
		}
		return old_fc;
	}
	virtual IGuiCtrl* SetFocus()
	{
		return SetFocus(this);
	}
	static IGuiCtrl* GetFocus()
	{
		return m_Focus;
	}
	virtual bool IsFocus()
	{
		IGuiBoard* board = GetBoard();
		if (board && !board->IsFocus())
			return false;
		if (!m_Focus) return false;
		IGuiCtrl* foc = m_Focus;
		if (foc == this)
			return IsEffect(this);
		for(list_t::iterator_t ite = GetChildren().Head(); ite != GetChildren().Tail(); ++ite)
		{
			IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(*ite);
			if (!ctrl) continue;
			if (ctrl->IsFocus()) return true;
		}
		return false;
	}
};

//////////////////////////////////////////////////////////////////

// GUI 控件对象接口
EXP_INTERFACE IGuiCtrlBase : public IGuiCtrl
{
	EXP_DECLARE_DYNAMIC_MULT(IGuiCtrlBase, IGuiCtrl)

protected:
	bool m_bEnable;		// 是否可用
	bool m_bVisible;	// 是否可见

	CRect m_Rect;		// 控件区域

	bool m_Updated;

public:
	IGuiCtrlBase();

public:
	// 更新状态
	state_t* GetState(const CString& sType, CGC* pGC = NULL);
	bool SetState(const CString& sType, void* pState);
	void UpdateState(bool bRefreshSelf = true);
	bool IsUpdated();

	// 区域控制
	bool P2C(CRect& rc);
	bool C2P(CRect& rc);
	bool B2C(CRect& rc);
	bool C2B(CRect& rc);
	bool SetWindowRect(const CRect& rc);
	bool GetWindowRect(CRect& rc);
	bool SetRealRect(const CRect& rc);
	bool GetRealRect(CRect& rc);
	bool GetClientRect(CRect& rc);

	// 刷新绘图
	void Refresh(bool bSelf = true);

	// 设置可用性
	bool SetEnable(bool bEnable = true);
	bool IsEnabled() const;

	// 设置可见性
	bool SetVisible(bool bVisible = true);
	bool IsVisible() const;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiCtrl_h__*/