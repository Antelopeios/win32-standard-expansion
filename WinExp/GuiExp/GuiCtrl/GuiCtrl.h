// Copyright 2011-2012, ľͷ��
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
// Date:	2012-02-02
// Version:	1.0.0018.1802
//
// History:
//	- 1.0.0001.2236(2011-05-23)	+ IGuiCtrl���Ч��������ؽӿ�
//								+ ���IGuiCtrl::IsUpdated()�ӿ�
//	- 1.0.0002.1517(2011-05-25)	# ���ؼ���ЧʱIGuiCtrl::SetFocus()Ӧ��ֱ�ӷ���
//	- 1.0.0003.1746(2011-05-26)	# IGuiCtrl::SetFocus()Ӧ��ͬʱ�ݹ����ø��ؼ�����
//								# IGuiCtrl::GetFocus()Ӧ��ֱ�ӷ�����ײ�Ľ���ؼ�
//	- 1.0.0004.1709(2011-06-03)	= ����IGuiCtrl������ƽӿڵ�����
//								+ ���IGuiCtrl::GetClientRect()�ӿ�
//	- 1.0.0005.2304(2011-06-08)	+ ���IGuiCtrl::UpdateState()�ӿ�
//								+ ��IGuiCtrl::SetFocus()�ӿ�ʵ������Ӹ���״̬�߼�
//	- 1.0.0006.1010(2011-06-17)	= ��IGuiCtrlBase�ӿ��ƶ���GuiCtrl.h��,ʹ�ⲿ����ʹ�ô˽ӿ�
//	- 1.0.0007.1732(2011-06-24)	+ ��Ӿ�̬IGuiCtrl::SetFocus()�ӿ�
//	- 1.0.0008.2000(2011-07-16)	+ ���IGuiCtrl::GetClipRect()�ӿ�,���ڻ�ͼʱ��̬��ȡ��ǰ�������µĻ�ͼ����
//	- 1.0.0009.2111(2011-07-31)	= IGuiCtrl::Init()���жϿؼ��Ƿ�ɼ�,������WM_SHOWWINDOW��Ϣ
//	- 1.0.0010.1720(2011-08-03)	+ IGuiCtrl���Scrollϵ�нӿ�,֧���¼���Ӧ��ȡ����ƫ�Ʋ������ͼ
//	- 1.0.0011.1714(2011-08-08)	+ IGuiCtrl���Scroll�ؼ��󶨽ӿ�
//	- 1.0.0012.1715(2011-08-09)	# ��IGuiCtrlû�и�����ʱ������Ϣ����
//								^ �Ż�Scroll�ؼ��󶨽ӿڵ���Ϣ����
//	- 1.0.0013.1730(2011-08-11)	= IGuiCtrl��SetFocusʱ�ȷ��ͽ���ʧȥ��Ϣ,�ٷ��ͽ�������Ϣ
//	- 1.0.0014.1816(2011-08-24)	+ ��IGuiCtrl���һ��Scroll����ʱ,�Զ���������main����
//	- 1.0.0015.1643(2011-08-25)	^ �����IGuiCtrl::GetState()�ӿڵĵ��÷�ʽ��Ч��
//	- 1.0.0016.1318(2011-08-31)	^ ��IGuiCtrl::SetScroll()����Ĳ����뵱ǰm_Scroll���,��������m_Scroll��ˢ�²���
//	- 1.0.0017.2154(2011-09-22)	^ ��IGuiCtrl::SetScrollSize()����Ĳ����뵱ǰm_szScroll���,�������к�������Ϣ����
//	- 1.0.0018.1802(2012-02-02)	+ ���IGuiCtrl��Ч��������йܽӿ�
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
EXP_INTERFACE IGuiCtrl : public IGuiBase
{
	EXP_DECLARE_DYNAMIC_MULT(IGuiCtrl, IGuiBase)

protected:
	static IGuiCtrl* m_Focus;
	IGuiEffect* m_Effect;
	BOOL m_bTruEff;
	CSize m_szScroll;
	IGuiCtrl* m_Scroll;

public:
	IGuiCtrl()
		: m_Effect(NULL)
		, m_bTruEff(TRUE)
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
		if (m_bTruEff && m_Effect)
			del(m_Effect);
		m_Focus = NULL;
		EXP_BASE::Fina();
	}

public:
	void Send(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		if (GetParent())
			IGuiSender::Send(pGui, nMessage, wParam, lParam);
	}

	// ��ÿؼ�״̬
	virtual void* GetState(const CString& sType) = 0;
	virtual BOOL SetState(const CString& sType, void* pState) = 0;
	virtual void UpdateState(BOOL bRefreshSelf = TRUE) = 0;
	virtual BOOL IsUpdated() = 0;

	// ����Ч������
	void SetEffectTrust(BOOL bTru = TRUE)
	{
		m_bTruEff = bTru;
	}
	void SetEffect(IGuiEffect* pEff)
	{
		if (m_bTruEff && m_Effect)
			del(m_Effect);
		m_Effect = pEff;
		Refresh(FALSE);
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
	virtual BOOL P2C(CRect& rc) = 0;
	virtual BOOL C2P(CRect& rc) = 0;
	virtual BOOL B2C(CRect& rc) = 0;
	virtual BOOL C2B(CRect& rc) = 0;
	virtual BOOL SetWindowRect(const CRect& rc) = 0;
	virtual BOOL GetWindowRect(CRect& rc) = 0;
	virtual BOOL SetRealRect(const CRect& rc) = 0;
	virtual BOOL GetRealRect(CRect& rc) = 0;
	virtual BOOL GetClientRect(CRect& rc) = 0;
	BOOL GetClipRect(CRect& rc)
	{
		CRect rc_clp;
		GetClipBox(rc_clp);
		if (!GetClientRect(rc)) return FALSE;
		rc.Offset(-rc_clp.pt1);
		return TRUE;
	}
	BOOL GetScrollSize(CSize& sz) const
	{
		sz = m_szScroll;
		return TRUE;
	}
	BOOL SetScrollSize(const CSize& sz, BOOL bWheel = FALSE)
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
			if (m_szScroll == sz) return TRUE;
			m_szScroll = sz;
			CRect rc;
			GetWindowRect(rc);
			Send(ExDynCast<IGuiObject>(this), WM_SIZE, SIZE_RESTORED, 
				(LPARAM)ExMakeLong(rc.Width(), rc.Height()));
			Refresh(FALSE);
		}
		return TRUE;
	}
	IGuiCtrl* GetScroll() const
	{
		return m_Scroll;
	}
	void SetScroll(IGuiCtrl* pScroll)
	{
		IGuiCtrl* old_scr = m_Scroll;
		m_Scroll = pScroll;
		if (m_Scroll && old_scr != m_Scroll)
		{
			m_Scroll->SetState(_T("main"), this);
			SetScrollSize(m_szScroll, TRUE);
		}
	}

	// ˢ�»�ͼ
	virtual void Refresh(BOOL bSelf = TRUE) = 0;

	// ���ÿ�����
	virtual BOOL SetEnable(BOOL bEnable = TRUE) = 0;
	virtual BOOL IsEnabled() const = 0;

	// ���ÿɼ���
	virtual BOOL SetVisible(BOOL bVisible = TRUE) = 0;
	virtual BOOL IsVisible() const = 0;

	// �ж���Ч��
	static BOOL IsEffect(IGuiCtrl* pCtrl)
	{ return (pCtrl && pCtrl->IsEnabled() && pCtrl->IsVisible()); }

	static IGuiCtrl* SetFocus(IGuiCtrl* pFoc)
	{
		if (pFoc && !IsEffect(pFoc)) return NULL;
		// ���ÿؼ�����
		IGuiCtrl* old_fc = m_Focus;
		m_Focus = pFoc;
		if (old_fc == m_Focus) return NULL;
		// ���ô��ڽ���
		if (m_Focus)
		{
			IGuiBoard* board = m_Focus->GetBoard();
			if (board) board->SetFocus();
		}
		// ���ͽ���ı���Ϣ
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
	virtual BOOL IsFocus()
	{
		IGuiBoard* board = GetBoard();
		if (board && !board->IsFocus())
			return FALSE;
		if (!m_Focus) return FALSE;
		IGuiCtrl* foc = m_Focus;
		if (foc == this)
			return IsEffect(this);
		for(list_t::iterator_t ite = GetChildren().Head(); ite != GetChildren().Tail(); ++ite)
		{
			IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(*ite);
			if (!ctrl) continue;
			if (ctrl->IsFocus()) return TRUE;
		}
		return FALSE;
	}
};

//////////////////////////////////////////////////////////////////

// GUI �ؼ�����ӿ�
EXP_INTERFACE IGuiCtrlBase : public IGuiCtrl
{
	EXP_DECLARE_DYNAMIC_MULT(IGuiCtrlBase, IGuiCtrl)

protected:
	BOOL m_bEnable;		// �Ƿ����
	BOOL m_bVisible;	// �Ƿ�ɼ�

	CRect m_Rect;		// �ؼ�����

	BOOL m_Updated;

public:
	IGuiCtrlBase();

public:
	// ����״̬
	void* GetState(const CString& sType);
	BOOL SetState(const CString& sType, void* pState);
	void UpdateState(BOOL bRefreshSelf = TRUE);
	BOOL IsUpdated();

	// �������
	BOOL P2C(CRect& rc);
	BOOL C2P(CRect& rc);
	BOOL B2C(CRect& rc);
	BOOL C2B(CRect& rc);
	BOOL SetWindowRect(const CRect& rc);
	BOOL GetWindowRect(CRect& rc);
	BOOL SetRealRect(const CRect& rc);
	BOOL GetRealRect(CRect& rc);
	BOOL GetClientRect(CRect& rc);

	// ˢ�»�ͼ
	void Refresh(BOOL bSelf = TRUE);

	// ���ÿ�����
	BOOL SetEnable(BOOL bEnable = TRUE);
	BOOL IsEnabled() const;

	// ���ÿɼ���
	BOOL SetVisible(BOOL bVisible = TRUE);
	BOOL IsVisible() const;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiCtrl_h__*/