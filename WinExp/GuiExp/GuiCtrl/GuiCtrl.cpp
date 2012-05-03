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
// Date:	2012-04-26
// Version:	1.0.0028.1346
//////////////////////////////////////////////////////////////////

#include "GuiCommon/GuiCommon.h"
#include "GuiEffect/GuiEffect.hpp"
#include "GuiEvent/GuiEvent.hpp"
#include "GuiCtrl.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI 控件对象接口
EXP_IMPLEMENT_DYNAMIC_MULT(IGuiCtl, IGuiBase)
IGuiCtl* IGuiCtl::m_Focus = NULL;

//////////////////////////////////////////////////////////////////

IGuiCtl::IGuiCtl()
	: m_Effect(NULL)
	, m_bTruEff(TRUE)
	, m_bEnable(TRUE)
	, m_bVisible(TRUE)
	, m_bNoFocus(FALSE)
	, m_Updated(TRUE)
{
	m_Scroll[1] = m_Scroll[0] = NULL;
}

BOOL IGuiCtl::IsValid() const
{
	return EXP_BASE::IsValid();
}

void IGuiCtl::Init(IGuiComp* pComp)
{
	EXP_BASE::Init(pComp);
	IGuiWnd* wnd = GetWnd();
	if (!wnd) return;
	if (IsVisible() && wnd->IsVisible())
	{
		SendMessage(WM_SHOWWINDOW, 1);
		SetFocus();
	}
	else
		SendMessage(WM_SHOWWINDOW, 0);
}

void IGuiCtl::Fina()
{
	if (m_bTruEff && m_Effect)
		del(m_Effect);
	m_Focus = NULL;
	EXP_BASE::Fina();
}

void IGuiCtl::SetTrust(BOOL bTruCldr)
{
	IGuiBase::SetTrust(bTruCldr);
}

BOOL IGuiCtl::IsTrust() const
{
	return IGuiBase::IsTrust();
}

void IGuiCtl::SendSet(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	for(set_list_t::iterator_t ite = GetSet().Head(); ite != GetSet().Tail(); ++ite)
	{
		IGuiSet* set = (IGuiSet*)(*ite);
		ExAssert(set);
		set->Msg(nMessage, wParam, lParam);
	}
}

void IGuiCtl::SendMsg(void* pGui, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	IGuiSender::Send(pGui, nMessage, wParam, lParam);
}

void IGuiCtl::Send(void* pGui, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if(!GetParent()) return;
	SendSet(nMessage, wParam, lParam);
	SendMsg(pGui, nMessage, wParam, lParam);
}

void IGuiCtl::SendMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	Send(this, nMessage, wParam, lParam);
}

// 控件设置对象管理
BOOL IGuiCtl::AddSet(void* p)
{
	IGuiSet* set = (IGuiSet*)(p);
	if (!set) return FALSE;
	set->Ctl() = this;
	return IGuiSetMgr::AddSet(p);
}
BOOL IGuiCtl::AddSet(LPCTSTR key)
{
	if (!IGuiSetMgr::AddSet(key)) return FALSE;
	IGuiSet* set = (IGuiSet*)(GetSet().LastItem());
	if (!set)
	{
		DelSet(key);
		return FALSE;
	}
	set->Ctl() = this;
	return TRUE;
}
BOOL IGuiCtl::InsSet(void* p)
{
	IGuiSet* set = (IGuiSet*)(p);
	if (!set) return FALSE;
	set->Ctl() = this;
	return IGuiSetMgr::InsSet(p);
}
BOOL IGuiCtl::InsSet(LPCTSTR key)
{
	if (!IGuiSetMgr::InsSet(key)) return FALSE;
	IGuiSet* set = (IGuiSet*)(GetSet().HeadItem());
	if (!set)
	{
		DelSet(key);
		return FALSE;
	}
	set->Ctl() = this;
	return TRUE;
}

// 更新状态
BOOL IGuiCtl::Execute(const CString& key, const CString& val)
{
	IGuiSet* set = FindKeySet(key);
	if (set) return set->Exc(val);
	return TRUE;
}
void* IGuiCtl::Execute(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent)
{
	// 设置区域
	SetRect(ExStringToRect(xml.GetAttr(_T("rect"), ite)));
	// 设置可见性
	CString t = xml.GetAttr(_T("visible"), ite); t.Lower();
	if (t == _T("false"))
		SetVisible(FALSE);
	else
	if (t == _T("true"))
		SetVisible(TRUE);
	// 设置无焦点
	t = xml.GetAttr(_T("no_focus"), ite); t.Lower();
	if (t == _T("false"))
		SetNoFocus(FALSE);
	else
	if (t == _T("true"))
		SetNoFocus(TRUE);
	// 设置消息穿透
	t = xml.GetAttr(_T("through"), ite); t.Lower();
	if (t == _T("false"))
		SetThrough(FALSE);
	else
	if (t == _T("true"))
		SetThrough(TRUE);
	return IGuiBase::Execute(xml, ite, parent);
}
void* IGuiCtl::GetState(const CString& sType, void* pParam/* = NULL*/)
{
	IGuiSet* set = FindKeySet(sType);
	if (set) return set->Get(pParam);
	return NULL;
}
BOOL IGuiCtl::SetState(const CString& sType, void* pState, void* pParam/* = NULL*/)
{
	IGuiSet* set = FindKeySet(sType);
	if (set) set->Set(pState, pParam);
	UpdateState();
	return TRUE;
}
void IGuiCtl::UpdateState(BOOL bRefreshSelf/* = TRUE*/)
{
	m_Updated = TRUE;
	Refresh(bRefreshSelf);
}
BOOL IGuiCtl::IsUpdated()
{
	BOOL updt = m_Updated;
	m_Updated = FALSE; // 外部一旦获知当前状态,则更新状态自动复位
	return updt;
}

// 设置效果对象
void IGuiCtl::SetEffectTrust(BOOL bTru)
{
	m_bTruEff = bTru;
}
void IGuiCtl::SetEffect(void* p)
{
	if (m_bTruEff && m_Effect)
		del(m_Effect);
	m_Effect = ExDynCast<IGuiEffect>(p);
	Refresh(FALSE);
}
IGuiEffect* IGuiCtl::GetEffect()
{
	return m_Effect;
}

// 获得绘图板
IGuiBase* IGuiCtl::GetParent() const
{
	return ExDynCast<IGuiBase>(m_Pare);
}
IGuiWnd* IGuiCtl::GetWnd() const
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
wnd_t IGuiCtl::GethWnd() const
{
	IGuiWnd* wnd = GetWnd();
	return wnd ? wnd->GethWnd() : NULL;
}

// 区域控制
BOOL IGuiCtl::GetRect(CRect& rc) const
{
	rc = m_Rect;
	return TRUE;
}
BOOL IGuiCtl::SetRect(const CRect& rc)
{
	if (m_Rect == rc) return TRUE;
	m_Rect = rc;
	SendMessage(WM_SIZE, SIZE_RESTORED, 
		(LPARAM)ExMakeLong(m_Rect.Width(), m_Rect.Height()));
	return TRUE;
}
BOOL IGuiCtl::P2C(CRect& rc) const
{
	CRect rc_wnd;
	if (!GetWindowRect(rc_wnd)) return FALSE;
	rc.Offset(-(rc_wnd.pt1));
	return TRUE;
}
BOOL IGuiCtl::C2P(CRect& rc) const
{
	CRect rc_wnd;
	if (!GetWindowRect(rc_wnd)) return FALSE;
	rc.Offset(rc_wnd.pt1);
	return TRUE;
}
BOOL IGuiCtl::B2C(CRect& rc) const
{
	if (!m_Pare) return FALSE;
	if (!P2C(rc)) return FALSE;
	IGuiCtl* ctl = ExDynCast<IGuiCtl>(m_Pare);
	if (ctl) return ctl->B2C(rc);
	return ExDynCast<IGuiWnd>(m_Pare) ? TRUE : FALSE;
}
BOOL IGuiCtl::C2B(CRect& rc) const
{
	if (!m_Pare) return FALSE;
	if (!C2P(rc)) return FALSE;
	IGuiCtl* ctl = ExDynCast<IGuiCtl>(m_Pare);
	if (ctl) return ctl->C2B(rc);
	return ExDynCast<IGuiWnd>(m_Pare) ? TRUE : FALSE;
}
BOOL IGuiCtl::SetWindowRect(const CRect& rc)
{
	IGuiBase* pare = GetParent();
	if (!pare) return SetRect(rc);
	IGuiCtl* ctl = ExDynCast<IGuiCtl>(pare);
	if (ctl)
	{
		CSize scr_sz;
		if (!ctl->GetScrollSize(scr_sz)) return FALSE;
		CRect rect(rc);
		rect.Offset(CPoint(scr_sz.cx, scr_sz.cy));
		if (!SetRect(rect)) return FALSE;
	}
	else
	{
		if (!SetRect(rc)) return FALSE;
	}
	SendMessage(WM_SIZE, SIZE_RESTORED, 
		(LPARAM)ExMakeLong(m_Rect.Width(), m_Rect.Height()));
	Refresh(FALSE);
	return TRUE;
}
BOOL IGuiCtl::GetWindowRect(CRect& rc) const
{
	IGuiBase* pare = GetParent();
	if (!pare) return GetRect(rc);
	IGuiCtl* ctl = ExDynCast<IGuiCtl>(pare);
	if (ctl)
	{
		if (!GetRect(rc)) return FALSE;
		CSize scr_sz;
		if (!ctl->GetScrollSize(scr_sz)) return FALSE;
		rc.Offset(-CPoint(scr_sz.cx, scr_sz.cy));
		return TRUE;
	}
	else
		return GetRect(rc);
}
BOOL IGuiCtl::SetRealRect(const CRect& rc)
{
	CRect rc_tmp(rc);
	if (!B2C(rc_tmp)) return FALSE;
	return SetWindowRect(rc_tmp);
}
BOOL IGuiCtl::GetRealRect(CRect& rc) const
{
	if (!m_Pare) return FALSE;
	if (!GetWindowRect(rc)) return FALSE;
	IGuiCtl* ctl = ExDynCast<IGuiCtl>(m_Pare);
	if (ctl) return ctl->C2B(rc);
	return ExDynCast<IGuiWnd>(m_Pare) ? TRUE : FALSE;
}
BOOL IGuiCtl::GetClientRect(CRect& rc) const
{
	if (!GetRect(rc)) return FALSE;
	rc.MoveTo(CPoint());
	return TRUE;
}

BOOL IGuiCtl::SetAllRect(const CSize& sz)
{
	if (m_AllRect == sz) return TRUE;
	m_AllRect = sz;
	if (GetParent())
	{
		if (GetScroll(TRUE))
			GetScroll(TRUE)->SetState(_T("sli_all"), (void*)m_AllRect.cy);
		if (GetScroll(FALSE))
			GetScroll(FALSE)->SetState(_T("sli_all"), (void*)m_AllRect.cx);
		Refresh();
	}
	return TRUE;
}
BOOL IGuiCtl::GetAllRect(CSize& sz) const
{
	sz = m_AllRect;
	return TRUE;
}
BOOL IGuiCtl::SetFraRect(const CSize& sz)
{
	if (m_FraRect == sz) return TRUE;
	m_FraRect = sz;
	if (GetParent())
	{
		if (GetScroll(TRUE))
			GetScroll(TRUE)->SetState(_T("sli_fra"), (void*)m_FraRect.cy);
		if (GetScroll(FALSE))
			GetScroll(FALSE)->SetState(_T("sli_fra"), (void*)m_FraRect.cx);
		Refresh();
	}
	return TRUE;
}
BOOL IGuiCtl::GetFraRect(CSize& sz) const
{
	sz = m_FraRect;
	return TRUE;
}

BOOL IGuiCtl::GetScrollSize(CSize& sz) const
{
	sz = m_szScroll;
	return TRUE;
}
BOOL IGuiCtl::SetScrollSize(const CSize& sz, BOOL bWheel)
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
IGuiCtl* IGuiCtl::GetScroll(BOOL bLine) const
{
	return m_Scroll[bLine ? 0 : 1];
}
void IGuiCtl::SetScroll(void* p, BOOL bLine)
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
BOOL IGuiCtl::IsNeedScroll(BOOL bLine)
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

// 判断是否可视(如控件在父控件窗口外)
BOOL IGuiCtl::IsDisplayed() const
{
	CRect rc_slf;
	if (!GetWindowRect(rc_slf)) return FALSE;
	CRect rc_prt;
	if (!GetParent()->GetClientRect(rc_prt)) return FALSE;
	return (!rc_slf.Inter(rc_prt).IsEmpty());
}
BOOL IGuiCtl::GetDisplaySize(CSize& sz) const
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

// 刷新绘图
void IGuiCtl::Refresh(BOOL bSelf/* = TRUE*/)
{
	IGuiWnd* wnd = GetWnd();
	if (!wnd) return;
	if (bSelf)
	{
		CRect rc;
		GetRealRect(rc);
		wnd->InvalidateRect(rc);
	}
	else
		wnd->Invalidate();
}

// 设置可用性
BOOL IGuiCtl::SetEnable(BOOL bEnable/* = TRUE*/)
{
	if (m_bEnable == bEnable) return m_bEnable;
	BOOL old = m_bEnable;
	m_bEnable = bEnable;
	UpdateState();
	return old;
}
BOOL IGuiCtl::IsEnabled() const
{
	return m_bEnable;
}

// 设置可见性
BOOL IGuiCtl::SetVisible(BOOL bVisible/* = TRUE*/)
{
	if (m_bVisible == bVisible) return m_bVisible;
	BOOL old = m_bVisible;
	m_bVisible = bVisible;
	if (GetParent())
	{
		SendMessage(WM_SHOWWINDOW, m_bVisible);
		UpdateState();
	}
	return old;
}
BOOL IGuiCtl::IsVisible() const
{
	return m_bVisible;
}

// 设置无焦点
BOOL IGuiCtl::SetNoFocus(BOOL bNoFocus/* = TRUE*/)
{
	if (m_bNoFocus == bNoFocus) return m_bNoFocus;
	BOOL old = m_bNoFocus;
	m_bNoFocus = bNoFocus;
	if (m_bNoFocus && m_Focus == this) SetFocus();
	UpdateState();
	return old;
}
BOOL IGuiCtl::IsNoFocus() const
{
	return m_bNoFocus;
}

// 设置消息穿透
BOOL IGuiCtl::SetThrough(BOOL bThrough/* = TRUE*/)
{
	BOOL old = IGuiBase::SetThrough(bThrough);
	if (IsThrough()) SetNoFocus(TRUE);
	return old;
}

// 判断有效性
BOOL IGuiCtl::IsEffect(const IGuiCtl* pCtrl)
{
	return (pCtrl && pCtrl->IsEnabled() && pCtrl->IsVisible());
}

// 焦点控制
IGuiCtl* IGuiCtl::SetFocus(IGuiCtl* pFoc)
{
	if (pFoc && !IsEffect(pFoc)) return NULL;
	if (pFoc && !pFoc->IsVisible()) return NULL;
	if (pFoc)
	{
		IGuiWnd* wnd = pFoc->GetWnd();
		if (!wnd/* || !wnd->IsVisible()*/) return NULL;
		if (pFoc->IsNoFocus())
		{
			IGuiCtl* next = pFoc->GetNext(TRUE, TRUE);
			if (next == pFoc)
			{
				IGuiCtl* pre = ExDynCast<IGuiCtl>(pFoc->GetParent());
				if (pre)
					return pre->SetFocus();
				else
					return NULL;
			}
			else
				return next->SetFocus();
		}
	}
	// 设置控件焦点
	IGuiCtl* old_fc = m_Focus;
	m_Focus = pFoc;
	if (old_fc == m_Focus) return NULL;
	// 设置窗口焦点
	if (m_Focus)
	{
		IGuiWnd* wnd = m_Focus->GetWnd();
		if (wnd) wnd->SetFocus();
	}
	// 发送焦点改变消息
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
IGuiCtl* IGuiCtl::SetFocus()
{
	return SetFocus(this);
}
IGuiCtl* IGuiCtl::GetFocus()
{
	return m_Focus;
}
BOOL IGuiCtl::IsFocus() const
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

// 控件枚举
IGuiCtl* IGuiCtl::GetHead()
{
	IGuiComp* comp = GetParent();
	return ExDynCast<IGuiCtl>(comp->GetComp().HeadItem());
}
IGuiCtl* IGuiCtl::GetLast()
{
	IGuiComp* comp = GetParent();
	return ExDynCast<IGuiCtl>(comp->GetComp().LastItem());
}
IGuiCtl* IGuiCtl::GetNext(BOOL bCheckThrough/* = TRUE*/, BOOL bCheckNoFocus/* = FALSE*/)
{
	IGuiComp* comp = GetParent();
	IGuiComp::list_t::iterator_t ite = comp->FindComp(ExDynCast<IGuiComp>(this));
	ExAssert(ite != comp->GetComp().Tail());
	IGuiCtl* next = NULL;
	do
	{
		if (ite == comp->GetComp().Last())
			ite = comp->GetComp().Head();
		else
			++ite;
		next = ExDynCast<IGuiCtl>(*ite);
	} while (next != this && (!IGuiCtl::IsEffect(next) || 
			(bCheckThrough && next->IsThrough()) || 
			(bCheckNoFocus && next->IsNoFocus()) ));
	ExAssert(next);
	return next;
}
IGuiCtl* IGuiCtl::GetPrev(BOOL bCheckThrough/* = TRUE*/, BOOL bCheckNoFocus/* = FALSE*/)
{
	IGuiComp* comp = GetParent();
	IGuiComp::list_t::iterator_t ite = comp->FindComp(ExDynCast<IGuiComp>(this));
	ExAssert(ite != comp->GetComp().Tail());
	IGuiCtl* next = NULL;
	do
	{
		if (ite == comp->GetComp().Head())
			ite = comp->GetComp().Last();
		else
			--ite;
		next = ExDynCast<IGuiCtl>(*ite);
	} while (next != this && (!IGuiCtl::IsEffect(next) || 
			(bCheckThrough && next->IsThrough()) || 
			(bCheckNoFocus && next->IsNoFocus()) ));
	ExAssert(next);
	return next;
}

//////////////////////////////////////////////////////////////////

EXP_END
	
//////////////////////////////////////////////////////////////////
// 属性设置模板

template <typename TypeT>
class ICtrlSetT : public IGuiSet
{
protected:
	TypeT m_Val;

	void ZeroVal(_true_type) { m_Val = 0; }
	void ZeroVal(_false_type) {}

public:
	ICtrlSetT()
	{ ZeroVal(_TraitsT<TypeT>::is_POD_type()); }

public:
	BOOL Exc(const CString& val)
	{
		Set((void*)_ttol(val));
		Ctl()->UpdateState();
		return TRUE;
	}
	void* Get(void* par = NULL)
	{
		return (void*)m_Val;
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		m_Val = (TypeT)(LONG)sta;
		return TRUE;
	}
};

template <>
class ICtrlSetT<BOOL> : public IGuiSet
{
protected:
	BOOL m_Val;

public:
	ICtrlSetT()
		: m_Val(FALSE)
	{}

public:
	BOOL Exc(const CString& val)
	{
		CString tmp(val);
		tmp.Lower();
		if (tmp == _T("false"))
			Set((void*)FALSE);
		else
		if (tmp == _T("true"))
			Set((void*)TRUE);
		else
			Set((void*)_ttol(val));
		Ctl()->UpdateState();
		return TRUE;
	}
	void* Get(void* par = NULL)
	{
		return (void*)m_Val;
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		m_Val = (BOOL)sta;
		return TRUE;
	}
};

template <>
class ICtrlSetT<CString> : public IGuiSet
{
protected:
	CString m_Val;

public:
	BOOL Exc(const CString& val)
	{
		Set((void*)&val);
		Ctl()->UpdateState();
		return TRUE;
	}
	void* Get(void* par = NULL)
	{
		return (void*)(&m_Val);
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		m_Val = *(CString*)sta;
		return TRUE;
	}
};

template <typename TypeT>
class IIterSetT : public IGuiSet
{
public:
	typedef TypeT items_t;
	typedef typename items_t::iterator_t iter_t;

protected:
	iter_t m_Val;

public:
	CString GetKey() const { return _T("iter"); }
	void* Get(void* par = NULL)
	{
		return (void*)(&m_Val);
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		m_Val = *(iter_t*)sta;
		return TRUE;
	}
};

template <typename TypeT>
class IItemSetT : public IGuiSet
{
public:
	typedef TypeT items_t;
	typedef typename items_t::iterator_t iter_t;

protected:
	CString m_Key;
	items_t m_ItemList;

public:
	CString GetKey() const { return _T("items"); }
	BOOL Key(const CString& key)
	{
		if (key == _T("items") || 
			key == _T("insert") || 
			key == _T("delete") || 
			key == _T("clear"))
		{
			m_Key = key;
			return TRUE;
		}
		else
		{
			m_Key = _T("");
			return FALSE;
		}
	}
	void* Get(void* par = NULL)
	{
		if (m_Key == _T("items"))
			return (void*)(&m_ItemList);
		else
			return NULL;
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		if (m_Key == _T("items"))
		{
			items_t* new_sta = (items_t*)sta;
			if (new_sta == NULL) return FALSE;
			for(iter_t ite = m_ItemList.Head(); ite != m_ItemList.Tail(); ++ite)
			{
				IGuiCtl* item = *ite;
				if (!item) continue;
				iter_t it = new_sta->Find(item);
				if (it == new_sta->Tail()) Ctl()->DelComp(item);
			}
			m_ItemList = *new_sta;
			for(iter_t ite = m_ItemList.Head(); ite != m_ItemList.Tail(); ++ite)
			{
				IGuiCtl* item = *ite;
				if (!item) continue;
				Ctl()->AddComp(item);
				item->SetState(_T("iter"), (void*)&ite);
			}
			Ctl()->SendMessage(WM_SIZE);
		}
		else
		if (m_Key == _T("insert"))
		{
			if (!sta) return TRUE;
			IGuiCtl* item = (IGuiCtl*)sta;
			iter_t ite = par ? *(iter_t*)par : m_ItemList.Tail();
			Ctl()->AddComp(item);
			m_ItemList.Add(item, ite);
			if (items_t::SimpleCon == 1) --ite;
			item->SetState(_T("iter"), (void*)&ite);
			Ctl()->SendMessage(WM_SIZE);
		}
		else
		if (m_Key == _T("delete"))
		{
			iter_t ite = *(iter_t*)sta;
			IGuiCtl* item = *ite;
			if (!item) return FALSE;
			Ctl()->DelComp(item);
			m_ItemList.Del(ite);
			Ctl()->SendMessage(WM_SIZE);
		}
		else
		if (m_Key == _T("clear"))
		{
			for(iter_t ite = m_ItemList.Head(); ite != m_ItemList.Tail(); ++ite)
			{
				IGuiCtl* item = *ite;
				if (!item) continue;
				Ctl()->DelComp(item);
			}
			m_ItemList.Clear();
			Ctl()->SendMessage(WM_SIZE);
		}
		return TRUE;
	}
};

//////////////////////////////////////////////////////////////////

#define SET_STATE(type, val, scp) \
	{ \
		if (val != (type)(LONG_PTR)pState) \
		{ \
			val = (type)(LONG_PTR)pState; \
			scp; \
			return EXP_BASE::SetState(sType, pState, pParam); \
		} \
		else \
			return TRUE; \
	}
//#define SET_STATE()

//////////////////////////////////////////////////////////////////

#include "GuiCtrl/GuiPicture.hpp"
#include "GuiCtrl/GuiButton.hpp"
#include "GuiCtrl/GuiEdit.hpp"
#include "GuiCtrl/GuiList.hpp"
#include "GuiCtrl/GuiListCtrl.hpp"
#include "GuiCtrl/GuiListView.hpp"
#include "GuiCtrl/GuiGroup.hpp"
#include "GuiCtrl/GuiScroll.hpp"
#include "GuiCtrl/GuiText.hpp"
#include "GuiCtrl/GuiProgress.hpp"
#include "GuiCtrl/GuiTree.hpp"

//////////////////////////////////////////////////////////////////
