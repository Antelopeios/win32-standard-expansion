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
// Date:	2010-05-13
// Version:	1.0.0000.1600
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
interface EXP_API IGuiCtrl : public IGuiComp, public IGuiSender
{
	EXP_DECLARE_DYNAMIC_MULT2(IGuiCtrl, IGuiComp, IGuiSender)

protected:
	IGuiCtrl** m_Focus;

	bool m_bEnable;		// 是否可用
	bool m_bVisible;	// 是否可见

	CRect m_Rect;		// 控件区域

public:
	IGuiCtrl()
		: m_Focus(NULL)
		, m_bEnable(true)
		, m_bVisible(true)
	{}

protected:
	void Init(IGuiComp* pComp)
	{
		EXP_BASE::Init(pComp);
		if (!pComp) return;
		if (pComp->Empty())
			m_Focus = ExMem::Alloc<IGuiCtrl*>();
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
	IGuiBoard* GetBoard()
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
	CRect rc_p;
	board->GetClientRect(rc_p);

	// 区域控制
	void P2C(CRect& rc)
	{
		if (!m_Pare) return;
		IGuiBoard* board = GetBoard();
		if (!board) return;
		if (m_Pare == board)
		{
		}
		else
		{
		}
	}
	void SetRect(const CRect& rc) { m_Rect = rc; }
	void GetRect(CRect& rc) { rc = m_Rect; }
	void SetRealRect(const CRect& rc)
	{
	}
	void GetRealRect(CRect& rc)
	{
	}

	// 刷新绘图
	void Refresh()
	{
		IGuiBoard* board = GetBoard();
		if (!board) return;
		CRect rc;
		GetRealRect(rc);
		board->InvalidateRect(rc);
	}

	// 设置可用性
	bool SetEnable(bool bEnable = true)
	{
		bool old = m_bEnable;
		m_bEnable = bEnable;
		return old;
	}
	bool IsEnabled() const { return m_bEnable; }

	// 设置可见性
	bool SetVisible(bool bVisible = true)
	{
		bool old = m_bVisible;
		m_bVisible = bVisible;
		return old;
	}
	bool IsVisible() const { return m_bVisible; }

	// 判断有效性
	static bool IsEffect(IGuiCtrl* pCtrl)
	{ return (pCtrl && pCtrl->IsEnabled() && pCtrl->IsVisible()); }

	IGuiCtrl* SetFocus(IGuiCtrl* pFocus = NULL)
	{
		if (!m_Focus) return NULL;
		// 设置焦点
		IGuiCtrl* old_fc = (*m_Focus);
		(*m_Focus) = pFocus ? pFocus : this;
		if (old_fc == (*m_Focus)) return NULL;
		// 发送焦点改变消息
		ExAssert(*m_Focus);
		(*m_Focus)->Send(WM_SETFOCUS, 0, (LPARAM)old_fc);
		if (!old_fc) return old_fc;
		old_fc->Send(WM_KILLFOCUS, 0, (LPARAM)(*m_Focus));
		return old_fc;
	}
	IGuiCtrl* GetFocus()
	{
		return (*m_Focus);
	}
	bool IsFocus()
	{
		IGuiBoard* board = GetBoard();
		if (board && !board->IsFocus())
			return false;
		if (!m_Focus) return false;
		IGuiCtrl* foc = GetFocus();
		if (foc == this)
			return IsEffect(this);
		else
		if (!Empty())
			for(list_t::iterator_t ite = list_t::Head(); ite != list_t::Tail(); ++ite)
			{
				if (!(*ite)) continue;
				if ((*ite)->IsFocus()) return true;
			}
		return false;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiCtrl_h__*/