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
// Date:	2011-08-03
// Version:	1.0.0003.1752
//
// History:
//	- 1.0.0001.1010(2011-06-17)	= ��IGuiCtrlBase�ӿڶ����ƶ���GuiCtrl.cpp��
//	- 1.0.0002.1528(2011-07-01)	+ IGuiCtrlBase::SetWindowRect()���ڸı�ؼ���Сʱ����WM_SIZE��Ϣ
//								+ IGuiCtrlBase::SetVisible()���ڸı�ؼ�����״̬ʱ����WM_SHOWWINDOW��Ϣ
//	- 1.0.0003.1752(2011-08-03)	= ��û�и�����ʱ,IGuiCtrlBase::SetWindowRect()��Ӧ������������Ϣ
//////////////////////////////////////////////////////////////////

#include "GuiCommon/GuiCommon.h"
#include "GuiCtrl.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI �ؼ�����ӿ�
EXP_IMPLEMENT_DYNAMIC_MULT(IGuiCtl, IGuiBase)
IGuiCtl* IGuiCtl::m_Focus = NULL;

//////////////////////////////////////////////////////////////////

// GUI �ؼ�����ӿ�
EXP_IMPLEMENT_DYNAMIC_MULT(IGuiCtrlBase, IGuiCtl)

IGuiCtrlBase::IGuiCtrlBase()
	: m_bEnable(TRUE)
	, m_bVisible(TRUE)
	, m_Updated(TRUE)
{}

// ����״̬
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
	m_Updated = FALSE; // �ⲿһ����֪��ǰ״̬,�����״̬�Զ���λ
	return updt;
}

// �������
BOOL IGuiCtrlBase::P2C(CRect& rc)
{
	rc.Offset(-(m_Rect.pt1));
	return TRUE;
}
BOOL IGuiCtrlBase::C2P(CRect& rc)
{
	rc.Offset(m_Rect.pt1);
	return TRUE;
}
BOOL IGuiCtrlBase::B2C(CRect& rc)
{
	if (!m_Pare) return FALSE;
	if (!P2C(rc)) return FALSE;
	IGuiCtl* ctl = ExDynCast<IGuiCtl>(m_Pare);
	if (ctl) return ctl->B2C(rc);
	return ExDynCast<IGuiWnd>(m_Pare) ? TRUE : FALSE;
}
BOOL IGuiCtrlBase::C2B(CRect& rc)
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
	m_Rect = rc;
	if (GetParent())
	{
		SendMessage(WM_SIZE, SIZE_RESTORED, 
			(LPARAM)ExMakeLong(m_Rect.Width(), m_Rect.Height()));
		Refresh(FALSE);
	}
	return TRUE;
}
BOOL IGuiCtrlBase::GetWindowRect(CRect& rc)
{
	rc = m_Rect;
	return TRUE;
}
BOOL IGuiCtrlBase::SetRealRect(const CRect& rc)
{
	CRect rc_tmp(rc);
	if (!B2C(rc_tmp)) return FALSE;
	return SetWindowRect(rc_tmp);
}
BOOL IGuiCtrlBase::GetRealRect(CRect& rc)
{
	if (!m_Pare) return FALSE;
	if (!GetWindowRect(rc)) return FALSE;
	IGuiCtl* ctl = ExDynCast<IGuiCtl>(m_Pare);
	if (ctl) return ctl->C2B(rc);
	return ExDynCast<IGuiWnd>(m_Pare) ? TRUE : FALSE;
}
BOOL IGuiCtrlBase::GetClientRect(CRect& rc)
{
	if (!GetWindowRect(rc)) return FALSE;
	rc.MoveTo(CPoint());
	return TRUE;
}

// ˢ�»�ͼ
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

// ���ÿ�����
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

// ���ÿɼ���
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
#include "GuiCtrl/GuiListView.hpp"
#include "GuiCtrl/GuiGroup.hpp"
#include "GuiCtrl/GuiScroll.hpp"

//////////////////////////////////////////////////////////////////
