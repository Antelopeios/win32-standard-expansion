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
// Date:	2011-08-26
// Version:	1.0.0006.2050
//
// History:
//	- 1.0.0002.1525(2011-05-13)	^ ��IGuiBoardBase�ӿ�ʵ����GuiBoard��ʵ�ַ���
//	- 1.0.0003.0942(2011-06-17)	= ��IGuiBoardBase�ӿ������ƻص�GuiBoard.cpp��
//	- 1.0.0004.1253(2011-06-24)	+ ���WNDCLASSEX::style���ƽӿ�ʵ��
//	- 1.0.0005.2000(2011-07-16)	= ���ݼ�������ͼ���Ż�����IGuiBoardBase::LayeredWindow()��ʵ��
//	- 1.0.0006.2050(2011-08-26)	# ����һ��IGuiBoardBase::SetLayered()�ڲ�����
//								# �������������Ӵ���ʱ,��ͼ�Ὣ�Ӵ��ڸ��ǵ�����
//////////////////////////////////////////////////////////////////

#include "GuiCommon/GuiCommon.h"
#include "GuiBoard.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI ���ڶ���ӿ�
EXP_IMPLEMENT_DYNAMIC_MULT(IGuiBoard, IGuiBase)

//////////////////////////////////////////////////////////////////

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED	0x00080000
#endif

// GUI ���ڶ���
EXP_IMPLEMENT_DYNAMIC_MULT(IGuiBoardBase, IGuiBoard)
const LPCTSTR IGuiBoardBase::s_ClassName = _T("GuiExp_Foundation");

IGuiBoardBase::IGuiBoardBase(void)
	: m_hIns(::GetModuleHandle(NULL))
	, m_bPaint(true)
	, m_bLayered(false)
	, m_bColorKey(false)
	, m_crKey(ExRGB(255, 0, 255))
{}
IGuiBoardBase::IGuiBoardBase(wnd_t hWnd)
	: m_hIns(::GetModuleHandle(NULL))
	, type_base_t(hWnd)
	, m_bPaint(true)
	, m_bLayered(false)
	, m_bColorKey(false)
	, m_crKey(ExRGB(255, 0, 255))
{}
IGuiBoardBase::~IGuiBoardBase(void)
{}

LRESULT CALLBACK IGuiBoardBase::BoardProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, nMessage, wParam, lParam);
}

ATOM IGuiBoardBase::RegisterWndClass(LPCTSTR sClassName, UINT uStyle)
{
	WNDCLASSEX wcex		= {0};
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= uStyle;
	wcex.lpfnWndProc	= BoardProc;
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
			wnd_t wndParent/* = NULL*/, UINT uStyle/* = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW*/)
{
	RegisterWndClass(s_ClassName, uStyle);
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

// �Ƿ��ͼ
void IGuiBoardBase::SetCusPaint(bool bPaint)
{
	m_bPaint = bPaint;
}
bool IGuiBoardBase::IsCusPaint() const
{
	return m_bPaint;
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
	::InvalidateRect(Get(), &(RECT)rcInv, TRUE);
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
bool IGuiBoardBase::IsVisible() const
{
	if (IsNull()) return false;
	return ::IsWindowVisible(Get());
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
void IGuiBoardBase::SetLayered(bool bLayered/* = true*/, bool bColorKey/* = true*/, pixel_t crKey/* = ExRGB(255, 0, 255)*/)
{
	/*
		�� m_bLayered == true ʱ���� WS_EX_LAYERED
		�ᵼ�µ�һ�� UpdateLayeredWindow ʱ���ڲ���ʾ
	*/
	if (!(m_bLayered = bLayered))
	{
		DWORD ex_style = GetWindowLong(GWL_EXSTYLE);
		if (m_bColorKey = bColorKey)
		{
			if((ex_style & WS_EX_LAYERED) != WS_EX_LAYERED)
				SetWindowLong(GWL_EXSTYLE, ex_style ^ WS_EX_LAYERED);
			::SetLayeredWindowAttributes(Get(), m_crKey = crKey, EXP_CM, LWA_COLORKEY);
		}
		else
		{
			if (ex_style & WS_EX_LAYERED)
				SetWindowLong(GWL_EXSTYLE, ex_style & ~WS_EX_LAYERED);
		}
	}
	Invalidate();
}
bool IGuiBoardBase::IsLayered()
{
	return m_bLayered;
}
bool IGuiBoardBase::IsColorKey()
{
	return m_bColorKey;
}
pixel_t IGuiBoardBase::GetColorKey()
{
	return m_crKey;
}
void IGuiBoardBase::LayeredWindow(HDC hDC, HDC tGrp)
{
	if (!hDC || !tGrp) return;

	DWORD ex_style = GetWindowLong(GWL_EXSTYLE);
	if (m_bLayered)
	{
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
		CRect rect;
		GetClientRect(rect);

		// ������ο�
		HRGN bkg_rgn = ::CreateRectRgnIndirect(&(RECT)rect);
		HWND cld_wnd_fst = ::GetWindow(GethWnd(), GW_CHILD);
		HWND cld_wnd_lst = cld_wnd_fst ? ::GetWindow(cld_wnd_fst, GW_HWNDLAST) : NULL;
		while (cld_wnd_fst 
			&& cld_wnd_fst != cld_wnd_lst)
		{	// �����Ӵ���
			if (::IsWindowVisible(cld_wnd_fst))
			{
				RECT cld_rct;
				::GetWindowRect(cld_wnd_fst, &cld_rct);
				POINT pt = {cld_rct.left, cld_rct.top};
				::ScreenToClient(GethWnd(), &pt);
				cld_rct.right	+= (pt.x - cld_rct.left);
				cld_rct.bottom	+= (pt.y - cld_rct.top);
				cld_rct.left	= pt.x;
				cld_rct.top		= pt.y;
				HRGN cld_rgn = ::CreateRectRgnIndirect(&cld_rct);
				::CombineRgn(bkg_rgn, bkg_rgn, cld_rgn, RGN_DIFF);
				::DeleteObject(cld_rgn);
			}
			cld_wnd_fst = ::GetWindow(cld_wnd_fst, GW_HWNDNEXT);
		}
		if (cld_wnd_lst && 
			::IsWindowVisible(cld_wnd_lst))
		{	// ���һ���Ӵ���
			RECT cld_rct;
			::GetWindowRect(cld_wnd_lst, &cld_rct);
			POINT pt = {cld_rct.left, cld_rct.top};
			::ScreenToClient(GethWnd(), &pt);
			cld_rct.right	+= (pt.x - cld_rct.left);
			cld_rct.bottom	+= (pt.y - cld_rct.top);
			cld_rct.left	= pt.x;
			cld_rct.top		= pt.y;
			HRGN cld_rgn = ::CreateRectRgnIndirect(&cld_rct);
			::CombineRgn(bkg_rgn, bkg_rgn, cld_rgn, RGN_DIFF);
			::DeleteObject(cld_rgn);
		}
		::SelectClipRgn(hDC, bkg_rgn);
		::DeleteObject(bkg_rgn);

		// �οպ��ͼ
		GetClipBox(rect);
		::BitBlt(hDC, rect.Left(), rect.Top(), rect.Width(), rect.Height(), tGrp, 0, 0, SRCCOPY);
	}
}

//////////////////////////////////////////////////////////////////

EXP_END

#include "GuiBoard/GuiThunk.hpp"
#include "GuiBoard/GuiWnd.hpp"

//////////////////////////////////////////////////////////////////
