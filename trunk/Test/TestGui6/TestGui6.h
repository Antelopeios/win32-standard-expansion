#pragma once

#include "resource.h"

//////////////////////////////////////////////////////////////////

class CPicEvent : public IGuiEvent
{
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
				CRect rc_wnd;
				IGuiWnd* wnd = ctl->GetWnd();
				if (wnd)
				{
					wnd->GetClientRect(rc_wnd);
					ctl->SetWindowRect(rc_wnd);
				}
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CBarEvent : public IGuiEvent
{
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
				IGuiWnd* wnd = ctl->GetWnd();
				if (wnd)
				{
					CRect rc_wnd;
					wnd->GetClientRect(rc_wnd);
					rc_wnd.Bottom(22);
					ctl->SetWindowRect(rc_wnd);
				}
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CBtnEvent : public IGuiEvent
{
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
				IGuiWnd* wnd = ctl->GetWnd();
				if (wnd)
				{
					CSize sz_txt;
					CText* txt = ((CText**)ctl->GetState(_T("font")))[0];
					CString* str = (CString*)ctl->GetState(_T("text"));
					txt->GetSize(*str, sz_txt);

					CRect rc_wnd;
					wnd->GetClientRect(rc_wnd);
					rc_wnd.Right(sz_txt.cx + 20);
					rc_wnd.Bottom(22);
					ctl->SetWindowRect(rc_wnd);
				}
			}
			break;
		case BM_CLICK:
			{
				CRect rc_ctl;
				ctl->GetWindowRect(rc_ctl);
				IGuiWnd* wnd = ctl->GetWnd();
				wnd->ClientToScreen(rc_ctl);

				ExGet<IGuiWnd>(_T("menu"))->MoveWindow(rc_ctl.LeftBottom());
				ExGet<IGuiWnd>(_T("menu"))->ShowWindow((int)&rc_ctl);
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CWndEvent : public IGuiEvent
{
public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiWnd* wnd = ExDynCast<IGuiWnd>(pGui);
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