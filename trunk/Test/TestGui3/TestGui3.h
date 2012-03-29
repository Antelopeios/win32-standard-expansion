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

class CCustomEvent : public IGuiEvent
{
public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiWnd* wnd = ExDynCast<IGuiWnd>(pGui);
		if (!wnd) return;

		switch( nMessage )
		{
		case WM_KEYDOWN:
			{
				if (wParam == VK_ESCAPE) // ESC
					wnd->PostMessage(WM_CLOSE);
			}
			break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;
		}
	}
};
