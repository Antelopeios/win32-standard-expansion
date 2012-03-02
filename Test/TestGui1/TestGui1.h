#pragma once

#include "resource.h"

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

//////////////////////////////////////////////////////////////////

class CCusPicEvent : public IGuiEvent
{
public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		switch( nMessage )
		{
		case WM_NCHITTEST:
			{
				LRESULT hit_test = GetResult();
				SetResult((hit_test == HTCLIENT) ? HTCAPTION : hit_test);
			}
			break;
		case WM_KEYDOWN:
			{
				if (wParam == VK_RETURN) // Enter
					ctl->SetVisible(!ctl->IsVisible());
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CModWndEvent : public IGuiEvent
{
public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiWnd* wnd = ExDynCast<IGuiWnd>(pGui);
		if (!wnd) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (wParam)
			{
				wnd->CenterWindow();
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CCusBtnEvent : public IGuiEvent
{
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
				if (!wnd) return;
				IGuiCtl* pic = ExDynCast<IGuiCtl>(wnd->GetChildren().HeadItem());
				if (!pic) return;
				pixel_t pix = (pixel_t)pic->GetState(_T("color"));
				if (pix)
					pix = 0;
				else
					pix = ExRGBA(99, 66, 33, 128);
				pic->SetState(_T("color"), (void*)pix);

				IGuiWnd* mod_wnd = ExGui<IGuiWnd>(_T("CGuiWnd"));
				mod_wnd->AddEvent(dbnew(CModWndEvent));
				mod_wnd->Create(_T(""), CRect(0, 0, 100, 100), SW_HIDE, WS_POPUP, NULL, wnd->GethWnd());
				mod_wnd->DoModal();
				del(mod_wnd);
			}
			break;
		}
	}
};