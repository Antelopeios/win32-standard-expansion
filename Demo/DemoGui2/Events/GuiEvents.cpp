#include "stdafx.h"
#include "../Main/Tagger.h"

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
		case WM_KEYDOWN:
			if (GET_CTL(files)->IsVisible())
				GET_CTL(files)->SendMessage(nMessage, wParam, lParam);
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
				CString* edit = (CString*)ctrl->GetState(_T("text"));
				if (edit->Empty())
					GET_CTL(cover)->SetVisible(TRUE);
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
				ctrl->SetVisible(FALSE);
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

				ctrl->SetWindowRect(rc_wnd);
			}
			break;
		case WM_COMMAND:
			if (wParam == BN_CLICKED)
			{
				GET_CTL(cloud)->SetVisible(FALSE);
				GET_CTL(files)->SetVisible(TRUE);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_cloud, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_scr_cloud : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_scr_cloud, IGuiEvent)

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

				rc_wnd.Left(rc_wnd.Right() - GUI()->ScrWidth());
				ctrl->SetWindowRect(rc_wnd);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_scr_cloud, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_files : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_files, IGuiEvent)

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

				ctrl->SetWindowRect(rc_wnd);
			}
			break;
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE || 
				wParam == VK_BACK || 
				wParam == VK_DELETE)
			{
				GET_CTL(files)->SetVisible(FALSE);
				GET_CTL(cloud)->SetVisible(TRUE);
			}
			break;
		case WM_COMMAND:
			if (wParam == BN_CLICKED)
			{
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_files, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_scr_files : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_scr_files, IGuiEvent)

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

				rc_wnd.Left(rc_wnd.Right() - GUI()->ScrWidth());
				ctrl->SetWindowRect(rc_wnd);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_scr_files, IGuiEvent)
