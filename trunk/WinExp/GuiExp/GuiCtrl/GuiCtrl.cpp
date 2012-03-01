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
// Date:	2012-02-29
// Version:	1.0.0020.1723
//////////////////////////////////////////////////////////////////

#include "GuiCommon/GuiCommon.h"
#include "GuiCtrl.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI 控件对象接口
EXP_IMPLEMENT_DYNAMIC_MULT(IGuiCtl, IGuiBase)
IGuiCtl* IGuiCtl::m_Focus = NULL;

//////////////////////////////////////////////////////////////////

// GUI 控件对象接口
EXP_IMPLEMENT_DYNAMIC_MULT(IGuiCtrlBase, IGuiCtl)

IGuiCtrlBase::IGuiCtrlBase()
	: m_bEnable(TRUE)
	, m_bVisible(TRUE)
	, m_Updated(TRUE)
{}

// 更新状态
void* IGuiCtrlBase::GetState(const CString& sType)
{
	return NULL;
}
BOOL IGuiCtrlBase::SetState(const CString& sType, void* pState)
{
	UpdateState();
	return TRUE;
}
void IGuiCtrlBase::UpdateState(BOOL bRefreshSelf/* = TRUE*/)
{
	m_Updated = TRUE;
	Refresh(bRefreshSelf);
}
BOOL IGuiCtrlBase::IsUpdated()
{
	BOOL updt = m_Updated;
	m_Updated = FALSE; // 外部一旦获知当前状态,则更新状态自动复位
	return updt;
}

// 区域控制
BOOL IGuiCtrlBase::GetRect(CRect& rc) const
{
	rc = m_Rect;
	return TRUE;
}
BOOL IGuiCtrlBase::SetRect(const CRect& rc)
{
	m_Rect = rc;
	return TRUE;
}
BOOL IGuiCtrlBase::P2C(CRect& rc) const
{
	CRect rc_wnd;
	if (!GetWindowRect(rc_wnd)) return FALSE;
	rc.Offset(-(rc_wnd.pt1));
	return TRUE;
}
BOOL IGuiCtrlBase::C2P(CRect& rc) const
{
	CRect rc_wnd;
	if (!GetWindowRect(rc_wnd)) return FALSE;
	rc.Offset(rc_wnd.pt1);
	return TRUE;
}
BOOL IGuiCtrlBase::B2C(CRect& rc) const
{
	if (!m_Pare) return FALSE;
	if (!P2C(rc)) return FALSE;
	IGuiCtl* ctl = ExDynCast<IGuiCtl>(m_Pare);
	if (ctl) return ctl->B2C(rc);
	return ExDynCast<IGuiWnd>(m_Pare) ? TRUE : FALSE;
}
BOOL IGuiCtrlBase::C2B(CRect& rc) const
{
	if (!m_Pare) return FALSE;
	if (!C2P(rc)) return FALSE;
	IGuiCtl* ctl = ExDynCast<IGuiCtl>(m_Pare);
	if (ctl) return ctl->C2B(rc);
	return ExDynCast<IGuiWnd>(m_Pare) ? TRUE : FALSE;
}
BOOL IGuiCtrlBase::SetWindowRect(const CRect& rc)
{
	if (m_Rect == rc) return TRUE;
	SetRect(rc);
	if (GetParent())
	{
		SendMessage(WM_SIZE, SIZE_RESTORED, 
			(LPARAM)ExMakeLong(m_Rect.Width(), m_Rect.Height()));
		Refresh(FALSE);
	}
	return TRUE;
}
BOOL IGuiCtrlBase::GetWindowRect(CRect& rc) const
{
	IGuiBase* pare = GetParent();
	if (!pare) return FALSE;
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
BOOL IGuiCtrlBase::SetRealRect(const CRect& rc)
{
	CRect rc_tmp(rc);
	if (!B2C(rc_tmp)) return FALSE;
	return SetWindowRect(rc_tmp);
}
BOOL IGuiCtrlBase::GetRealRect(CRect& rc) const
{
	if (!m_Pare) return FALSE;
	if (!GetWindowRect(rc)) return FALSE;
	IGuiCtl* ctl = ExDynCast<IGuiCtl>(m_Pare);
	if (ctl) return ctl->C2B(rc);
	return ExDynCast<IGuiWnd>(m_Pare) ? TRUE : FALSE;
}
BOOL IGuiCtrlBase::GetClientRect(CRect& rc) const
{
	if (!GetWindowRect(rc)) return FALSE;
	rc.MoveTo(CPoint());
	return TRUE;
}

BOOL IGuiCtrlBase::SetAllRect(const CSize& sz)
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
BOOL IGuiCtrlBase::GetAllRect(CSize& sz) const
{
	sz = m_AllRect;
	return TRUE;
}
BOOL IGuiCtrlBase::SetFraRect(const CSize& sz)
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
BOOL IGuiCtrlBase::GetFraRect(CSize& sz) const
{
	sz = m_FraRect;
	return TRUE;
}

// 刷新绘图
void IGuiCtrlBase::Refresh(BOOL bSelf/* = TRUE*/)
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
BOOL IGuiCtrlBase::SetEnable(BOOL bEnable/* = TRUE*/)
{
	if (m_bEnable == bEnable) return m_bEnable;
	BOOL old = m_bEnable;
	m_bEnable = bEnable;
	UpdateState();
	return old;
}
BOOL IGuiCtrlBase::IsEnabled() const
{
	return m_bEnable;
}

// 设置可见性
BOOL IGuiCtrlBase::SetVisible(BOOL bVisible/* = TRUE*/)
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
BOOL IGuiCtrlBase::IsVisible() const
{
	return m_bVisible;
}

//////////////////////////////////////////////////////////////////

EXP_END

#include "GuiCtrl/GuiPicture.hpp"
#include "GuiCtrl/GuiButton.hpp"
#include "GuiCtrl/GuiEdit.hpp"
#include "GuiCtrl/GuiList.hpp"
#include "GuiCtrl/GuiListCtrl.hpp"
#include "GuiCtrl/GuiListView.hpp"
#include "GuiCtrl/GuiGroup.hpp"
#include "GuiCtrl/GuiScroll.hpp"

//////////////////////////////////////////////////////////////////
