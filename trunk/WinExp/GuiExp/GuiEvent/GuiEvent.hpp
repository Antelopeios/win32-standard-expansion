// Copyright 2011-2012, 木头云
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
// GuiEvent - 事件对象
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-28
// Version:	1.0.0004.1752
//
// History:
//	- 1.0.0001.1750(2012-02-21)	+ 添加CQuitEvent事件对象
//	- 1.0.0002.1630(2012-03-02)	+ 添加CMoveEvent事件对象
//	- 1.0.0003.1647(2012-03-04)	+ 添加CCloseEvent,CMinEvent,CMaxEvent,CRestoreEvent事件对象
//	- 1.0.0004.1752(2012-03-28)	+ 添加CSizeEvent,CScrollEvent事件对象
//////////////////////////////////////////////////////////////////

#ifndef __GuiEvent_hpp__
#define __GuiEvent_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////

#include "GuiEvent/GuiWndEvent.hpp"
#include "GuiEvent/GuiButtonEvent.hpp"
#include "GuiEvent/GuiEditEvent.hpp"
#include "GuiEvent/GuiListEvent.hpp"
#include "GuiEvent/GuiLCEvent.hpp"
#include "GuiEvent/GuiLVEvent.hpp"
#include "GuiEvent/GuiScrollEvent.hpp"
#include "GuiEvent/GuiMenuEvent.hpp"
#include "GuiEvent/GuiProgressEvent.hpp"
#include "GuiEvent/GuiTreeEvent.hpp"

//////////////////////////////////////////////////////////////////

EXP_BEG

//////////////////////////////////////////////////////////////////
// CQuitEvent - 当窗口关闭时将会自动退出程序

class CQuitEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CQuitEvent, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		switch( nMessage )
		{
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CQuitEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////
// CMoveEvent - 将窗口或控件区域变成标题栏

class CMoveEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CMoveEvent, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		switch( nMessage )
		{
		case WM_NCHITTEST:
			{
				LRESULT hit_test = GetResult();
				SetResult((hit_test == HTCLIENT) ? HTCAPTION : hit_test);
				SetState(return_next);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CMoveEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////
// CCloseEvent - 按钮点击关闭事件

class CCloseEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CCloseEvent, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		switch( nMessage )
		{
		case BM_CLICK:
			{
				IGuiWnd* wnd = ctl->GetWnd();
				if (!wnd) break;
				wnd->PostMessage(WM_CLOSE);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CCloseEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////
// CMinEvent - 按钮点击最小化事件

class CMinEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CMinEvent, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		switch( nMessage )
		{
		case BM_CLICK:
			{
				IGuiWnd* wnd = ctl->GetWnd();
				if (!wnd) break;
				wnd->ShowWindow(SW_SHOWMINIMIZED);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CMinEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////
// CMaxEvent - 按钮点击最大化事件

class CMaxEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CMaxEvent, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		switch( nMessage )
		{
		case BM_CLICK:
			{
				IGuiWnd* wnd = ctl->GetWnd();
				if (!wnd) break;
				if (!::IsZoomed(wnd->GethWnd()))
					wnd->ShowWindow(SW_SHOWMAXIMIZED);
				CRect rc;
				wnd->GetWindowRect(rc);
				IGuiWnd::GetDesktopRect(rc, rc.LeftTop(), TRUE);
				wnd->MoveWindow(rc);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CMaxEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////
// CRestoreEvent - 按钮点击还原事件

class CRestoreEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CRestoreEvent, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		switch( nMessage )
		{
		case BM_CLICK:
			{
				IGuiWnd* wnd = ctl->GetWnd();
				if (!wnd) break;
				if (::IsZoomed(wnd->GethWnd()))
					wnd->ShowWindow(SW_SHOWNORMAL);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CRestoreEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////
// CSizeEvent - 将大小改变消息作为WM_SHOWWINDOW转发给所有子控件

class CSizeEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CSizeEvent, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiBase* wnd = ExDynCast<IGuiBase>(pGui);
		if (!wnd) return;

		switch( nMessage )
		{
		case WM_SIZE:
			for(IGuiBase::list_t::iterator_t ite = wnd->GetComp().Head(); ite != wnd->GetComp().Tail(); ++ite)
			{
				IGuiCtl* ctl = ExDynCast<IGuiCtl>(*ite);
				if (!ctl) continue;
				ctl->SendMessage(WM_SHOWWINDOW, TRUE);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CSizeEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////
// CScrollEvent - 让控件可以自动根据滚动条的显隐调整大小

class CScrollEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CScrollEvent, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (wParam)
			{
				IGuiBase* wnd = ctl->GetParent();
				if (!wnd) break;
				CRect rc_wnd;
				wnd->GetClientRect(rc_wnd);
				ctl->SetWindowRect(rc_wnd);
				BOOL nd_sc = FALSE;
				if (ctl->IsNeedScroll(TRUE))
				{
					CRect rc_scr;
					ctl->GetScroll(TRUE)->GetWindowRect(rc_scr);
					rc_wnd.Right(rc_wnd.Right() - rc_scr.Width());
					nd_sc = TRUE;
				}
				if (ctl->IsNeedScroll(FALSE))
				{
					CRect rc_scr;
					ctl->GetScroll(FALSE)->GetWindowRect(rc_scr);
					rc_wnd.Bottom(rc_wnd.Bottom() - rc_scr.Height());
					nd_sc = TRUE;
				}
				if (nd_sc)
					ctl->SetWindowRect(rc_wnd);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CScrollEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiEvent_hpp__*/
