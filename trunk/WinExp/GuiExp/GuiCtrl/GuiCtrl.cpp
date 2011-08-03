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
// Date:	2011-08-03
// Version:	1.0.0003.1752
//
// History:
//	- 1.0.0001.1010(2011-06-17)	= 将IGuiCtrlBase接口定义移动到GuiCtrl.cpp中
//	- 1.0.0002.1528(2011-07-01)	+ IGuiCtrlBase::SetWindowRect()将在改变控件大小时发送WM_SIZE消息
//								+ IGuiCtrlBase::SetVisible()将在改变控件可视状态时发送WM_SHOWWINDOW消息
//	- 1.0.0003.1752(2011-08-03)	= 当没有父窗口时,IGuiCtrlBase::SetWindowRect()不应当向自身发送消息
//////////////////////////////////////////////////////////////////

#include "GuiCommon/GuiCommon.h"
#include "GuiCtrl.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI 控件对象接口
EXP_IMPLEMENT_DYNAMIC_MULT(IGuiCtrl, IGuiBase)
IGuiCtrl* IGuiCtrl::m_Focus = NULL;

//////////////////////////////////////////////////////////////////

// GUI 控件对象接口
EXP_IMPLEMENT_DYNAMIC_MULT(IGuiCtrlBase, IGuiCtrl)

IGuiCtrlBase::IGuiCtrlBase()
	: m_bEnable(true)
	, m_bVisible(true)
	, m_Updated(true)
{}

// 更新状态
IGuiCtrlBase::state_t* IGuiCtrlBase::GetState(const CString& sType, CGC* pGC/* = NULL*/)
{
	state_t* state = pGC ? ExMem::Alloc<state_t>(pGC) : state_t::Alloc();
	ExAssert(state);
	if (state) state->sta_typ = sType;
	return state;
}
bool IGuiCtrlBase::SetState(const CString& sType, void* pState)
{
	UpdateState();
	return true;
}
void IGuiCtrlBase::UpdateState(bool bRefreshSelf/* = true*/)
{
	m_Updated = true;
	Refresh(bRefreshSelf);
}
bool IGuiCtrlBase::IsUpdated()
{
	bool updt = m_Updated;
	m_Updated = false; // 外部一旦获知当前状态,则更新状态自动复位
	return updt;
}

// 区域控制
bool IGuiCtrlBase::P2C(CRect& rc)
{
	rc.Offset(-(m_Rect.pt1));
	return true;
}
bool IGuiCtrlBase::C2P(CRect& rc)
{
	rc.Offset(m_Rect.pt1);
	return true;
}
bool IGuiCtrlBase::B2C(CRect& rc)
{
	if (!m_Pare) return false;
	if (!P2C(rc)) return false;
	IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(m_Pare);
	if (ctrl) return ctrl->B2C(rc);
	return ExDynCast<IGuiBoard>(m_Pare) ? true : false;
}
bool IGuiCtrlBase::C2B(CRect& rc)
{
	if (!m_Pare) return false;
	if (!C2P(rc)) return false;
	IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(m_Pare);
	if (ctrl) return ctrl->C2B(rc);
	return ExDynCast<IGuiBoard>(m_Pare) ? true : false;
}
bool IGuiCtrlBase::SetWindowRect(const CRect& rc)
{
	if (m_Rect == rc) return true;
	m_Rect = rc;
	if (GetParent())
	{
		Send(ExDynCast<IGuiObject>(this), WM_SIZE, SIZE_RESTORED, 
			(LPARAM)ExMakeLong(m_Rect.Width(), m_Rect.Height()));
		Refresh(false);
	}
	return true;
}
bool IGuiCtrlBase::GetWindowRect(CRect& rc)
{
	rc = m_Rect;
	return true;
}
bool IGuiCtrlBase::SetRealRect(const CRect& rc)
{
	CRect rc_tmp(rc);
	if (!B2C(rc_tmp)) return false;
	return SetWindowRect(rc_tmp);
}
bool IGuiCtrlBase::GetRealRect(CRect& rc)
{
	if (!m_Pare) return false;
	if (!GetWindowRect(rc)) return false;
	IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(m_Pare);
	if (ctrl) return ctrl->C2B(rc);
	return ExDynCast<IGuiBoard>(m_Pare) ? true : false;
}
bool IGuiCtrlBase::GetClientRect(CRect& rc)
{
	if (!GetWindowRect(rc)) return false;
	rc.MoveTo(CPoint());
	return true;
}

// 刷新绘图
void IGuiCtrlBase::Refresh(bool bSelf/* = true*/)
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
bool IGuiCtrlBase::SetEnable(bool bEnable/* = true*/)
{
	if (m_bEnable == bEnable) return m_bEnable;
	bool old = m_bEnable;
	m_bEnable = bEnable;
	UpdateState();
	return old;
}
bool IGuiCtrlBase::IsEnabled() const
{
	return m_bEnable;
}

// 设置可见性
bool IGuiCtrlBase::SetVisible(bool bVisible/* = true*/)
{
	if (m_bVisible == bVisible) return m_bVisible;
	bool old = m_bVisible;
	m_bVisible = bVisible;
	Send(ExDynCast<IGuiObject>(this), WM_SHOWWINDOW, m_bVisible);
	UpdateState();
	return old;
}
bool IGuiCtrlBase::IsVisible() const
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
