#pragma once

//////////////////////////////////////////////////////////////////

class CCustomEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CCustomEvent, IGuiEvent)

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

EXP_IMPLEMENT_DYNCREATE_CLS(CCustomEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CCusPicEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CCusPicEvent, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			{
				IGuiWnd* wnd = ExGet<IGuiWnd>(_T("wnd"));
				if (wParam && ctl && ::IsWindowVisible(wnd->GethWnd()))
				{
					wnd->ShowWindow(SW_HIDE);
					CImage* img = (CImage*)ctl->GetState(_T("image"));
					CRect rc(0, 0, img->GetWidth(), img->GetHeight()), rect;
					wnd->GetWindowRect(rect);
					rect.Width(rc.Width());
					rect.Height(rc.Height());
					wnd->MoveWindow(rect);
					ctl->SetWindowRect(rc);
					wnd->ShowWindow(SW_SHOW);
				}
			}
			break;
		case WM_NCHITTEST:
			{
				LRESULT hit_test = GetResult();
				SetResult((hit_test == HTCLIENT) ? HTCAPTION : hit_test);
				SetState(return_next);
			}
			break;
		case WM_KEYDOWN:
			{
				if (wParam == VK_RETURN && ctl) // Enter
					ctl->SetVisible(!ctl->IsVisible());
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CCusPicEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CCusBtnEvent2;
class CCusBtnEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CCusBtnEvent, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		switch( nMessage )
		{
		case BM_CLICK:
			{
				CGuiSkin::Parse(_T("<image name=\"bk\" path=\"TestImg1/ground.png\" />"));
				ExGet<IGuiWnd>(_T("wnd"))->SetLayered(TRUE, FALSE);
				ExGet<IGuiCtl>(_T("btn"))->PopEvent();
				ExGet<IGuiCtl>(_T("btn"))->AddEvent((IGuiEvent*)dbnew(CCusBtnEvent2));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CCusBtnEvent, IGuiEvent)
	
//////////////////////////////////////////////////////////////////

class CCusBtnEvent2 : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CCusBtnEvent2, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		switch( nMessage )
		{
		case BM_CLICK:
			{
				CGuiSkin::Parse(_T("<image name=\"bk\" path=\"TestImg1/ground.jpg\" />"));
				ExGet<IGuiWnd>(_T("wnd"))->SetLayered(FALSE, FALSE);
				ExGet<IGuiCtl>(_T("btn"))->PopEvent();
				ExGet<IGuiCtl>(_T("btn"))->AddEvent((IGuiEvent*)dbnew(CCusBtnEvent));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CCusBtnEvent2, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_scr_h : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_scr_h, IGuiEvent)

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
				ExAssert(wnd);
				wnd->GetClientRect(rc_wnd);
				rc_wnd.Inflate(CPoint(1, 1));

				LONG l, r, t, b;
				l = rc_wnd.Right() - 20;
				t = rc_wnd.Top();
				r = rc_wnd.Right();
				b = rc_wnd.Bottom();
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};
