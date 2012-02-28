#include "stdafx.h"
#include "../Main/Tagger.h"

//////////////////////////////////////////////////////////////////

class CEvent_wnd : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_wnd, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiWnd* wnd = ExDynCast<IGuiWnd>(pGui);
		if (!wnd) return;

		switch( nMessage )
		{
		case WM_SIZE:
			for(IGuiBase::list_t::iterator_t ite = wnd->GetChildren().Head(); ite != wnd->GetChildren().Tail(); ++ite)
			{
				IGuiCtl* ctl = ExDynCast<IGuiCtl>(*ite);
				if (!ctl) continue;
				ctl->SendMessage(WM_SHOWWINDOW, 1);
			}
			break;
		case WM_KEYDOWN:
			if (GET_CTL(files)->IsVisible())
				GET_CTL(files)->SendMessage(nMessage, wParam, lParam);
			break;
		case WM_CLOSE:
			wnd->DefProc(nMessage, wParam, lParam);
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
				ctl->SetWindowRect(rc_wnd);
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
				rc_wnd.Deflate(CPoint(1, 1));
				rc_wnd.Height(20);
				ctl->SetWindowRect(rc_wnd);
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
				rc_wnd.Deflate(CPoint(2, 2));
				rc_wnd.Height(18);
				ctl->SetWindowRect(rc_wnd);
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
				rc_wnd.Deflate(CPoint(4, 4));
				rc_wnd.Height(16);
				ctl->SetWindowRect(rc_wnd);
			}
			break;
		case WM_KILLFOCUS:
			{
				CString* edit = (CString*)ctl->GetState(_T("text"));
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
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		switch( nMessage )
		{
		case WM_SETFOCUS:
			{
				ctl->SetVisible(FALSE);
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
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (wParam)
			{
				IGuiWnd* wnd = ctl->GetWnd();
				ExAssert(wnd);
				CRect rc_wnd;
				wnd->GetClientRect(rc_wnd);
				rc_wnd.Top(22);

				ctl->SetWindowRect(rc_wnd);
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
				rc_wnd.Top(22);

				rc_wnd.Left(rc_wnd.Right() - GUI()->ScrWidth());
				ctl->SetWindowRect(rc_wnd);
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
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (wParam)
			{
				IGuiWnd* wnd = ctl->GetWnd();
				ExAssert(wnd);
				CRect rc_wnd;
				wnd->GetClientRect(rc_wnd);
				rc_wnd.Top(22);

				ctl->SetWindowRect(rc_wnd);
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
				rc_wnd.Top(22);

				rc_wnd.Left(rc_wnd.Right() - GUI()->ScrWidth());
				ctl->SetWindowRect(rc_wnd);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_scr_files, IGuiEvent)
