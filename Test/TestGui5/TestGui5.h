#pragma once

//////////////////////////////////////////////////////////////////

class CCustomEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CCustomEvent, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiBoard* board = ExDynCast<IGuiBoard>(pGui);
		if (!board) return;

		switch( nMessage )
		{
		case WM_KEYDOWN:
			{
				if (wParam == VK_ESCAPE) // ESC
					board->PostMessage(WM_CLOSE);
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
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (wParam && ctrl && ::IsWindowVisible(ExGet(IGuiBoard, _T("wnd"))->GethWnd()))
			{
				ExGet(IGuiBoard, _T("wnd"))->ShowWindow(SW_HIDE);
				CImage* img = (CImage*)ctrl->GetState(_T("image"));
				CRect rc(0, 0, img->GetWidth(), img->GetHeight()), rect;
				ExGet(IGuiBoard, _T("wnd"))->GetWindowRect(rect);
				rect.Width(rc.Width());
				rect.Height(rc.Height());
				ExGet(IGuiBoard, _T("wnd"))->MoveWindow(rect);
				ctrl->SetWindowRect(rc);
				ExGet(IGuiBoard, _T("wnd"))->ShowWindow(SW_SHOW);
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
				if (wParam == VK_RETURN && ctrl) // Enter
					ctrl->SetVisible(!ctrl->IsVisible());
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
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		switch( nMessage )
		{
		case BM_CLICK:
			{
				CGuiSkin::Load(_T("<image name=\"bk\" path=\"TestImg1/ground.png\" />"));
				ExGet(IGuiBoard, _T("wnd"))->SetLayered(TRUE, FALSE);
				ExGet(IGuiCtrl, _T("btn"))->DelEvent();
				ExGet(IGuiCtrl, _T("btn"))->AddEvent((IGuiEvent*)dbnew(CCusBtnEvent2));
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
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		switch( nMessage )
		{
		case BM_CLICK:
			{
				CGuiSkin::Load(_T("<image name=\"bk\" path=\"TestImg1/ground.jpg\" />"));
				ExGet(IGuiBoard, _T("wnd"))->SetLayered(FALSE, FALSE);
				ExGet(IGuiCtrl, _T("btn"))->DelEvent();
				ExGet(IGuiCtrl, _T("btn"))->AddEvent((IGuiEvent*)dbnew(CCusBtnEvent));
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
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (wParam)
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetClientRect(rc_wnd);
				rc_wnd.Inflate(CPoint(1, 1));

				LONG l, r, t, b;
				l = rc_wnd.Right() - 20;
				t = rc_wnd.Top();
				r = rc_wnd.Right();
				b = rc_wnd.Bottom();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};
