// Copyright 2011, ľͷ��
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
// GuiCtrl - �ؼ�����
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-06-03
// Version:	1.0.0004.1709
//
// History:
//	- 1.0.0001.2236(2011-05-23)	+ IGuiCtrl����Ч��������ؽӿ�
//								+ ����IGuiCtrl::IsUpdated()�ӿ�
//	- 1.0.0002.1517(2011-05-25)	# ���ؼ���ЧʱIGuiCtrl::SetFocus()Ӧ��ֱ�ӷ���
//	- 1.0.0003.1746(2011-05-26)	# IGuiCtrl::SetFocus()Ӧ��ͬʱ�ݹ����ø��ؼ�����
//								# IGuiCtrl::GetFocus()Ӧ��ֱ�ӷ�����ײ�Ľ���ؼ�
//	- 1.0.0004.1709(2011-06-03)	= ����IGuiCtrl������ƽӿڵ�����
//								+ ����IGuiCtrl::GetClientRect()�ӿ�
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

// GUI �ؼ�����ӿ�
interface EXP_API IGuiCtrl : public IGuiBase
{
	EXP_DECLARE_DYNAMIC_MULT(IGuiCtrl, IGuiBase)

public:
	struct state_t : IPoolTypeT<state_t, EXP_MEMORY_ALLOC>
	{
		CString			sta_typ;
		CArrayT<void*>	sta_arr;
	};

protected:
	static IGuiCtrl* m_Focus;
	IGuiEffect* m_Effect;

public:
	IGuiCtrl()
		: m_Effect(NULL)
	{}

protected:
	void Init(IGuiComp* pComp)
	{
		EXP_BASE::Init(pComp);
		SetFocus();
	}
	void Fina()
	{
		m_Focus = NULL;
		EXP_BASE::Fina();
	}

public:
	// ��ÿؼ�״̬
	virtual state_t* GetState(const CString& sType, CGC* pGC = NULL) = 0;
	virtual void SetState(const CString& sType, void* pState) = 0;
	virtual bool IsUpdated() = 0;

	// ����Ч������
	void SetEffect(IGuiEffect* pEff)
	{
		m_Effect = pEff;
		Refresh(false);
	}
	IGuiEffect* GetEffect()
	{
		return m_Effect;
	}

	// ��û�ͼ��
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

	// �������
	virtual bool P2C(CRect& rc) = 0;
	virtual bool C2P(CRect& rc) = 0;
	virtual bool B2C(CRect& rc) = 0;
	virtual bool C2B(CRect& rc) = 0;
	virtual bool SetWindowRect(const CRect& rc) = 0;
	virtual bool GetWindowRect(CRect& rc) = 0;
	virtual bool SetRealRect(const CRect& rc) = 0;
	virtual bool GetRealRect(CRect& rc) = 0;
	virtual bool GetClientRect(CRect& rc) = 0;

	// ˢ�»�ͼ
	virtual void Refresh(bool bSelf = true) = 0;

	// ���ÿ�����
	virtual bool SetEnable(bool bEnable = true) = 0;
	virtual bool IsEnabled() const = 0;

	// ���ÿɼ���
	virtual bool SetVisible(bool bVisible = true) = 0;
	virtual bool IsVisible() const = 0;

	// �ж���Ч��
	static bool IsEffect(IGuiCtrl* pCtrl)
	{ return (pCtrl && pCtrl->IsEnabled() && pCtrl->IsVisible()); }

	virtual IGuiCtrl* SetFocus()
	{
		IGuiBoard* board = GetBoard();
		if (!board) return NULL;
		if (!IsEffect(this)) return NULL;
		// ���ý���
		IGuiCtrl* old_fc = m_Focus;
		m_Focus = this;
		if (old_fc == m_Focus) return NULL;
		board->SetFocus();
		// ���ͽ���ı���Ϣ
		m_Focus->Send(ExDynCast<IGuiObject>(m_Focus), WM_SETFOCUS, 0, (LPARAM)old_fc);
		if (!old_fc) return old_fc;
		old_fc->Send(ExDynCast<IGuiObject>(old_fc), WM_KILLFOCUS, 0, (LPARAM)(m_Focus));
		board->Invalidate();
		return old_fc;
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

EXP_END

#endif/*__GuiCtrl_h__*/