#include "stdafx.h"
#include "DemoGui1.h"

#define GUI_W_MIN 700
#define GUI_H_MIN 500

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
				ctrl->SetVisible(false);
				ctrl->SetVisible(true);
			}
			break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_wnd, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_banner : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_banner, IGuiEvent)

protected:
	bool m_bZoomed;

public:
	CEvent_banner()
		: m_bZoomed(false)
	{}

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
		case WM_NCLBUTTONDOWN:
			{
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				m_bZoomed = ::IsZoomed(wnd->GethWnd());
			}
			break;
		case WM_NCLBUTTONDBLCLK:
			{
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				IGuiComp::list_t::iterator_t ite = wnd->FindComp(ctrl);
				if (m_bZoomed)
					ite += 6; // win_sysbtn_restore
				else
					ite += 5; // win_sysbtn_max
				ctrl = ExDynCast<IGuiCtrl>(*ite);
				ctrl->Send(ExDynCast<IGuiObject>(*ite), WM_COMMAND, BN_CLICKED);
			}
			break;
		case WM_SHOWWINDOW:
			if (wParam)
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

protected:
	bool m_MouseDown;

public:
	CEvent_corner_lb()
		: m_MouseDown(false)
	{}

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

				LONG l, r, t, b;
				l = rc_wnd.Left();
				t = rc_wnd.Bottom() - CResManager::corner_lb.GetHeight();
				r = l + CResManager::corner_lb.GetWidth();
				b = t + CResManager::corner_lb.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_MOUSEMOVE:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
			// 鼠标移动时定位光标
			if (m_MouseDown)
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetWindowRect(rc_wnd);

				POINT pt_tmp = {0};
				::GetCursorPos(&pt_tmp); /*此处有可能移动到窗口外部,而lParam不支持负坐标*/

				rc_wnd.pt1.x = pt_tmp.x;
				rc_wnd.pt2.y = pt_tmp.y;
				if (rc_wnd.Width() < GUI_W_MIN)
					rc_wnd.pt1.x = rc_wnd.pt2.x - GUI_W_MIN;
				if (rc_wnd.Height() < GUI_H_MIN)
					rc_wnd.pt2.y = rc_wnd.pt1.y + GUI_H_MIN;
				wnd->MoveWindow(rc_wnd);
			}
			break;
		case WM_LBUTTONDOWN:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
			ctrl->SetCapture();
			m_MouseDown = true;
			break;
		case WM_LBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
			m_MouseDown = false;
			ctrl->ReleaseCapture();
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_corner_lb, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_corner_rb : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_corner_rb, IGuiEvent)

protected:
	bool m_MouseDown;

public:
	CEvent_corner_rb()
		: m_MouseDown(false)
	{}

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

				LONG l, r, t, b;
				l = rc_wnd.Right() - CResManager::corner_rb.GetWidth();
				t = rc_wnd.Bottom() - CResManager::corner_rb.GetHeight();
				r = l + CResManager::corner_rb.GetWidth();
				b = t + CResManager::corner_rb.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_MOUSEMOVE:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
			// 鼠标移动时定位光标
			if (m_MouseDown)
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetWindowRect(rc_wnd);

				POINT pt_tmp = {0};
				::GetCursorPos(&pt_tmp); /*此处有可能移动到窗口外部,而lParam不支持负坐标*/

				rc_wnd.pt2.x = pt_tmp.x;
				rc_wnd.pt2.y = pt_tmp.y;
				if (rc_wnd.Width() < GUI_W_MIN)
					rc_wnd.pt2.x = rc_wnd.pt1.x + GUI_W_MIN;
				if (rc_wnd.Height() < GUI_H_MIN)
					rc_wnd.pt2.y = rc_wnd.pt1.y + GUI_H_MIN;
				wnd->MoveWindow(rc_wnd);
			}
			break;
		case WM_LBUTTONDOWN:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
			ctrl->SetCapture();
			m_MouseDown = true;
			break;
		case WM_LBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
			m_MouseDown = false;
			ctrl->ReleaseCapture();
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_corner_rb, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_corner_rt : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_corner_rt, IGuiEvent)

protected:
	bool m_MouseDown;

public:
	CEvent_corner_rt()
		: m_MouseDown(false)
	{}

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

				LONG l, r, t, b;
				l = rc_wnd.Right() - CResManager::corner_rt.GetWidth();
				t = rc_wnd.Top();
				r = l + CResManager::corner_rt.GetWidth();
				b = t + CResManager::corner_rt.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_MOUSEMOVE:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
			// 鼠标移动时定位光标
			if (m_MouseDown)
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetWindowRect(rc_wnd);

				POINT pt_tmp = {0};
				::GetCursorPos(&pt_tmp); /*此处有可能移动到窗口外部,而lParam不支持负坐标*/

				rc_wnd.pt2.x = pt_tmp.x;
				rc_wnd.pt1.y = pt_tmp.y;
				if (rc_wnd.Width() < GUI_W_MIN)
					rc_wnd.pt2.x = rc_wnd.pt1.x + GUI_W_MIN;
				if (rc_wnd.Height() < GUI_H_MIN)
					rc_wnd.pt1.y = rc_wnd.pt2.y - GUI_H_MIN;
				wnd->MoveWindow(rc_wnd);
			}
			break;
		case WM_LBUTTONDOWN:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
			ctrl->SetCapture();
			m_MouseDown = true;
			break;
		case WM_LBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
			m_MouseDown = false;
			ctrl->ReleaseCapture();
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_corner_rt, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_corner_lt : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_corner_lt, IGuiEvent)

protected:
	bool m_MouseDown;

public:
	CEvent_corner_lt()
		: m_MouseDown(false)
	{}

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

				LONG l, r, t, b;
				l = rc_wnd.Left();
				t = rc_wnd.Top();
				r = l + CResManager::corner_lt.GetWidth();
				b = t + CResManager::corner_lt.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_MOUSEMOVE:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
			// 鼠标移动时定位光标
			if (m_MouseDown)
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetWindowRect(rc_wnd);

				POINT pt_tmp = {0};
				::GetCursorPos(&pt_tmp); /*此处有可能移动到窗口外部,而lParam不支持负坐标*/

				rc_wnd.pt1.x = pt_tmp.x;
				rc_wnd.pt1.y = pt_tmp.y;
				if (rc_wnd.Width() < GUI_W_MIN)
					rc_wnd.pt1.x = rc_wnd.pt2.x - GUI_W_MIN;
				if (rc_wnd.Height() < GUI_H_MIN)
					rc_wnd.pt1.y = rc_wnd.pt2.y - GUI_H_MIN;
				wnd->MoveWindow(rc_wnd);
			}
			break;
		case WM_LBUTTONDOWN:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
			ctrl->SetCapture();
			m_MouseDown = true;
			break;
		case WM_LBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
			m_MouseDown = false;
			ctrl->ReleaseCapture();
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_corner_lt, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_line_bottom : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_line_bottom, IGuiEvent)

protected:
	bool m_MouseDown;

public:
	CEvent_line_bottom()
		: m_MouseDown(false)
	{}

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

				LONG l, r, t, b;
				l = rc_wnd.Left() + CResManager::corner_lb.GetWidth();
				t = rc_wnd.Bottom() - CResManager::line_bottom.GetHeight();
				r = rc_wnd.Right() - CResManager::corner_rb.GetWidth();
				b = t + CResManager::line_bottom.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_MOUSEMOVE:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			// 鼠标移动时定位光标
			if (m_MouseDown)
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetWindowRect(rc_wnd);

				POINT pt_tmp = {0};
				::GetCursorPos(&pt_tmp); /*此处有可能移动到窗口外部,而lParam不支持负坐标*/

				rc_wnd.pt2.y = pt_tmp.y;
				if (rc_wnd.Height() < GUI_H_MIN)
					rc_wnd.pt2.y = rc_wnd.pt1.y + GUI_H_MIN;
				wnd->MoveWindow(rc_wnd);
			}
			break;
		case WM_LBUTTONDOWN:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			ctrl->SetCapture();
			m_MouseDown = true;
			break;
		case WM_LBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			m_MouseDown = false;
			ctrl->ReleaseCapture();
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_line_bottom, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_line_left : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_line_left, IGuiEvent)

protected:
	bool m_MouseDown;

public:
	CEvent_line_left()
		: m_MouseDown(false)
	{}

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

				LONG l, r, t, b;
				l = rc_wnd.Left();
				t = rc_wnd.Top() + CResManager::corner_lt.GetHeight();
				r = l + CResManager::line_left.GetWidth();
				b = rc_wnd.Bottom() - CResManager::corner_lb.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_MOUSEMOVE:
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			// 鼠标移动时定位光标
			if (m_MouseDown)
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetWindowRect(rc_wnd);

				POINT pt_tmp = {0};
				::GetCursorPos(&pt_tmp); /*此处有可能移动到窗口外部,而lParam不支持负坐标*/

				rc_wnd.pt1.x = pt_tmp.x;
				if (rc_wnd.Width() < GUI_W_MIN)
					rc_wnd.pt1.x = rc_wnd.pt2.x - GUI_W_MIN;
				wnd->MoveWindow(rc_wnd);
			}
			break;
		case WM_LBUTTONDOWN:
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			ctrl->SetCapture();
			m_MouseDown = true;
			break;
		case WM_LBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			m_MouseDown = false;
			ctrl->ReleaseCapture();
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_line_left, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_line_right : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_line_right, IGuiEvent)

protected:
	bool m_MouseDown;

public:
	CEvent_line_right()
		: m_MouseDown(false)
	{}

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

				LONG l, r, t, b;
				l = rc_wnd.Right() - CResManager::line_right.GetWidth();
				t = rc_wnd.Top() + CResManager::corner_rt.GetHeight();
				r = l + CResManager::line_right.GetWidth();
				b = rc_wnd.Bottom() - CResManager::corner_rb.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_MOUSEMOVE:
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			// 鼠标移动时定位光标
			if (m_MouseDown)
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetWindowRect(rc_wnd);

				POINT pt_tmp = {0};
				::GetCursorPos(&pt_tmp); /*此处有可能移动到窗口外部,而lParam不支持负坐标*/

				rc_wnd.pt2.x = pt_tmp.x;
				if (rc_wnd.Width() < GUI_W_MIN)
					rc_wnd.pt2.x = rc_wnd.pt1.x + GUI_W_MIN;
				wnd->MoveWindow(rc_wnd);
			}
			break;
		case WM_LBUTTONDOWN:
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			ctrl->SetCapture();
			m_MouseDown = true;
			break;
		case WM_LBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			m_MouseDown = false;
			ctrl->ReleaseCapture();
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_line_right, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_line_top : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_line_top, IGuiEvent)

protected:
	bool m_MouseDown;

public:
	CEvent_line_top()
		: m_MouseDown(false)
	{}

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

				LONG l, r, t, b;
				l = rc_wnd.Left() + CResManager::corner_lt.GetWidth();
				t = rc_wnd.Top();
				r = rc_wnd.Right() - CResManager::corner_rt.GetWidth();
				b = t + CResManager::line_top.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_MOUSEMOVE:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			// 鼠标移动时定位光标
			if (m_MouseDown)
			{
				CRect rc_wnd;
				IGuiBoard* wnd = ctrl->GetBoard();
				ExAssert(wnd);
				wnd->GetWindowRect(rc_wnd);

				POINT pt_tmp = {0};
				::GetCursorPos(&pt_tmp); /*此处有可能移动到窗口外部,而lParam不支持负坐标*/

				rc_wnd.pt1.y = pt_tmp.y;
				if (rc_wnd.Height() < GUI_H_MIN)
					rc_wnd.pt1.y = rc_wnd.pt2.y - GUI_H_MIN;
				wnd->MoveWindow(rc_wnd);
			}
			break;
		case WM_LBUTTONDOWN:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			ctrl->SetCapture();
			m_MouseDown = true;
			break;
		case WM_LBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			m_MouseDown = false;
			ctrl->ReleaseCapture();
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
			if (wParam)
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
			if (wParam)
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

class CEvent_list : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_list, IGuiEvent)

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

				LONG l, r, t, b;
				l = rc_wnd.Left() + CResManager::line_left.GetWidth();
				t = rc_wnd.Top() + CResManager::line_top.GetHeight() + CResManager::banner.GetHeight();
				r = rc_wnd.Right() - CResManager::line_right.GetWidth();
				b = rc_wnd.Bottom() - CResManager::line_bottom.GetHeight() - CResManager::toolbar_bg.GetHeight();
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_list, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_win_sysbtn_close : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_win_sysbtn_close, IGuiEvent)

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

				LONG l, r, t, b;
				l = rc_wnd.Right() - CResManager::line_right.GetWidth() - CResManager::win_sysbtn_close.GetWidth();
				t = rc_wnd.Top() + CResManager::line_top.GetHeight();
				r = l + CResManager::win_sysbtn_close.GetWidth();
				b = t + CResManager::win_sysbtn_close.GetHeight() / 3;
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_COMMAND:
			switch (wParam)
			{
			case BN_CLICKED:
				{
					IGuiBoard* board = ctrl->GetBoard();
					if (!board) return;
					board->PostMessage(WM_CLOSE);
				}
				break;
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_win_sysbtn_close, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_win_sysbtn_max : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_win_sysbtn_max, IGuiEvent)

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

				LONG l, r, t, b;
				l = rc_wnd.Right() - 
					CResManager::line_right.GetWidth() - 
					CResManager::win_sysbtn_close.GetWidth() - 
					CResManager::win_sysbtn_max.GetWidth();
				t = rc_wnd.Top() + CResManager::line_top.GetHeight();
				r = l + CResManager::win_sysbtn_max.GetWidth();
				b = t + CResManager::win_sysbtn_max.GetHeight() / 3;
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			// 判断窗口
			{
				IGuiBoard* board = ctrl->GetBoard();
				if (!board) return;
				ctrl->SetVisible(!::IsZoomed(board->GethWnd()));
			}
			break;
		case WM_COMMAND:
			switch (wParam)
			{
			case BN_CLICKED:
				{
					IGuiBoard* board = ctrl->GetBoard();
					if (!board) return;
					// 先最大化窗口
					board->ShowWindow(SW_SHOWMAXIMIZED);
					// 重新设置窗口大小
					RECT rect;
					::GetWindowRect(board->GethWnd(), &rect);
					HMONITOR monitor = ::MonitorFromRect(&rect, MONITOR_DEFAULTTONEAREST);
					MONITORINFO mi = {0};
					mi.cbSize = sizeof(mi);
					::GetMonitorInfo(monitor, &mi);
					board->MoveWindow(CRect(mi.rcWork));
				}
				break;
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_win_sysbtn_max, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_win_sysbtn_restore : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_win_sysbtn_restore, IGuiEvent)

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

				LONG l, r, t, b;
				l = rc_wnd.Right() - 
					CResManager::line_right.GetWidth() - 
					CResManager::win_sysbtn_close.GetWidth() - 
					CResManager::win_sysbtn_restore.GetWidth();
				t = rc_wnd.Top() + CResManager::line_top.GetHeight();
				r = l + CResManager::win_sysbtn_restore.GetWidth();
				b = t + CResManager::win_sysbtn_restore.GetHeight() / 3;
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			// 判断窗口
			{
				IGuiBoard* board = ctrl->GetBoard();
				if (!board) return;
				ctrl->SetVisible(::IsZoomed(board->GethWnd()));
			}
			break;
		case WM_COMMAND:
			switch (wParam)
			{
			case BN_CLICKED:
				{
					IGuiBoard* board = ctrl->GetBoard();
					if (!board) return;
					// 还原窗口
					board->ShowWindow(SW_SHOWNORMAL);
				}
				break;
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_win_sysbtn_restore, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_win_sysbtn_min : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_win_sysbtn_min, IGuiEvent)

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

				LONG w_dec = 
					::IsZoomed(wnd->GethWnd()) ? 
					CResManager::win_sysbtn_restore.GetWidth() : 
					CResManager::win_sysbtn_max.GetWidth();

				LONG l, r, t, b;
				l = rc_wnd.Right() - 
					CResManager::line_right.GetWidth() - 
					CResManager::win_sysbtn_close.GetWidth() - w_dec - 
					CResManager::win_sysbtn_min.GetWidth();
				t = rc_wnd.Top() + CResManager::line_top.GetHeight();
				r = l + CResManager::win_sysbtn_min.GetWidth();
				b = t + CResManager::win_sysbtn_min.GetHeight() / 3;
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_COMMAND:
			switch (wParam)
			{
			case BN_CLICKED:
				{
					IGuiBoard* board = ctrl->GetBoard();
					if (!board) return;
					// 最小化窗口
					board->ShowWindow(SW_SHOWMINIMIZED);
				}
				break;
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_win_sysbtn_min, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_topbar_btn : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_topbar_btn, IGuiEvent)

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

				LONG l, r, t, b;
				l = rc_wnd.Left() + CResManager::line_left.GetWidth();
				t = rc_wnd.Top() + 
					CResManager::line_top.GetHeight() + 
					CResManager::banner.GetHeight() - 
					CResManager::tag_bg.GetHeight() - 
					CResManager::topbar_btn.GetHeight() / 3;
				r = l + CResManager::topbar_btn.GetWidth();
				b = t + CResManager::topbar_btn.GetHeight() / 3;
				ctrl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_topbar_btn, IGuiEvent)

//////////////////////////////////////////////////////////////////
