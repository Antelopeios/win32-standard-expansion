#include "stdafx.h"
#include "DemoGui1.h"

//////////////////////////////////////////////////////////////////

class CEvent_banner : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_banner, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		switch( nMessage )
		{
		case WM_NCHITTEST:
			{
				LRESULT hit_test = GetResult();
				SetResult((hit_test == HTCLIENT) ? HTCAPTION : hit_test);
			}
			break;
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetClientRect(rc_wnd);

				LONG l, r, t, b;
				l = rc_wnd.Left() + CResManager::line_left.GetWidth();
				t = rc_wnd.Top() + CResManager::line_top.GetHeight();
				r = rc_wnd.Right() - CResManager::line_right.GetWidth();
				b = t + CResManager::banner.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_banner, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_corner_lb : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_corner_lb, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetClientRect(rc_wnd);

				LONG l, r, t, b;
				l = rc_wnd.Left();
				t = rc_wnd.Bottom() - CResManager::corner_lb.GetHeight();
				r = l + CResManager::corner_lb.GetWidth();
				b = t + CResManager::corner_lb.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_corner_lb, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_corner_rb : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_corner_rb, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetClientRect(rc_wnd);

				LONG l, r, t, b;
				l = rc_wnd.Right() - CResManager::corner_rb.GetWidth();
				t = rc_wnd.Bottom() - CResManager::corner_rb.GetHeight();
				r = l + CResManager::corner_rb.GetWidth();
				b = t + CResManager::corner_rb.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_corner_rb, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_corner_rt : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_corner_rt, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetClientRect(rc_wnd);

				LONG l, r, t, b;
				l = rc_wnd.Right() - CResManager::corner_rt.GetWidth();
				t = rc_wnd.Top();
				r = l + CResManager::corner_rt.GetWidth();
				b = t + CResManager::corner_rt.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_corner_rt, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_corner_lt : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_corner_lt, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetClientRect(rc_wnd);

				LONG l, r, t, b;
				l = rc_wnd.Left();
				t = rc_wnd.Top();
				r = l + CResManager::corner_lt.GetWidth();
				b = t + CResManager::corner_lt.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_corner_lt, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_line_bottom : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_line_bottom, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetClientRect(rc_wnd);

				LONG l, r, t, b;
				l = rc_wnd.Left() + CResManager::corner_lb.GetWidth();
				t = rc_wnd.Bottom() - CResManager::line_bottom.GetHeight();
				r = rc_wnd.Right() - CResManager::corner_rb.GetWidth();
				b = t + CResManager::line_bottom.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_line_bottom, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_line_left : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_line_left, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetClientRect(rc_wnd);

				LONG l, r, t, b;
				l = rc_wnd.Left();
				t = rc_wnd.Top() + CResManager::corner_lt.GetHeight();
				r = l + CResManager::line_left.GetWidth();
				b = rc_wnd.Bottom() - CResManager::corner_lb.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_line_left, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_line_right : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_line_right, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetClientRect(rc_wnd);

				LONG l, r, t, b;
				l = rc_wnd.Right() - CResManager::line_right.GetWidth();
				t = rc_wnd.Top() + CResManager::corner_rt.GetHeight();
				r = l + CResManager::line_right.GetWidth();
				b = rc_wnd.Bottom() - CResManager::corner_rb.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_line_right, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_line_top : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_line_top, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetClientRect(rc_wnd);

				LONG l, r, t, b;
				l = rc_wnd.Left() + CResManager::corner_lt.GetWidth();
				t = rc_wnd.Top();
				r = rc_wnd.Right() - CResManager::corner_rt.GetWidth();
				b = t + CResManager::line_top.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_line_top, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_tag_bg : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_tag_bg, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetClientRect(rc_wnd);

				LONG l, r, t, b;
				l = rc_wnd.Left() + CResManager::line_left.GetWidth();
				t = rc_wnd.Top() + CResManager::line_top.GetHeight() + 115;
				r = rc_wnd.Right() - CResManager::line_right.GetWidth();
				b = t + CResManager::tag_bg.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_tag_bg, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_toolbar_bg : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_toolbar_bg, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetClientRect(rc_wnd);

				LONG l, r, t, b;
				l = rc_wnd.Left() + CResManager::line_left.GetWidth();
				t = rc_wnd.Bottom() - CResManager::line_bottom.GetHeight() - CResManager::toolbar_bg.GetHeight();
				r = rc_wnd.Right() - CResManager::line_right.GetWidth();
				b = t + CResManager::toolbar_bg.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_toolbar_bg, IGuiEvent)

//////////////////////////////////////////////////////////////////
