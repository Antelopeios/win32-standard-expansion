#pragma once

#include "resource.h"

//////////////////////////////////////////////////////////////////

class CScrEvent : public IGuiEvent
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
					rc_wnd.Left(rc_wnd.Right() - 10);
					ctl->SetWindowRect(rc_wnd);
				}
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CLstEvent : public IGuiEvent
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
					if (ctl->IsNeedScroll())
					{
						CRect rc_scr;
						ctl->GetScroll()->GetWindowRect(rc_scr);
						rc_wnd.Right(rc_wnd.Right() - rc_scr.Width());
					}
					ctl->SetWindowRect(rc_wnd);
				}
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