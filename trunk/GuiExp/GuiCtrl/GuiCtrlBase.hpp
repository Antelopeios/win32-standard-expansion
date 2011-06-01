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
// GuiCtrlBase - 控件基础实现
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-25
// Version:	1.0.0001.1500
//
// History:
//	- 1.0.0001.1500(2011-05-25)	+ IGuiCtrlBase添加控件状态接口基本实现
//								= 将控件GC统一到IGuiCtrlBase内定义
//								= 当可见性或可用性发生改变时,控件将设置更新状态
//////////////////////////////////////////////////////////////////

#ifndef __GuiCtrlBase_hpp__
#define __GuiCtrlBase_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI 控件对象接口
interface IGuiCtrlBase : public IGuiCtrl
{
	EXP_DECLARE_DYNAMIC_MULT(IGuiCtrlBase, IGuiCtrl)

protected:
	CGC m_GC;

	bool m_bEnable;		// 是否可用
	bool m_bVisible;	// 是否可见

	CRect m_Rect;		// 控件区域

	bool m_Updated;

public:
	IGuiCtrlBase()
		: m_bEnable(true)
		, m_bVisible(true)
		, m_Updated(true)
	{}

public:
	// 更新状态
	state_t* GetState(const CString& sType, CGC* pGC = NULL)
	{
		state_t* state = pGC ? ExMem::Alloc<state_t>(pGC) : state_t::Alloc();
		ExAssert(state);
		if (state) state->sta_typ = sType;
		return state;
	}
	void SetState(const CString& sType, void* pState)
	{
		if (!pState) return;
		m_Updated = true;
		Refresh();
	}
	bool IsUpdated()
	{
		bool updt = m_Updated;
		m_Updated = false; // 外部一旦获知当前状态,则更新状态自动复位
		return updt;
	}

	// 区域控制
	bool P2C(CRect& rc)
	{
		rc.Offset(-(m_Rect.pt1));
		return true;
	}
	bool C2P(CRect& rc)
	{
		rc.Offset(m_Rect.pt1);
		return true;
	}
	bool B2C(CRect& rc)
	{
		if (!m_Pare) return false;
		if (!P2C(rc)) return false;
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(m_Pare);
		if (ctrl) return ctrl->B2C(rc);
		return ExDynCast<IGuiBoard>(m_Pare) ? true : false;
	}
	bool C2B(CRect& rc)
	{
		if (!m_Pare) return false;
		if (!C2P(rc)) return false;
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(m_Pare);
		if (ctrl) return ctrl->C2B(rc);
		return ExDynCast<IGuiBoard>(m_Pare) ? true : false;
	}
	bool SetRect(const CRect& rc)
	{
		m_Rect = rc;
		Refresh(false);
		return true;
	}
	bool GetRect(CRect& rc)
	{
		rc = m_Rect;
		return true;
	}
	bool SetRealRect(const CRect& rc)
	{
		CRect rc_tmp(rc);
		if (!B2C(rc_tmp)) return false;
		return SetRect(rc_tmp);
	}
	bool GetRealRect(CRect& rc)
	{
		if (!m_Pare) return false;
		if (!GetRect(rc)) return false;
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(m_Pare);
		if (ctrl) return ctrl->C2B(rc);
		return ExDynCast<IGuiBoard>(m_Pare) ? true : false;
	}

	// 刷新绘图
	void Refresh(bool bSelf = true)
	{
		IGuiBoard* board = GetBoard();
		if (!board) return;
		if (bSelf)
		{
			CRect rc;
			GetRealRect(rc);
			board->InvalidateRect(rc);
		}
		else
			board->Invalidate();
	}

	// 设置可用性
	bool SetEnable(bool bEnable = true)
	{
		bool old = m_bEnable;
		m_bEnable = bEnable;
		m_Updated = true;
		Refresh();
		return old;
	}
	bool IsEnabled() const { return m_bEnable; }

	// 设置可见性
	bool SetVisible(bool bVisible = true)
	{
		bool old = m_bVisible;
		m_bVisible = bVisible;
		m_Updated = true;
		Refresh();
		return old;
	}
	bool IsVisible() const { return m_bVisible; }
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNAMIC_MULT(IGuiCtrlBase, IGuiCtrl)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiCtrlBase_hpp__*/