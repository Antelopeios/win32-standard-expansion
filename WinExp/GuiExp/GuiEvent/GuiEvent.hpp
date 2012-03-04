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
// GuiEvent - �¼�����
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-04
// Version:	1.0.0003.1647
//
// History:
//	- 1.0.0001.1750(2012-02-21)	+ ���CQuitEvent�¼�����
//	- 1.0.0002.1630(2012-03-02)	+ ���CMoveEvent�¼�����
//	- 1.0.0003.1647(2012-03-04)	+ ���CCloseEvent,CMinEvent,CMaxEvent,CRestoreEvent�¼�����
//////////////////////////////////////////////////////////////////

#ifndef __GuiEvent_hpp__
#define __GuiEvent_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////
// CQuitEvent - �����ڹر�ʱ�����Զ��˳�����

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
// CMoveEvent - �����ڻ�ؼ������ɱ�����

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
// CCloseEvent - ��ť����ر��¼�

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
// CMinEvent - ��ť�����С���¼�

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
// CMaxEvent - ��ť�������¼�

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
// CRestoreEvent - ��ť�����ԭ�¼�

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

EXP_END

#endif/*__GuiEvent_hpp__*/
