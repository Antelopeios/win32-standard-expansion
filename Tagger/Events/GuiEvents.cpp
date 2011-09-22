#include "stdafx.h"
#include "Main/Tagger.h"

//////////////////////////////////////////////////////////////////

class CEvent_wnd : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_wnd, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiBoard* board = ExDynCast<IGuiBoard>(pGui);
		if (!board) return;

		switch( nMessage )
		{
		case WM_SIZE:
			for(IGuiBase::list_t::iterator_t ite = board->GetChildren().Head(); ite != board->GetChildren().Tail(); ++ite)
			{
				IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(*ite);
				if (!ctrl) continue;
				ctrl->Send(ExDynCast<IGuiObject>(ctrl), WM_SHOWWINDOW, 1);
			}
			break;
		case WM_CLOSE:
			board->DefProc(nMessage, wParam, lParam);
			break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_wnd, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_ground : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_ground, IGuiEvent)

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
				ctrl->SetWindowRect(rc_wnd);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_ground, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_bottom1 : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_bottom1, IGuiEvent)

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
				rc_wnd.Deflate(CPoint(1, 1));
				rc_wnd.Height(20);
				ctrl->SetWindowRect(rc_wnd);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_bottom1, IGuiEvent)

class CEvent_bottom2 : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_bottom2, IGuiEvent)

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
				rc_wnd.Deflate(CPoint(2, 2));
				rc_wnd.Height(18);
				ctrl->SetWindowRect(rc_wnd);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_bottom2, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_search : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_search, IGuiEvent)

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
				rc_wnd.Deflate(CPoint(4, 4));
				rc_wnd.Height(16);
				ctrl->SetWindowRect(rc_wnd);
			}
			break;
		case WM_KILLFOCUS:
			{
				CString* edit = (CString*)ctrl->GetState(_T("edit"));
				if (edit->Empty())
					GET_CTL(cover)->SetVisible(true);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_search, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_cover : public CEvent_search
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_cover, CEvent_search)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		switch( nMessage )
		{
		case WM_SETFOCUS:
			{
				ctrl->SetVisible(false);
				GET_CTL(search)->SetFocus();
			}
			break;
		case WM_KILLFOCUS:
			return;
		}
		EXP_BASE::OnMessage(pGui, nMessage, wParam, lParam);
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_cover, CEvent_search)

//////////////////////////////////////////////////////////////////

class CEvent_cloud : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_cloud, IGuiEvent)

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
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				CRect rc_wnd;
				wnd->GetClientRect(rc_wnd);
				rc_wnd.Top(22);

				if (GET_CTL(scr_h)->IsVisible())
					rc_wnd.Right(rc_wnd.Right() - 10);
				ctrl->SetWindowRect(rc_wnd);
			}
			break;
		case WM_SIZE:
			{
				LONG all_line = (LONG)(LONG_PTR)ctrl->GetState(_T("all_line"));
				LONG fra_line = (LONG)(LONG_PTR)ctrl->GetState(_T("fra_line"));

				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				CRect rc_wnd;
				wnd->GetClientRect(rc_wnd);

				if (all_line > fra_line)
				{
					if(!GET_CTL(scr_h)->IsVisible())
					{
						GET_CTL(scr_h)->SetVisible(true);
						CRect rc;
						ctrl->GetWindowRect(rc);
						rc.Right(rc_wnd.Right() - 10);
						ctrl->SetWindowRect(rc);
					}
				}
				else
				{
					if (GET_CTL(scr_h)->IsVisible())
					{
						GET_CTL(scr_h)->SetVisible(false);
						CRect rc;
						ctrl->GetWindowRect(rc);
						rc.Right(rc_wnd.Right());
						ctrl->SetWindowRect(rc);
					}
				}
			}
			break;
		case WM_MOUSEWHEEL:
			GET_CTL(scr_h)->Send(ExDynCast<IGuiObject>(GET_CTL(scr_h)), nMessage, wParam, lParam);
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_cloud, IGuiEvent)

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
				rc_wnd.Top(22);

				rc_wnd.Left(rc_wnd.Right() - 10);
				ctrl->SetWindowRect(rc_wnd);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_scr_h, IGuiEvent)
