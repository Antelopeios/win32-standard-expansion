#include "stdafx.h"

#define GUI_W_MIN 1000
#define GUI_H_MIN 750

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
			for(IGuiBase::list_t::iterator_t ite = wnd->GetComp().Head(); ite != wnd->GetComp().Tail(); ++ite)
			{
				IGuiCtl* ctl = ExDynCast<IGuiCtl>(*ite);
				if(!ctl || ExDynCheck(_T("CGuiLVItem"), ExDynCast<IBaseObject>(ctl))) continue;
				if (ctl->IsVisible())
					ctl->SendMessage(WM_SHOWWINDOW, 1);
			}
			{
				IGuiCtl* ctl = ::IsZoomed(wnd->GethWnd()) ? GUI_CTL(win_sysbtn_max) : GUI_CTL(win_sysbtn_restore);
				ctl->SendMessage(BM_CLICK);
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
	BOOL m_bZoomed;

public:
	CEvent_banner()
		: m_bZoomed(FALSE)
	{}

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
		//case WM_NCLBUTTONDOWN:
		//	{
		//		IGuiWnd* wnd = ctl->GetWnd();
		//		ExAssert(wnd);
		//		m_bZoomed = ::IsZoomed(wnd->GethWnd());
		//	}
		//	break;
		//case WM_NCLBUTTONDBLCLK:
		//	{
		//		IGuiWnd* wnd = ctl->GetWnd();
		//		ExAssert(wnd);
		//		ctl = m_bZoomed ? GUI_CTL(win_sysbtn_restore) : GUI_CTL(win_sysbtn_max);
		//		ctl->SendMessage(BM_CLICK);
		//	}
		//	break;
		case WM_SHOWWINDOW:
			if (wParam)
			{
				CRect rc_wnd;
				IGuiWnd* wnd = ctl->GetWnd();
				ExAssert(wnd);
				wnd->GetClientRect(rc_wnd);
				rc_wnd.Inflate(CPoint(1, 1));

				LONG l, r, t, b;
				l = rc_wnd.Left() + GUI_IMG(line_left)->GetWidth();
				t = rc_wnd.Top() + GUI_IMG(line_top)->GetHeight();
				r = rc_wnd.Right() - GUI_IMG(line_right)->GetWidth();
				b = t + GUI_IMG(banner)->GetHeight();
				ctl->SetWindowRect(CRect(l, t, r, b));
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
	BOOL m_MouseDown;

public:
	CEvent_corner_lb()
		: m_MouseDown(FALSE)
	{}

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

				LONG l, r, t, b;
				l = rc_wnd.Left();
				t = rc_wnd.Bottom() - GUI_IMG(corner_lb)->GetHeight();
				r = l + GUI_IMG(corner_lb)->GetWidth();
				b = t + GUI_IMG(corner_lb)->GetHeight();
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_MOUSEMOVE:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
			// 鼠标移动时定位光标
			if (m_MouseDown)
			{
				CRect rc_wnd;
				IGuiWnd* wnd = ctl->GetWnd();
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
			ctl->SetCapture();
			m_MouseDown = TRUE;
			break;
		case WM_LBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
			m_MouseDown = FALSE;
			ctl->ReleaseCapture();
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
	BOOL m_MouseDown;

public:
	CEvent_corner_rb()
		: m_MouseDown(FALSE)
	{}

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

				LONG l, r, t, b;
				l = rc_wnd.Right() - GUI_IMG(corner_rb)->GetWidth();
				t = rc_wnd.Bottom() - GUI_IMG(corner_rb)->GetHeight();
				r = l + GUI_IMG(corner_rb)->GetWidth();
				b = t + GUI_IMG(corner_rb)->GetHeight();
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_MOUSEMOVE:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
			// 鼠标移动时定位光标
			if (m_MouseDown)
			{
				CRect rc_wnd;
				IGuiWnd* wnd = ctl->GetWnd();
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
			ctl->SetCapture();
			m_MouseDown = TRUE;
			break;
		case WM_LBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
			m_MouseDown = FALSE;
			ctl->ReleaseCapture();
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
	BOOL m_MouseDown;

public:
	CEvent_corner_rt()
		: m_MouseDown(FALSE)
	{}

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

				LONG l, r, t, b;
				l = rc_wnd.Right() - GUI_IMG(corner_rt)->GetWidth();
				t = rc_wnd.Top();
				r = l + GUI_IMG(corner_rt)->GetWidth();
				b = t + GUI_IMG(corner_rt)->GetHeight();
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_MOUSEMOVE:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
			// 鼠标移动时定位光标
			if (m_MouseDown)
			{
				CRect rc_wnd;
				IGuiWnd* wnd = ctl->GetWnd();
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
			ctl->SetCapture();
			m_MouseDown = TRUE;
			break;
		case WM_LBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
			m_MouseDown = FALSE;
			ctl->ReleaseCapture();
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
	BOOL m_MouseDown;

public:
	CEvent_corner_lt()
		: m_MouseDown(FALSE)
	{}

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

				LONG l, r, t, b;
				l = rc_wnd.Left();
				t = rc_wnd.Top();
				r = l + GUI_IMG(corner_lt)->GetWidth();
				b = t + GUI_IMG(corner_lt)->GetHeight();
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_MOUSEMOVE:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
			// 鼠标移动时定位光标
			if (m_MouseDown)
			{
				CRect rc_wnd;
				IGuiWnd* wnd = ctl->GetWnd();
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
			ctl->SetCapture();
			m_MouseDown = TRUE;
			break;
		case WM_LBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
			m_MouseDown = FALSE;
			ctl->ReleaseCapture();
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
	BOOL m_MouseDown;

public:
	CEvent_line_bottom()
		: m_MouseDown(FALSE)
	{}

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

				LONG l, r, t, b;
				l = rc_wnd.Left() + GUI_IMG(corner_lb)->GetWidth();
				t = rc_wnd.Bottom() - GUI_IMG(line_bottom)->GetHeight();
				r = rc_wnd.Right() - GUI_IMG(corner_rb)->GetWidth();
				b = t + GUI_IMG(line_bottom)->GetHeight();
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_MOUSEMOVE:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			// 鼠标移动时定位光标
			if (m_MouseDown)
			{
				CRect rc_wnd;
				IGuiWnd* wnd = ctl->GetWnd();
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
			ctl->SetCapture();
			m_MouseDown = TRUE;
			break;
		case WM_LBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			m_MouseDown = FALSE;
			ctl->ReleaseCapture();
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
	BOOL m_MouseDown;

public:
	CEvent_line_left()
		: m_MouseDown(FALSE)
	{}

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

				LONG l, r, t, b;
				l = rc_wnd.Left();
				t = rc_wnd.Top() + GUI_IMG(corner_lt)->GetHeight();
				r = l + GUI_IMG(line_left)->GetWidth();
				b = rc_wnd.Bottom() - GUI_IMG(corner_lb)->GetHeight();
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_MOUSEMOVE:
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			// 鼠标移动时定位光标
			if (m_MouseDown)
			{
				CRect rc_wnd;
				IGuiWnd* wnd = ctl->GetWnd();
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
			ctl->SetCapture();
			m_MouseDown = TRUE;
			break;
		case WM_LBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			m_MouseDown = FALSE;
			ctl->ReleaseCapture();
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
	BOOL m_MouseDown;

public:
	CEvent_line_right()
		: m_MouseDown(FALSE)
	{}

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

				LONG l, r, t, b;
				l = rc_wnd.Right() - GUI_IMG(line_right)->GetWidth();
				t = rc_wnd.Top() + GUI_IMG(corner_rt)->GetHeight();
				r = l + GUI_IMG(line_right)->GetWidth();
				b = rc_wnd.Bottom() - GUI_IMG(corner_rb)->GetHeight();
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_MOUSEMOVE:
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			// 鼠标移动时定位光标
			if (m_MouseDown)
			{
				CRect rc_wnd;
				IGuiWnd* wnd = ctl->GetWnd();
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
			ctl->SetCapture();
			m_MouseDown = TRUE;
			break;
		case WM_LBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			m_MouseDown = FALSE;
			ctl->ReleaseCapture();
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
	BOOL m_MouseDown;

public:
	CEvent_line_top()
		: m_MouseDown(FALSE)
	{}

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

				LONG l, r, t, b;
				l = rc_wnd.Left() + GUI_IMG(corner_lt)->GetWidth();
				t = rc_wnd.Top();
				r = rc_wnd.Right() - GUI_IMG(corner_rt)->GetWidth();
				b = t + GUI_IMG(line_top)->GetHeight();
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_MOUSEMOVE:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			// 鼠标移动时定位光标
			if (m_MouseDown)
			{
				CRect rc_wnd;
				IGuiWnd* wnd = ctl->GetWnd();
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
			ctl->SetCapture();
			m_MouseDown = TRUE;
			break;
		case WM_LBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			m_MouseDown = FALSE;
			ctl->ReleaseCapture();
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
				l = rc_wnd.Left() + GUI_IMG(line_left)->GetWidth();
				t = rc_wnd.Top() + GUI_IMG(line_top)->GetHeight() + 115;
				r = rc_wnd.Right() - GUI_IMG(line_right)->GetWidth();
				b = t + GUI_IMG(tag_bg)->GetHeight();
				ctl->SetWindowRect(CRect(l, t, r, b));
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
				l = rc_wnd.Left() + GUI_IMG(line_left)->GetWidth();
				t = rc_wnd.Bottom() - 
					GUI_IMG(line_bottom)->GetHeight() - 
					GUI_IMG(toolbar_bg)->GetHeight();
				r = rc_wnd.Right() - GUI_IMG(line_right)->GetWidth();
				b = t + GUI_IMG(toolbar_bg)->GetHeight();
				ctl->SetWindowRect(CRect(l, t, r, b));
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

protected:
	CGC gc;

	IGuiCtl* m_OldBtn;

	CListT<CImage*> m_list_ico;
	CListT<CString> m_list_str;

	BOOL m_Loaded;

	void LoadDirImage(CListT<CImage*>& lstImage, CListT<CString>& lstString, LPCTSTR sPath, ICoderObject* pCoder, CGC* pGC)
	{
		if (!sPath || !pCoder || !pGC) return;
		CString path(sPath); path += _T("\\");
		CString path_find(path); path_find += _T("*");

		CIOFile file;
		IFileObject* old_file = pCoder->GetFile();
		pCoder->SetFile(&file);

		WIN32_FIND_DATA fd = {0};
		HANDLE find = ::FindFirstFile(path_find, &fd);
		if (find != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (0 == wcscmp(fd.cFileName, L".") || 
					0 == wcscmp(fd.cFileName, L"..")) continue;
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					LoadDirImage(lstImage, lstString, path + fd.cFileName, pCoder, pGC);
				else
				{
					file.Open(path + fd.cFileName);
					image_t img = pCoder->Decode();
					if (img)
					{
						CImage* image = gcnew(*pGC, CImage);
						image->Set(img);
						lstImage.Add(image);
						lstString.Add(fd.cFileName);
					}
				}
			} while (::FindNextFile(find, &fd));
			::FindClose(find);
		}

		pCoder->SetFile(old_file);
	}
	
public:
	CEvent_list()
		: m_OldBtn(NULL)
		, m_Loaded(FALSE)
	{
		ICoderObject* coder = CImgAnalyzer::GetCoder(CImgAnalyzer::png, &gc);
		if (!coder) return;
		CIOFile file;
		coder->SetFile(&file);
		LoadDirImage(m_list_ico, m_list_str, CString(IApp::GetPath()) + _T("icon"), coder, &gc);
	}

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
				if(!m_Loaded)
				{
					m_Loaded = TRUE;
					IGuiCtl::items_t items_list;
					CListT<CImage*>::iterator_t ite_ico = m_list_ico.Head();
					CListT<CString>::iterator_t ite_str = m_list_str.Head();
					for(; ite_ico != m_list_ico.Tail() && ite_str != m_list_str.Tail(); ++ite_ico, ++ite_str)
					{
						IGuiCtl* btn = ExGui<IGuiCtl>(_T("CGuiLVItem"));
						btn->Execute(_T("image"), _T("\
							list_cor_lt,\
							list_item_top,\
							list_cor_rt,\
							list_item_left,\
							list_item,\
							list_item_right,\
							list_cor_lb,\
							list_item_bottom,\
							list_cor_rb"));
						btn->Execute(_T("font"), _T("\
							txt_btn,txt_btn,txt_btn,txt_btn,\
							txt_btn,txt_btn,txt_btn,txt_btn"));
						btn->SetState(_T("icon"), *ite_ico);
						btn->SetState(_T("text"), &(*ite_str));
						btn->SetState(_T("locate"), (void*)1);
						btn->SetState(_T("loc_off"), (void*)5);
						btn->SetState(_T("ico_off"), (void*)5);
						btn->SetState(_T("shake_ico"), (void*)1);
						btn->SetWindowRect(CRect(0, 0, 80, 90));
						items_list.Add(btn);
					}
					GUI_CTL(list)->SetState(_T("items"), &items_list);
					items_list.HeadItem()->SetFocus();
				}
				CRect rc_wnd;
				IGuiWnd* wnd = ctl->GetWnd();
				ExAssert(wnd);
				wnd->GetClientRect(rc_wnd);
				rc_wnd.Inflate(CPoint(1, 1));

				LONG l, r, t, b;
				l = rc_wnd.Left() + GUI_IMG(line_left)->GetWidth();
				t = rc_wnd.Top() + GUI_IMG(line_top)->GetHeight() + GUI_IMG(banner)->GetHeight();
				r = rc_wnd.Right() - GUI_IMG(line_right)->GetWidth();
				b = rc_wnd.Bottom() - GUI_IMG(line_bottom)->GetHeight() - GUI_IMG(toolbar_bg)->GetHeight();
				if (GUI_CTL(scr_h)->IsVisible())
				{
					CRect rc_scr;
					GUI_CTL(scr_h)->GetWindowRect(rc_scr);
					r -= rc_scr.Width();
				}
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case WM_COMMAND:
			if (wParam == BN_CLICKED)
			{
				IGuiCtl* btn = (IGuiCtl*)lParam;
				if (m_OldBtn == btn) break;

				CText* text = ((CText**)btn->GetState(_T("font")))[0];
				if (!text) break;
				CString* str = (CString*)btn->GetState(_T("text"));
				CSize txt_clp;
				text->GetSize(*str, txt_clp);

				CRect rc_btn;
				btn->GetWindowRect(rc_btn);
				if (txt_clp.cx <= ((rc_btn.Width() - 4) << 1)) break;
				rc_btn.pt2.y += 14;
				btn->SetWindowRect(rc_btn);
				m_OldBtn = btn;
			}
			else
			if (wParam == BN_KILLFOCUS)
			{
				IGuiCtl* btn = (IGuiCtl*)lParam;
				if (m_OldBtn != btn) break;

				CText* text = ((CText**)btn->GetState(_T("font")))[0];
				if (!text) break;
				CString* str = (CString*)btn->GetState(_T("text"));
				CSize txt_clp;
				text->GetSize(*str, txt_clp);

				CRect rc_btn;
				btn->GetWindowRect(rc_btn);
				if (txt_clp.cx <= ((rc_btn.Width() - 4) << 1)) break;
				rc_btn.pt2.y -= 14;
				btn->SetWindowRect(rc_btn);
				m_OldBtn = NULL;
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_list, IGuiEvent)

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
				l = rc_wnd.Right() - GUI_IMG(line_right)->GetWidth() - 
					(GUI_IMG(scr_h)->IsNull() ? 20 : GUI_IMG(scr_h)->GetWidth());
				t = rc_wnd.Top() + GUI_IMG(line_top)->GetHeight() + GUI_IMG(banner)->GetHeight();
				r = rc_wnd.Right() - GUI_IMG(line_right)->GetWidth();
				b = rc_wnd.Bottom() - GUI_IMG(line_bottom)->GetHeight() - GUI_IMG(toolbar_bg)->GetHeight();
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_scr_h, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_win_sysbtn_close : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_win_sysbtn_close, IGuiEvent)

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
				l = rc_wnd.Right() - 
					GUI_IMG(line_right)->GetWidth() - 
					GUI_IMG(win_sysbtn_close)->GetWidth() - 4;
				t = rc_wnd.Top() + GUI_IMG(line_top)->GetHeight() - 1;
				r = l + GUI_IMG(win_sysbtn_close)->GetWidth();
				b = t + GUI_IMG(win_sysbtn_close)->GetHeight() / 3;
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case BM_CLICK:
			{
				IGuiWnd* wnd = ctl->GetWnd();
				if (!wnd) return;
				wnd->PostMessage(WM_CLOSE);
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
				l = rc_wnd.Right() - 
					GUI_IMG(line_right)->GetWidth() - 
					GUI_IMG(win_sysbtn_close)->GetWidth() - 
					GUI_IMG(win_sysbtn_max)->GetWidth() - 4;
				t = rc_wnd.Top() + GUI_IMG(line_top)->GetHeight() - 1;
				r = l + GUI_IMG(win_sysbtn_max)->GetWidth();
				b = t + GUI_IMG(win_sysbtn_max)->GetHeight() / 3;
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			// 判断窗口
			{
				IGuiWnd* wnd = ctl->GetWnd();
				if (!wnd) return;
				ctl->SetVisible(!::IsZoomed(wnd->GethWnd()));
				GUI_CTL(win_sysbtn_restore)->SetVisible(::IsZoomed(wnd->GethWnd()));
			}
			break;
		case BM_CLICK:
			{
				IGuiWnd* wnd = ctl->GetWnd();
				if (!wnd) return;
				// 先最大化窗口
				if (!::IsZoomed(wnd->GethWnd()))
					wnd->ShowWindow(SW_SHOWMAXIMIZED);
				// 重新设置窗口大小
				RECT rect;
				::GetWindowRect(wnd->GethWnd(), &rect);
				HMONITOR monitor = ::MonitorFromRect(&rect, MONITOR_DEFAULTTONEAREST);
				MONITORINFO mi = {0};
				mi.cbSize = sizeof(mi);
				::GetMonitorInfo(monitor, &mi);
				if (memcmp(&rect, &(mi.rcWork), sizeof(RECT)) != 0)
					wnd->MoveWindow(CRect(mi.rcWork));
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
				l = rc_wnd.Right() - 
					GUI_IMG(line_right)->GetWidth() - 
					GUI_IMG(win_sysbtn_close)->GetWidth() - 
					GUI_IMG(win_sysbtn_restore)->GetWidth() - 4;
				t = rc_wnd.Top() + GUI_IMG(line_top)->GetHeight() - 1;
				r = l + GUI_IMG(win_sysbtn_restore)->GetWidth();
				b = t + GUI_IMG(win_sysbtn_restore)->GetHeight() / 3;
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			// 判断窗口
			{
				IGuiWnd* wnd = ctl->GetWnd();
				if (!wnd) return;
				ctl->SetVisible(::IsZoomed(wnd->GethWnd()));
				GUI_CTL(win_sysbtn_max)->SetVisible(!::IsZoomed(wnd->GethWnd()));
			}
			break;
		case BM_CLICK:
			{
				IGuiWnd* wnd = ctl->GetWnd();
				if (!wnd) return;
				// 还原窗口
				if (::IsZoomed(wnd->GethWnd()))
					wnd->ShowWindow(SW_SHOWNORMAL);
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

				LONG w_dec = 
					::IsZoomed(wnd->GethWnd()) ? 
					GUI_IMG(win_sysbtn_restore)->GetWidth() : 
					GUI_IMG(win_sysbtn_max)->GetWidth();

				LONG l, r, t, b;
				l = rc_wnd.Right() - 
					GUI_IMG(line_right)->GetWidth() - 
					GUI_IMG(win_sysbtn_close)->GetWidth() - w_dec - 
					GUI_IMG(win_sysbtn_min)->GetWidth() - 4;
				t = rc_wnd.Top() + GUI_IMG(line_top)->GetHeight() - 1;
				r = l + GUI_IMG(win_sysbtn_min)->GetWidth();
				b = t + GUI_IMG(win_sysbtn_min)->GetHeight() / 3;
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case BM_CLICK:
			{
				IGuiWnd* wnd = ctl->GetWnd();
				if (!wnd) return;
				// 最小化窗口
				wnd->ShowWindow(SW_SHOWMINIMIZED);
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

protected:
	BOOL m_Loaded;

public:
	CEvent_topbar_btn() : m_Loaded(FALSE) {}
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (wParam)
			{
				if(!m_Loaded)
				{
					m_Loaded = TRUE;
					IGuiCtl::items_t* items = (IGuiCtl::items_t*)ctl->GetState(_T("items"));
					items->GetAt(0)->SendMessage(BM_CLICK);
				}

				CRect rc_wnd;
				IGuiWnd* wnd = ctl->GetWnd();
				ExAssert(wnd);
				wnd->GetClientRect(rc_wnd);
				rc_wnd.Inflate(CPoint(1, 1));

				LONG l, r, t, b;
				l = rc_wnd.Left() + GUI_IMG(line_left)->GetWidth();
				t = rc_wnd.Top() + 
					GUI_IMG(line_top)->GetHeight() + 
					GUI_IMG(banner)->GetHeight() - 
					GUI_IMG(tag_bg)->GetHeight() - 
					GUI_IMG(topbar_btn)->GetHeight() / 4 - 8;
				r = l + GUI_IMG(topbar_btn)->GetWidth();
				b = t + GUI_IMG(topbar_btn)->GetHeight() / 4;
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_topbar_btn, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_items_topbar_btn : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_items_topbar_btn, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		switch( nMessage )
		{
		case BM_CLICK:
			{
				IGuiComp* pare = ctl->GetParent();	
				for(IGuiComp::list_t::iterator_t ite = pare->GetComp().Head(); ite != pare->GetComp().Tail(); ++ite)
				{
					IGuiCtl* c = ExDynCast<IGuiCtl>(*ite);
					if (c == ctl) continue;
					c->SetState(_T("status"), (void*)0);
				}
				ctl->SetState(_T("status"), (void*)3);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_items_topbar_btn, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_gamesearch_charmap : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_gamesearch_charmap, IGuiEvent)

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
				l = rc_wnd.Left() + 
					GUI_IMG(line_left)->GetWidth() + 
					GUI_IMG(search_bg)->GetWidth() + 20 + 
					GUI_IMG(search_button)->GetWidth();
				t = rc_wnd.Bottom() - 
					GUI_IMG(line_bottom)->GetHeight() - 
					GUI_IMG(gamesearch_charmap)->GetHeight() / 3 - 2;
				r = l + GUI_IMG(gamesearch_charmap)->GetWidth();
				b = t + GUI_IMG(gamesearch_charmap)->GetHeight() / 3;
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_gamesearch_charmap, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_toolbar_tools : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_toolbar_tools, IGuiEvent)

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
				l = rc_wnd.Right() - 
					GUI_IMG(line_right)->GetWidth() - 
					GUI_IMG(toolbar_tools)->GetWidth() - 20;
				t = rc_wnd.Bottom() - 
					GUI_IMG(line_bottom)->GetHeight() - 
					GUI_IMG(toolbar_tools)->GetHeight() / 3;
				r = l + GUI_IMG(toolbar_tools)->GetWidth();
				b = t + GUI_IMG(toolbar_tools)->GetHeight() / 3;
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_toolbar_tools, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_search_bg : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_search_bg, IGuiEvent)

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
				l = rc_wnd.Left() + 
					GUI_IMG(line_left)->GetWidth() + 15;
				t = rc_wnd.Bottom() - 
					GUI_IMG(line_bottom)->GetHeight() - 
					GUI_IMG(search_bg)->GetHeight() - 3;
				r = l + GUI_IMG(search_bg)->GetWidth();
				b = t + GUI_IMG(search_bg)->GetHeight();
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_search_bg, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_search_button : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_search_button, IGuiEvent)

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
				l = rc_wnd.Left() + 
					GUI_IMG(line_left)->GetWidth() + 15 + 
					GUI_IMG(search_bg)->GetWidth();
				t = rc_wnd.Bottom() - 
					GUI_IMG(line_bottom)->GetHeight() - 
					GUI_IMG(search_button)->GetHeight() - 3;
				r = l + GUI_IMG(search_button)->GetWidth();
				b = t + GUI_IMG(search_button)->GetHeight();
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_search_button, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_google_bg : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_google_bg, IGuiEvent)

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
				l = rc_wnd.Right() - 
					GUI_IMG(line_right)->GetWidth() - 
					GUI_IMG(google_button)->GetWidth() - 
					GUI_IMG(google_bg)->GetWidth() - 5;
				t = rc_wnd.Top() + GUI_IMG(line_top)->GetHeight() + 120;
				r = l + GUI_IMG(google_bg)->GetWidth();
				b = t + GUI_IMG(google_bg)->GetHeight();
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_google_bg, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_google_button : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_google_button, IGuiEvent)

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
				l = rc_wnd.Right() - 
					GUI_IMG(line_right)->GetWidth() - 
					GUI_IMG(google_button)->GetWidth() - 5;
				t = rc_wnd.Top() + GUI_IMG(line_top)->GetHeight() + 120;
				r = l + GUI_IMG(google_button)->GetWidth();
				b = t + GUI_IMG(google_button)->GetHeight();
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_google_button, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_tag_qb : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_tag_qb, IGuiEvent)

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
				l = rc_wnd.Left() + GUI_IMG(line_left)->GetWidth() + 5;
				t = rc_wnd.Top() + 
					GUI_IMG(line_top)->GetHeight() + 
					GUI_IMG(tag_bg)->GetHeight() - 
					GUI_IMG(tag_qb)->GetHeight() / 4 + 115;
				r = l + GUI_IMG(tag_qb)->GetWidth();
				b = t + GUI_IMG(tag_qb)->GetHeight() / 4;
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case BM_CLICK:
			{
				GUI_CTL(tag_qb)->SetState(_T("status"), (void*)3);
				GUI_CTL(tag_zx)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_wl)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_dz)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_wy)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_dj)->SetState(_T("status"), (void*)0);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_tag_qb, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_tag_zx : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_tag_zx, IGuiEvent)

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

				CRect rc_ctl;
				GUI_CTL(tag_qb)->GetWindowRect(rc_ctl);

				LONG l, r, t, b;
				l = rc_ctl.Right() + 5;
				t = rc_wnd.Top() + 
					GUI_IMG(line_top)->GetHeight() + 
					GUI_IMG(tag_bg)->GetHeight() - 
					GUI_IMG(tag_qb)->GetHeight() / 4 + 115;
				r = l + GUI_IMG(tag_qb)->GetWidth();
				b = t + GUI_IMG(tag_qb)->GetHeight() / 4;
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case BM_CLICK:
			{
				GUI_CTL(tag_qb)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_zx)->SetState(_T("status"), (void*)3);
				GUI_CTL(tag_wl)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_dz)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_wy)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_dj)->SetState(_T("status"), (void*)0);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_tag_zx, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_tag_wl : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_tag_wl, IGuiEvent)

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

				CRect rc_ctl;
				GUI_CTL(tag_zx)->GetWindowRect(rc_ctl);

				LONG l, r, t, b;
				l = rc_ctl.Right() + 5;
				t = rc_wnd.Top() + 
					GUI_IMG(line_top)->GetHeight() + 
					GUI_IMG(tag_bg)->GetHeight() - 
					GUI_IMG(tag_qb)->GetHeight() / 4 + 115;
				r = l + GUI_IMG(tag_qb)->GetWidth();
				b = t + GUI_IMG(tag_qb)->GetHeight() / 4;
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case BM_CLICK:
			{
				GUI_CTL(tag_qb)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_zx)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_wl)->SetState(_T("status"), (void*)3);
				GUI_CTL(tag_dz)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_wy)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_dj)->SetState(_T("status"), (void*)0);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_tag_wl, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_tag_dz : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_tag_dz, IGuiEvent)

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

				CRect rc_ctl;
				GUI_CTL(tag_wl)->GetWindowRect(rc_ctl);

				LONG l, r, t, b;
				l = rc_ctl.Right() + 5;
				t = rc_wnd.Top() + 
					GUI_IMG(line_top)->GetHeight() + 
					GUI_IMG(tag_bg)->GetHeight() - 
					GUI_IMG(tag_qb)->GetHeight() / 4 + 115;
				r = l + GUI_IMG(tag_qb)->GetWidth();
				b = t + GUI_IMG(tag_qb)->GetHeight() / 4;
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case BM_CLICK:
			{
				GUI_CTL(tag_qb)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_zx)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_wl)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_dz)->SetState(_T("status"), (void*)3);
				GUI_CTL(tag_wy)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_dj)->SetState(_T("status"), (void*)0);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_tag_dz, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_tag_wy : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_tag_wy, IGuiEvent)

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

				CRect rc_ctl;
				GUI_CTL(tag_dz)->GetWindowRect(rc_ctl);

				LONG l, r, t, b;
				l = rc_ctl.Right() + 5;
				t = rc_wnd.Top() + 
					GUI_IMG(line_top)->GetHeight() + 
					GUI_IMG(tag_bg)->GetHeight() - 
					GUI_IMG(tag_qb)->GetHeight() / 4 + 115;
				r = l + GUI_IMG(tag_qb)->GetWidth();
				b = t + GUI_IMG(tag_qb)->GetHeight() / 4;
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case BM_CLICK:
			{
				GUI_CTL(tag_qb)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_zx)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_wl)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_dz)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_wy)->SetState(_T("status"), (void*)3);
				GUI_CTL(tag_dj)->SetState(_T("status"), (void*)0);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_tag_wy, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CEvent_tag_dj : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CEvent_tag_dj, IGuiEvent)

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

				CRect rc_ctl;
				GUI_CTL(tag_wy)->GetWindowRect(rc_ctl);

				LONG l, r, t, b;
				l = rc_ctl.Right() + 5;
				t = rc_wnd.Top() + 
					GUI_IMG(line_top)->GetHeight() + 
					GUI_IMG(tag_bg)->GetHeight() - 
					GUI_IMG(tag_qb)->GetHeight() / 4 + 115;
				r = l + GUI_IMG(tag_qb)->GetWidth();
				b = t + GUI_IMG(tag_qb)->GetHeight() / 4;
				ctl->SetWindowRect(CRect(l, t, r, b));
			}
			break;
		case BM_CLICK:
			{
				GUI_CTL(tag_qb)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_zx)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_wl)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_dz)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_wy)->SetState(_T("status"), (void*)0);
				GUI_CTL(tag_dj)->SetState(_T("status"), (void*)3);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CEvent_tag_dj, IGuiEvent)

//////////////////////////////////////////////////////////////////
