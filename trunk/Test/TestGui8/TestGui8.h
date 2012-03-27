#pragma once

//////////////////////////////////////////////////////////////////

class CScrEventV : public IGuiEvent
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
					rc_wnd.Left(rc_wnd.Right() - 10);
					ctl->SetWindowRect(rc_wnd);
				}
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CScrEventH : public IGuiEvent
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
					rc_wnd.Right(rc_wnd.Right() - 10);
					rc_wnd.Top(rc_wnd.Bottom() - 10);
					ctl->SetWindowRect(rc_wnd);
				}
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CTxtEvent : public IGuiEvent
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