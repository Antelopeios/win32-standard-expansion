// Copyright 2011, ľͷ��
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////////////
// GuiBoard - �����ͼ��(Win32���ڶ���)
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-06-17
// Version:	1.0.0003.0942
//
// History:
//	- 1.0.0002.1525(2011-05-13)	^ ��IGuiBoardBase�ӿ�ʵ����GuiBoard��ʵ�ַ���
//	- 1.0.0003.0942(2011-06-17)	= ��IGuiBoardBase�ӿ������ƻص�GuiBoard.cpp��
//////////////////////////////////////////////////////////////////

#include "GuiCommon/GuiCommon.h"
#include "GuiBoard.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI ���ڶ���ӿ�
EXP_IMPLEMENT_DYNAMIC_MULT(IGuiBoard, IGuiBase)

//////////////////////////////////////////////////////////////////

// GUI ���ڶ���
EXP_IMPLEMENT_DYNAMIC_MULT(IGuiBoardBase, IGuiBoard)
const LPCTSTR IGuiBoardBase::s_ClassName = _T("GuiExp_Foundation");

IGuiBoardBase::IGuiBoardBase(void)
	: m_hIns(::GetModuleHandle(NULL))
	, m_bLayered(false)
{}
IGuiBoardBase::IGuiBoardBase(wnd_t hWnd)
	: m_hIns(::GetModuleHandle(NULL))
	, type_base_t(hWnd)
{}
IGuiBoardBase::~IGuiBoardBase(void)
{}

ATOM IGuiBoardBase::RegisterWndClass(LPCTSTR sClassName)
{
	WNDCLASSEX wcex		= {0};
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= DefWindowProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hIns;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= sClassName;
	return ::RegisterClassEx(&wcex);
}

bool IGuiBoardBase::Create(LPCTSTR sWndName, CRect& rcWnd, 
			int nCmdShow/* = SW_SHOWNORMAL*/, DWORD dwStyle/* = WS_POPUP*/, DWORD dwExStyle/* = NULL*/, 
			wnd_t wndParent/* = NULL*/)
{
	RegisterWndClass(s_ClassName);
	Attach
		(
		::CreateWindowEx
			(
			dwExStyle, s_ClassName, sWndName, dwStyle, 
			rcWnd.Left(), rcWnd.Top(), rcWnd.Width(), rcWnd.Height(), 
			wndParent, NULL, m_hIns, NULL
			)
		);
	if( IsNull() ) return false;

	ShowWindow(nCmdShow);
	UpdateWindow();

	return true;
}

bool IGuiBoardBase::IsNull() const
{
	return type_base_t::IsNull();
}

wnd_t IGuiBoardBase::operator=(wnd_t tType)
{
	Set(tType);
	return Get();
}

bool IGuiBoardBase::Attach(wnd_t hWnd)
{
	Set(hWnd);
	return true;
}
wnd_t IGuiBoardBase::Detach()
{
	wnd_t old_wnd = Get();
	Set(NULL);
	return old_wnd;
}
wnd_t IGuiBoardBase::GethWnd()
{
	return Get();
}

// ������Ϣ
LRESULT IGuiBoardBase::SendMessage(UINT nMessage, WPARAM wParam/* = 0*/, LPARAM lParam/* = 0*/)
{
	return ::SendMessage(Get(), nMessage, wParam, lParam);
}
bool IGuiBoardBase::PostMessage(UINT nMessage, WPARAM wParam/* = 0*/, LPARAM lParam/* = 0*/)
{
	return ::PostMessage(Get(), nMessage, wParam, lParam);
}

// ���������޸�
DWORD IGuiBoardBase::GetStyle()
{
	return (DWORD)GetWindowLong(GWL_STYLE);
}
DWORD IGuiBoardBase::GetExStyle()
{
	return (DWORD)GetWindowLong(GWL_EXSTYLE);
}
bool IGuiBoardBase::ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags/* = 0*/)
{
	return ModifyStyleEx(dwRemove, dwAdd, nFlags, GWL_STYLE);
}
bool IGuiBoardBase::ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags/* = 0*/, int nStyleOffset/* = GWL_EXSTYLE*/)
{
	if (IsNull()) return 0;
	DWORD style = GetWindowLong(nStyleOffset);
	DWORD new_style = (style & ~dwRemove) | dwAdd;
	if (style == new_style)
		return false;
	SetWindowLong(nStyleOffset, new_style);
	if (nFlags != 0)
	{
		::SetWindowPos(Get(), NULL, 0, 0, 0, 0,
			SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
	}
	return true;
}
LONG IGuiBoardBase::SetWindowLong(int nIndex, LONG dwNewLong)
{
	if (IsNull()) return 0;
	return ::SetWindowLong(Get(), nIndex, dwNewLong);
}
LONG IGuiBoardBase::GetWindowLong(int nIndex)
{
	if (IsNull()) return 0;
	return ::GetWindowLong(Get(), nIndex);
}

// �����ƶ�
void IGuiBoardBase::MoveWindow(int x, int y, int nWidth, int nHeight, bool bRepaint/* = true*/)
{
	if (IsNull()) return;
	::MoveWindow(Get(), x, y, nWidth, nHeight, bRepaint);
}
void IGuiBoardBase::MoveWindow(CRect& lpRect, bool bRepaint/* = true*/)
{
	if (IsNull()) return;
	::MoveWindow
		(
		Get(), 
		lpRect.Left(), 
		lpRect.Top(), 
		lpRect.Width(), 
		lpRect.Height(), 
		bRepaint
		);
}
void IGuiBoardBase::CenterWindow(wnd_t hWndCenter/* = NULL*/)
{
	if (IsNull()) return;

	// determine owner window to center against
	DWORD dwStyle = GetStyle();
	if (hWndCenter == NULL)
	{
		if (dwStyle & WS_CHILD)
			hWndCenter = ::GetParent(Get());
		else
			hWndCenter = ::GetWindow(Get(), GW_OWNER);
		if (hWndCenter != NULL)
		{
			// let parent determine alternate center window
			wnd_t hWndTemp =
				(wnd_t)::SendMessage(hWndCenter, 0x036B/*WM_QUERYCENTERWND*/, 0, 0);
			if (hWndTemp != NULL)
				hWndCenter = hWndTemp;
		}
	}

	// get coordinates of the window relative to its parent
	CRect rcDlg;
	GetWindowRect(rcDlg);
	CRect rcArea;
	CRect rcCenter;
	wnd_t hWndParent;
	if (!(dwStyle & WS_CHILD))
	{
		// don't center against invisible or minimized windows
		if (hWndCenter != NULL)
		{
			DWORD dwAlternateStyle = ::GetWindowLong(hWndCenter, GWL_STYLE);
			if (!(dwAlternateStyle & WS_VISIBLE) || (dwAlternateStyle & WS_MINIMIZE))
				hWndCenter = NULL;
		}

		MONITORINFO mi;
		mi.cbSize = sizeof(mi);

		// center within appropriate monitor coordinates
		if (hWndCenter == NULL)
		{
			wnd_t hwDefault = GetDesktopWindow();

			GetMonitorInfo(MonitorFromWindow(hwDefault, MONITOR_DEFAULTTOPRIMARY), &mi);
			rcCenter = mi.rcWork;
			rcArea	 = mi.rcWork;
		}
		else
		{
			RECT rc = {0, 0, 0, 0};
			::GetWindowRect(hWndCenter, &rc);
			rcCenter = rc;
			GetMonitorInfo(MonitorFromWindow(hWndCenter, MONITOR_DEFAULTTONEAREST), &mi);
			rcArea = mi.rcWork;
		}
	}
	else
	{
		// center within parent client coordinates
		hWndParent = GetParent();
		ExAssert(::IsWindow(hWndParent));
		RECT rc = {0, 0, 0, 0};
		::GetClientRect(hWndParent, &rc);
		rcArea = rc;
		ExAssert(::IsWindow(hWndCenter));
		::GetClientRect(hWndCenter, &rc);
		rcCenter = rc;
		::MapWindowPoints(hWndCenter, hWndParent, (POINT*)&rcCenter, 2);
	}

	// find dialog's upper left based on rcCenter
	int xLeft = (rcCenter.Left() + rcCenter.Right()) / 2 - rcDlg.Width() / 2;
	int yTop = (rcCenter.Top() + rcCenter.Bottom()) / 2 - rcDlg.Height() / 2;

	// if the dialog is outside the screen, move it inside
	if (xLeft + rcDlg.Width() > rcArea.Right())
		xLeft = rcArea.Right() - rcDlg.Width();
	if (xLeft < rcArea.Left())
		xLeft = rcArea.Left();

	if (yTop + rcDlg.Height() > rcArea.Bottom())
		yTop = rcArea.Bottom() - rcDlg.Height();
	if (yTop < rcArea.Top())
		yTop = rcArea.Top();

	// map screen coordinates to child coordinates
	::SetWindowPos
		(
		Get(), NULL, 
		xLeft, yTop, -1, -1,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE
		);
}

// ��������ת��
void IGuiBoardBase::ClientToScreen(CPoint& lpPoint)
{
	if (IsNull()) return;
	::ClientToScreen(Get(), (LPPOINT)&lpPoint);
}
void IGuiBoardBase::ClientToScreen(CRect& lpRect)
{
	if (IsNull()) return;
	CPoint pt(lpRect.pt1);
	ClientToScreen(pt);
	lpRect.pt2.x += (pt.x - lpRect.pt1.x);
	lpRect.pt2.y += (pt.y - lpRect.pt1.y);
	lpRect.pt1.x = pt.x;
	lpRect.pt1.y = pt.y;
}
void IGuiBoardBase::ScreenToClient(CPoint& lpPoint)
{
	if (IsNull()) return;
	::ScreenToClient(Get(), (LPPOINT)&lpPoint);
}
void IGuiBoardBase::ScreenToClient(CRect& lpRect)
{
	if (IsNull()) return;
	CPoint pt(lpRect.pt1);
	ScreenToClient(pt);
	lpRect.pt2.x += (pt.x - lpRect.pt1.x);
	lpRect.pt2.y += (pt.y - lpRect.pt1.y);
	lpRect.pt1.x = pt.x;
	lpRect.pt1.y = pt.y;
}

// ����ˢ��
void IGuiBoardBase::Invalidate()
{
	if (IsNull()) return ;
	RECT rect = {0, 0, 0, 0};
	::GetClientRect(Get(), &rect);
	::InvalidateRect(Get(), &rect, TRUE);
}
void IGuiBoardBase::InvalidateRect(CRect& rcInv)
{
	if (IsNull()) return ;
	RECT rect = 
	{
		rcInv.Left(), 
		rcInv.Top(), 
		rcInv.Right(), 
		rcInv.Bottom()
	};
	::InvalidateRect(Get(), &rect, TRUE);
}
void IGuiBoardBase::InvalidateRgn(HRGN hRgn)
{
	if (IsNull()) return ;
	::InvalidateRgn(Get(), hRgn, TRUE);
}
bool IGuiBoardBase::ShowWindow(int nCmdShow)
{
	if(IsNull()) return false;
	return (bool)::ShowWindow(Get(), nCmdShow);
}
bool IGuiBoardBase::UpdateWindow()
{
	if (IsNull()) return false;
	return (bool)::UpdateWindow(Get());
}

// ����DC
graph_t IGuiBoardBase::GetDC()
{
	if(IsNull()) return NULL;
	return (graph_t)::GetDC(Get());
}
bool IGuiBoardBase::ReleaseDC(graph_t hdc)
{
	if(IsNull()) return NULL;
	return ::ReleaseDC(Get(), (HDC)hdc);
}

// ��ô��ڴ�С
bool IGuiBoardBase::GetWindowRect(CRect& lpRect)
{
	if (IsNull()) return false;
	return (bool)::GetWindowRect(Get(), (LPRECT)&lpRect);
}
bool IGuiBoardBase::GetClientRect(CRect& lpRect)
{
	if (IsNull()) return false;
	return (bool)::GetClientRect(Get(), (LPRECT)&lpRect);
}

// ���ڹ�ϵ����
wnd_t IGuiBoardBase::GetParent()
{
	if (IsNull()) return NULL;
	return (wnd_t)::GetParent(Get());
}

// ���ý���
wnd_t IGuiBoardBase::SetFocus()
{
	return (wnd_t)::SetFocus(Get());
}
bool IGuiBoardBase::IsFocus()
{
	wnd_t hwnd = Get();
	if( !hwnd ) return false;
	wnd_t hfocwnd = GetFocus();
	if( !hfocwnd ) return false;
	wnd_t hfoc = hfocwnd;
	while (hfoc && hwnd != hfoc) hfoc = (wnd_t)::GetParent(hfoc);
	if( hfoc )
		return true;
	else
	{
		hfoc = hfocwnd;
		while (hwnd && hwnd != hfoc) hwnd = (wnd_t)::GetParent(hwnd);
		return hwnd ? true : false;
	}
}

// ����ͼ�㻯
void IGuiBoardBase::SetLayered(bool bLayered/* = true*/)
{
	m_bLayered = bLayered;
	Invalidate();
}
bool IGuiBoardBase::IsLayered()
{
	return m_bLayered;
}

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED	0x00080000
#endif

void IGuiBoardBase::LayeredWindow(HDC hDC, HDC tGrp)
{
	if (!hDC || !tGrp) return;

	if (m_bLayered)
	{
		DWORD ex_style = GetWindowLong(GWL_EXSTYLE);
		if((ex_style & WS_EX_LAYERED) != WS_EX_LAYERED)
			SetWindowLong(GWL_EXSTYLE, ex_style ^ WS_EX_LAYERED);

		CRect rect;
		GetWindowRect(rect);

		POINT pt_wnd = {rect.Left(), rect.Top()};
		SIZE  sz_wnd = {rect.Width(), rect.Height()};
		POINT pt_src = {0, 0};

		BLENDFUNCTION blend		  = {0};
		blend.AlphaFormat		  = AC_SRC_ALPHA;
		blend.SourceConstantAlpha = EXP_CM;
		::UpdateLayeredWindow(Get(), hDC, &pt_wnd, &sz_wnd, tGrp, &pt_src, 0, &blend, 2);
	}
	else
	{
		DWORD ex_style = GetWindowLong(GWL_EXSTYLE);
		if (ex_style | WS_EX_LAYERED)
			SetWindowLong(GWL_EXSTYLE, ex_style & ~WS_EX_LAYERED);

		CRect rect;
		GetClientRect(rect);

		::BitBlt(hDC, rect.Left(), rect.Top(), rect.Width(), rect.Height(), tGrp, 0, 0, SRCCOPY);
	}
}

//////////////////////////////////////////////////////////////////

EXP_END

#include "GuiBoard/GuiThunk.hpp"
#include "GuiBoard/GuiWnd.hpp"

//////////////////////////////////////////////////////////////////
