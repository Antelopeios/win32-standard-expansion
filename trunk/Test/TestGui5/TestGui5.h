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
		case WM_SHOWWINDOW:
			if (wParam)
			{
				CRect rc;
				ExGet(IGuiCtrl, _T("gnd"))->GetWindowRect(rc);
				board->MoveWindow(rc);
				board->CenterWindow();
			}
			break;
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
			if (wParam && ctrl)
			{
				CImage* img = (CImage*)ctrl->GetState(_T("image"));
				CRect rc(0, 0, img->GetWidth(), img->GetHeight());
				ctrl->SetWindowRect(rc);
				ExGet(IGuiBoard, _T("wnd"))->MoveWindow(rc);
				ExGet(IGuiBoard, _T("wnd"))->CenterWindow();
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
				CIOFile file(_T("TestGui5\\test2.xml"));
				if (file.Error() || file.Size() == 0) return;
				CGuiSkin::Load(&file);
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
				CIOFile file(_T("TestGui5\\test.xml"));
				if (file.Error() || file.Size() == 0) return;
				CGuiSkin::Load(&file);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CCusBtnEvent2, IGuiEvent)