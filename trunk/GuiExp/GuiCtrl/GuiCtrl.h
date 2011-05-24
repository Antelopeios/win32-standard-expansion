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
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2010-05-23
// Version:	1.0.0001.2236
//
// History:
//	- 1.0.0001.2236(2010-05-23)	+ IGuiCtrl添加效果对象相关接口
//								+ 添加IGuiCtrl::IsUpdated()接口
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
interface EXP_API IGuiCtrl : public IGuiBase
{
	EXP_DECLARE_DYNAMIC_MULT(IGuiCtrl, IGuiBase)

public:
	struct state_t : IPoolTypeT<state_t, EXP_MULT::alloc_t>
	{
		CString			sta_typ;
		CArrayT<void*>	sta_arr;
	};

protected:
	IGuiCtrl** m_Focus;
	IGuiEffect* m_Effect;

public:
	IGuiCtrl()
		: m_Focus(NULL)
		, m_Effect(NULL)
	{}

protected:
	void Init(IGuiComp* pComp)
	{
		EXP_BASE::Init(pComp);
		if (!pComp) return;
		if (pComp->Empty())
		{
			m_Focus = ExMem::Alloc<IGuiCtrl*>();
			(*m_Focus) = NULL;
		}
		else
		{
			IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pComp->LastItem());
			ExAssert(ctrl);
			m_Focus = ctrl->m_Focus;
		}
		SetFocus();
	}
	void Fina()
	{
		if (m_Pare && m_Pare->Empty())
			ExMem::Free(m_Focus);
		m_Focus = NULL;
		EXP_BASE::Fina();
	}

public:
	// 获得控件状态
	virtual state_t* GetState(const CString& sType, CGC* pGC = NULL) = 0;
	virtual void SetState(const CString& sType, void* pState) = 0;
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

	// 区域控制
	virtual bool P2C(CRect& rc) = 0;
	virtual bool C2P(CRect& rc) = 0;
	virtual bool B2C(CRect& rc) = 0;
	virtual bool C2B(CRect& rc) = 0;
	virtual bool SetRect(const CRect& rc) = 0;
	virtual bool GetRect(CRect& rc) = 0;
	virtual bool SetRealRect(const CRect& rc) = 0;
	virtual bool GetRealRect(CRect& rc) = 0;

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

	virtual IGuiCtrl* SetFocus(IGuiCtrl* pFocus = NULL)
	{
		if (!m_Focus) return NULL;
		// 设置焦点
		IGuiCtrl* old_fc = (*m_Focus);
		(*m_Focus) = pFocus ? pFocus : this;
		if (old_fc == (*m_Focus)) return NULL;
		// 发送焦点改变消息
		ExAssert(*m_Focus);
		(*m_Focus)->Send(ExDynCast<IGuiObject>(*m_Focus), WM_SETFOCUS, 0, (LPARAM)old_fc);
		if (!old_fc) return old_fc;
		old_fc->Send(ExDynCast<IGuiObject>(old_fc), WM_KILLFOCUS, 0, (LPARAM)(*m_Focus));
		return old_fc;
	}
	virtual IGuiCtrl* GetFocus()
	{
		return (*m_Focus);
	}
	virtual bool IsFocus()
	{
		IGuiBoard* board = GetBoard();
		if (board && !board->IsFocus())
			return false;
		if (!m_Focus) return false;
		IGuiCtrl* foc = GetFocus();
		if (foc == this)
			return IsEffect(this);
		for(list_t::iterator_t ite = list_t::Head(); ite != list_t::Tail(); ++ite)
		{
			IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(*ite);
			if (!ctrl) continue;
			if (ctrl->IsFocus()) return true;
		}
		return false;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiCtrl_h__*/