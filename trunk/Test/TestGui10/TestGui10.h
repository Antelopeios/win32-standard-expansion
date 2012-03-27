#pragma once

//////////////////////////////////////////////////////////////////

class CPrgEvent : public IGuiEvent
{
public:
	int m_Off;

public:
	CPrgEvent()
		: m_Off(0)
	{}

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		switch( nMessage )
		{
		case WM_TIMER:
			if (wParam == 1)
			{
				UINT val = (UINT)ctl->GetState(_T("val"));
				UINT max = (UINT)ctl->GetState(_T("max"));
				ctl->SetState(_T("val"), (void*)(val += m_Off));
				if (m_Off > 0 && val >= max) m_Off = -m_Off;
				else
				if (m_Off < 0 && val == 0) m_Off = -m_Off;
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CWndEvent : public IGuiEvent
{
public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiWnd* wnd = ExDynCast<IGuiWnd>(pGui);
		if (!wnd) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
			::SetTimer(wnd->GethWnd(), 1, 40, NULL);
		case WM_SIZE:
			{
				CRect rc_wnd, rc;
				wnd->GetClientRect(rc_wnd);
				rc.Width(rc_wnd.Width() - 40);
				rc.Height(25);
				rc.MoveTo(CPoint(
					(rc_wnd.Width() - rc.Width()) >> 1, 
					(rc_wnd.Height() - rc.Height()) >> 1));
				rc.Offset(CPoint(0, -(rc.Height() + 20)));
				ExGet<IGuiCtl>(_T("prog1"))->SetWindowRect(rc);
				rc.Offset(CPoint(0, rc.Height() + 20));
				ExGet<IGuiCtl>(_T("prog2"))->SetWindowRect(rc);
				rc.Offset(CPoint(0, rc.Height() + 20));
				ExGet<IGuiCtl>(_T("prog3"))->SetWindowRect(rc);
			}
			break;
		}
	}
};