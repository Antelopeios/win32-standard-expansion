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
// Date:	2012-03-16
// Version:	1.0.0024.2356
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
//	- 1.0.0019.1200(2012-02-22)	= ��IGuiCtrl����ΪIGuiCtl
//	- 1.0.0020.1723(2012-02-29)	% �޸Ĳ����ƹ��������ƻ���,��֧��ˮƽ�봹ֱ����ģʽ�Ĺ�����
//	- 1.0.0021.2237(2012-03-03)	+ ���itree_t���ͼ���,Ϊ��Ҫ���ͼ��ϵĿؼ���׼��
//								+ ���set_ins_t,���ڷ�����Ƽ��ϵĲ������,��Ϊ�����Ϳؼ����insert,delete��clear����
//	- 1.0.0022.0034(2012-03-04)	# ������IGuiCtl::SetWindowRect()��ʱ��Ӧ�ÿ��ǹ���ƫ��,����ᵼ�¿ؼ�λ�õ���ʾ�쳣
//	- 1.0.0023.1603(2012-03-12)	+ ���IGuiCtl::IsNeedScroll()�ӿ�,�����ⲿ�жϵ�ǰ�Ƿ���Ҫ��ʾ������
//	- 1.0.0024.2356(2012-03-16)	^ �Ƴ�IGuiCtl::set_ins_t,��IGuiCtl::GetState()��IGuiCtl::SetState()�ӿ�����Ӷ����param,��֧�ָ�Ϊͨ�õ������趨
//								- �Ƴ�IGuiCtl::GetClipRect(),���ڼ������Ŀ���ȫ��������ͼ�߼��㸺��
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
EXP_INTERFACE IGuiCtl : public IGuiBase, public IGuiSetMgr
{
	EXP_DECLARE_DYNAMIC_MULT(IGuiCtl, IGuiBase)

public:
	typedef CListT<IGuiCtl*> items_t;
	typedef CTreeT<IGuiCtl*> itree_t;

protected:
	static IGuiCtl* m_Focus;
	IGuiEffect* m_Effect;
	BOOL m_bTruEff;
	CSize m_szScroll;
	IGuiCtl* (m_Scroll[2]);

public:
	IGuiCtl()
		: m_Effect(NULL)
		, m_bTruEff(TRUE)
	{
		m_Scroll[1] = m_Scroll[0] = NULL;
	}
	BOOL IsValid() const { return EXP_BASE::IsValid(); }

protected:
	void Init(IGuiComp* pComp)
	{
		EXP_BASE::Init(pComp);
		if (IsVisible())
		{
			SendMessage(WM_SHOWWINDOW, 1);
			SetFocus();
		}
		else
			SendMessage(WM_SHOWWINDOW, 0);
	}
	void Fina()
	{
		if (m_bTruEff && m_Effect)
			del(m_Effect);
		m_Focus = NULL;
		EXP_BASE::Fina();
	}

public:
	void Send(void* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		if(!GetParent()) return;
		for(set_list_t::iterator_t ite = GetSet().Head(); ite != GetSet().Tail(); ++ite)
		{
			IGuiSet* set = ExDynCast<IGuiSet>(*ite);
			if(!set) continue;
			set->Msg(nMessage, wParam, lParam);
		}
		IGuiSender::Send(pGui, nMessage, wParam, lParam);
	}
	void SendMessage(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0) { Send(this, nMessage, wParam, lParam); }

	// �ؼ����ö������
	void AddSet(void* p)
	{
		IGuiSet* set = ExDynCast<IGuiSet>(p);
		if (!set) return;
		set->Ctl() = this;
		IGuiSetMgr::AddSet(p);
	}
	void InsSet(void* p)
	{
		IGuiSet* set = ExDynCast<IGuiSet>(p);
		if (!set) return;
		set->Ctl() = this;
		IGuiSetMgr::InsSet(p);
	}

	// ��ÿؼ�״̬
	virtual void* GetState(const CString& sType, void* pParam = NULL) = 0;
	virtual BOOL SetState(const CString& sType, void* pState, void* pParam = NULL) = 0;
	virtual void UpdateState(BOOL bRefreshSelf = TRUE) = 0;
	virtual BOOL IsUpdated() = 0;

	// ����Ч������
	void SetEffectTrust(BOOL bTru = TRUE)
	{
		m_bTruEff = bTru;
	}
	void SetEffect(void* p)
	{
		if (m_bTruEff && m_Effect)
			del(m_Effect);
		m_Effect = ExDynCast<IGuiEffect>(p);
		Refresh(FALSE);
	}
	IGuiEffect* GetEffect()
	{
		return m_Effect;
	}

	// ��û�ͼ��
	IGuiBase* GetParent() const { return ExDynCast<IGuiBase>(m_Pare); }
	IGuiWnd* GetWnd() const
	{
		if (m_Pare)
		{
			IGuiCtl* ctl = ExDynCast<IGuiCtl>(m_Pare);
			if (ctl) return ctl->GetWnd();
			IGuiWnd* wnd = ExDynCast<IGuiWnd>(m_Pare);
			if (wnd) return wnd;
		}
		return NULL;
	}
	wnd_t GethWnd() const
	{
		IGuiWnd* wnd = GetWnd();
		return wnd ? wnd->GethWnd() : NULL;
	}

	// �������
	virtual BOOL GetRect(CRect& rc) const = 0;
	virtual BOOL SetRect(const CRect& rc) = 0;

	virtual BOOL P2C(CRect& rc) const = 0;
	virtual BOOL C2P(CRect& rc) const = 0;
	virtual BOOL B2C(CRect& rc) const = 0;
	virtual BOOL C2B(CRect& rc) const = 0;
	virtual BOOL SetWindowRect(const CRect& rc) = 0;
	virtual BOOL SetRealRect(const CRect& rc) = 0;
	virtual BOOL GetAllRect(CSize& sz) const = 0;
	virtual BOOL GetFraRect(CSize& sz) const = 0;

	BOOL GetScrollSize(CSize& sz) const
	{
		sz = m_szScroll;
		return TRUE;
	}
	BOOL SetScrollSize(const CSize& sz, BOOL bWheel = FALSE)
	{
		if ((m_Scroll[0] || m_Scroll[1]) && bWheel)
		{
			if (m_Scroll[0])
				m_Scroll[0]->SendMessage(WM_MOUSEWHEEL, 
					ExMakeLong(-sz.cx, -sz.cy), 
					ExMakeLong(-1, -1));
			if (m_Scroll[1])
				m_Scroll[1]->SendMessage(WM_MOUSEWHEEL, 
					ExMakeLong(-sz.cx, -sz.cy), 
					ExMakeLong(-1, -1));
		}
		else
		{
			if (m_szScroll == sz) return TRUE;
			m_szScroll = sz;
			CRect rc;
			GetRect(rc);
			SendMessage(WM_SIZE, SIZE_RESTORED, 
				(LPARAM)ExMakeLong(rc.Width(), rc.Height()));
			Refresh(FALSE);
		}
		return TRUE;
	}
	IGuiCtl* GetScroll(BOOL bLine = TRUE) const
	{
		return m_Scroll[bLine ? 0 : 1];
	}
	void SetScroll(void* p, BOOL bLine = TRUE)
	{
		int inx = bLine ? 0 : 1;
		IGuiCtl* old_scr = m_Scroll[inx];
		m_Scroll[inx] = ExDynCast<IGuiCtl>(p);
		if (m_Scroll[inx] && old_scr != m_Scroll[inx])
		{
			m_Scroll[inx]->SetState(_T("main"), this);
			SetScrollSize(CSize(), TRUE);
		}
	}
	BOOL IsNeedScroll(BOOL bLine = TRUE)
	{
		CSize all_line, fra_line;
		GetAllRect(all_line);
		GetFraRect(fra_line);
		LONG all = 0, fra = 0;
		if (bLine)
		{
			all = all_line.cy;
			fra = fra_line.cy;
		}
		else
		{
			all = all_line.cx;
			fra = fra_line.cx;
		}
		return (GetScroll(bLine) ? (all > fra) : FALSE);
	}

	// �ж��Ƿ����(��ؼ��ڸ��ؼ�������)
	BOOL IsDisplayed() const
	{
		CRect rc_slf;
		if (!GetWindowRect(rc_slf)) return FALSE;
		CRect rc_prt;
		if (!GetParent()->GetClientRect(rc_prt)) return FALSE;
		return (!rc_slf.Inter(rc_prt).IsEmpty());
	}
	BOOL GetDisplaySize(CSize& sz) const
	{
		sz.CSizeT::CSizeT();
		CRect rc_slf;
		if (!GetWindowRect(rc_slf)) return FALSE;
		CRect rc_prt;
		if (!GetParent()->GetClientRect(rc_prt)) return FALSE;
		CRect rc_int(rc_slf);
		if (rc_int.Inter(rc_prt) != rc_slf)
		{
			if (rc_int.Left() == rc_slf.Left() || 
				rc_int.Right() == rc_slf.Right())
			{
				if (rc_int.Left() > rc_slf.Left())
					sz.cx = rc_slf.Left() - rc_int.Left();
				if (rc_int.Right() < rc_slf.Right())
					sz.cx = rc_slf.Right() - rc_int.Right();
			}
			if (rc_int.Top() == rc_slf.Top() || 
				rc_int.Bottom() == rc_slf.Bottom())
			{
				if (rc_int.Top() > rc_slf.Top())
					sz.cy = rc_slf.Top() - rc_int.Top();
				if (rc_int.Bottom() < rc_slf.Bottom())
					sz.cy = rc_slf.Bottom() - rc_int.Bottom();
			}
		}
		return TRUE;
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
	static BOOL IsEffect(const IGuiCtl* pCtrl)
	{ return (pCtrl && pCtrl->IsEnabled() && pCtrl->IsVisible()); }

	static IGuiCtl* SetFocus(IGuiCtl* pFoc)
	{
		if (pFoc && !IsEffect(pFoc)) return NULL;
		// ���ÿؼ�����
		IGuiCtl* old_fc = m_Focus;
		m_Focus = pFoc;
		if (old_fc == m_Focus) return NULL;
		// ���ô��ڽ���
		if (m_Focus)
		{
			IGuiWnd* wnd = m_Focus->GetWnd();
			if (wnd) wnd->SetFocus();
		}
		// ���ͽ���ı���Ϣ
		if (old_fc)
		{
			old_fc->SendMessage(WM_KILLFOCUS, 0, (LPARAM)(m_Focus));
			old_fc->UpdateState();
		}
		if (m_Focus)
		{
			m_Focus->SendMessage(WM_SETFOCUS, 0, (LPARAM)old_fc);
			m_Focus->UpdateState();
		}
		return old_fc;
	}
	virtual IGuiCtl* SetFocus()
	{
		return SetFocus(this);
	}
	static IGuiCtl* GetFocus()
	{
		return m_Focus;
	}
	virtual BOOL IsFocus() const
	{
		IGuiWnd* wnd = GetWnd();
		if (wnd && !wnd->IsFocus())
			return FALSE;
		if (!m_Focus) return FALSE;
		IGuiCtl* foc = m_Focus;
		if (foc == this)
			return IsEffect(this);
		for(list_t::iterator_t ite = GetComp().Head(); ite != GetComp().Tail(); ++ite)
		{
			IGuiCtl* ctl = ExDynCast<IGuiCtl>(*ite);
			if (!ctl) continue;
			if (ctl->IsFocus()) return TRUE;
		}
		return FALSE;
	}
};

//////////////////////////////////////////////////////////////////

// GUI �ؼ�����ӿ�
EXP_INTERFACE IGuiCtrlBase : public IGuiCtl
{
	EXP_DECLARE_DYNAMIC_MULT(IGuiCtrlBase, IGuiCtl)

protected:
	BOOL m_bEnable;		// �Ƿ����
	BOOL m_bVisible;	// �Ƿ�ɼ�

	CRect m_Rect;		// �ؼ�����

	BOOL m_Updated;

	// ��������
	CSize m_AllRect, m_FraRect;

public:
	IGuiCtrlBase();

public:
	// ����״̬
	BOOL Execute(const CString& key, const CString& val);
	void* GetState(const CString& sType, void* pParam = NULL);
	BOOL SetState(const CString& sType, void* pState, void* pParam = NULL);
	void UpdateState(BOOL bRefreshSelf = TRUE);
	BOOL IsUpdated();

	// �������
	BOOL GetRect(CRect& rc) const;
	BOOL SetRect(const CRect& rc);

	BOOL P2C(CRect& rc) const;
	BOOL C2P(CRect& rc) const;
	BOOL B2C(CRect& rc) const;
	BOOL C2B(CRect& rc) const;
	BOOL SetWindowRect(const CRect& rc);
	BOOL GetWindowRect(CRect& rc) const;
	BOOL SetRealRect(const CRect& rc);
	BOOL GetRealRect(CRect& rc) const;
	BOOL GetClientRect(CRect& rc) const;

	virtual BOOL SetAllRect(const CSize& sz);
	BOOL GetAllRect(CSize& sz) const;
	virtual BOOL SetFraRect(const CSize& sz);
	BOOL GetFraRect(CSize& sz) const;

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