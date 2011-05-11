// Copyright 2011, 木头云
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
// GuiBoard - 界面绘图板(Win32窗口对象)
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2010-05-11
// Version:	1.0.0001.1420
//
// History:
//	- 1.0.0001.1420(2010-05-11)	+ 添加CGuiWnd::Attach()和CGuiWnd::Detach()接口
//								- 移除一些接口中的ExAssert,直接忽略一些特殊情况
//								= 将CGuiWnd调整为IGuiBoardBase接口类
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuiBoard.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI 窗口对象接口
EXP_IMPLEMENT_DYNAMIC_CLS(IGuiBoard, IGuiComp)

//////////////////////////////////////////////////////////////////

// GUI 窗口对象
interface IGuiBoardBase : public IGuiBoard, public ITypeObjectT<wnd_t>
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiBoardBase, IGuiComp)

	typedef ITypeObjectT<wnd_t> type_base_t;

protected:
	static const CString s_ClassName;
	HINSTANCE m_hIns;

public:
	IGuiBoardBase(void)
		: m_hIns(::GetModuleHandle(NULL))
	{}
	IGuiBoardBase(wnd_t hWnd)
		: m_hIns(::GetModuleHandle(NULL))
		, type_base_t(hWnd)
	{}
	virtual ~IGuiBoardBase(void)
	{}

protected:
	ATOM RegisterWndClass(LPCTSTR sClassName)
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

public:
	bool Create(LPCTSTR sWndName, CRect& rcWnd, 
				int nCmdShow = SW_SHOWNORMAL, DWORD dwStyle = WS_POPUP, DWORD dwExStyle = NULL, 
				wnd_t wndParent = NULL)
	{
		Destroy();

		RegisterWndClass(s_ClassName);
		Attach(
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
	bool Destroy()
	{
		bool ret = true;
		if (!IsNull())
			ret = ::DestroyWindow(Get());
		Detach();
		return ret;
	}
	bool IsNull() const
	{ return type_base_t::IsNull(); }

	bool Attach(wnd_t hWnd)
	{
		Set(hWnd);
		return true;
	}
	wnd_t Detach()
	{
		wnd_t old_wnd = Get();
		Set(NULL);
		return old_wnd;
	}

	// 窗口属性修改
	DWORD GetStyle()
	{
		return (DWORD)GetWindowLong(GWL_STYLE);
	}
	DWORD GetExStyle()
	{
		return (DWORD)GetWindowLong(GWL_EXSTYLE);
	}
	bool ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0)
	{
		return ModifyStyleEx(dwRemove, dwAdd, nFlags, GWL_STYLE);
	}
	bool ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0, int nStyleOffset = GWL_EXSTYLE)
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
	LONG SetWindowLong(int nIndex, LONG dwNewLong)
	{
		if (IsNull()) return 0;
		return ::SetWindowLong(Get(), nIndex, dwNewLong);
	}
	LONG GetWindowLong(int nIndex)
	{
		if (IsNull()) return 0;
		return ::GetWindowLong(Get(), nIndex);
	}

	// 窗口移动
	void MoveWindow(int x, int y, int nWidth, int nHeight, bool bRepaint/* = true*/)
	{
		if (IsNull()) return;
		::MoveWindow(Get(), x, y, nWidth, nHeight, bRepaint);
	}
	void MoveWindow(CRect& lpRect, bool bRepaint/* = true*/)
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
	void CenterWindow(wnd_t hWndCenter/* = NULL*/)
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
				IGuiBoardBase(hWndCenter).GetWindowRect(rcCenter);
				GetMonitorInfo(MonitorFromWindow(hWndCenter, MONITOR_DEFAULTTONEAREST), &mi);
				rcArea = mi.rcWork;
			}
		}
		else
		{
			// center within parent client coordinates
			hWndParent = GetParent();
			ExAssert(::IsWindow(hWndParent));
			IGuiBoardBase(hWndParent).GetClientRect(rcArea);
			ExAssert(::IsWindow(hWndCenter));
			IGuiBoardBase(hWndCenter).GetClientRect(rcCenter);
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

	// 窗口坐标转换
	void ClientToScreen(CPoint& lpPoint)
	{
		if (IsNull()) return;
		::ClientToScreen(Get(), (LPPOINT)&lpPoint);
	}
	void ClientToScreen(CRect& lpRect)
	{
		if (IsNull()) return;
		CPoint pt(lpRect.pt1);
		ClientToScreen(pt);
		lpRect.pt2.x += (pt.x - lpRect.pt1.x);
		lpRect.pt2.y += (pt.y - lpRect.pt1.y);
		lpRect.pt1.x = pt.x;
		lpRect.pt1.y = pt.y;
	}
	void ScreenToClient(CPoint& lpPoint)
	{
		if (IsNull()) return;
		::ScreenToClient(Get(), (LPPOINT)&lpPoint);
	}
	void ScreenToClient(CRect& lpRect)
	{
		if (IsNull()) return;
		CPoint pt(lpRect.pt1);
		ScreenToClient(pt);
		lpRect.pt2.x += (pt.x - lpRect.pt1.x);
		lpRect.pt2.y += (pt.y - lpRect.pt1.y);
		lpRect.pt1.x = pt.x;
		lpRect.pt1.y = pt.y;
	}

	// 窗口刷新
	void Invalidate()
	{
		if (IsNull()) return ;
		RECT rect;
		::GetClientRect(Get(), &rect);
		::InvalidateRect(Get(), &rect, TRUE);
	}
	void InvalidateRect(CRect& rcInv)
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
	void InvalidateRgn(HRGN hRgn)
	{
		if (IsNull()) return ;
		::InvalidateRgn(Get(), hRgn, TRUE);
	}
	bool ShowWindow(int nCmdShow)
	{
		if(IsNull()) return false;
		return (bool)::ShowWindow(Get(), nCmdShow);
	}
	bool UpdateWindow()
	{
		if (IsNull()) return false;
		return (bool)::UpdateWindow(Get());
	}

	// 窗口DC
	graph_t GetDC()
	{
		if(IsNull()) return NULL;
		return (graph_t)::GetDC(Get());
	}
	bool ReleaseDC(graph_t hdc)
	{
		if(IsNull()) return NULL;
		return ::ReleaseDC(Get(), (HDC)hdc);
	}

	// 获得窗口大小
	bool GetWindowRect(CRect& lpRect)
	{
		if (IsNull()) return false;
		return (bool)::GetWindowRect(Get(), (LPRECT)&lpRect);
	}
	bool GetClientRect(CRect& lpRect)
	{
		if (IsNull()) return false;
		return (bool)::GetClientRect(Get(), (LPRECT)&lpRect);
	}

	// 窗口关系控制
	wnd_t GetParent()
	{
		if (IsNull()) return NULL;
		return (wnd_t)::GetParent(Get());
	}

	// 设置焦点
	wnd_t SetFocus()
	{ return (wnd_t)::SetFocus(Get()); }
	bool IsFocus()
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
};

EXP_IMPLEMENT_DYNAMIC_CLS(IGuiBoardBase, IGuiBoard)
const CString IGuiBoardBase::s_ClassName = _T("GuiExp_Foundation");

//////////////////////////////////////////////////////////////////

#include "GuiBoard/GuiThunk.hpp"

//////////////////////////////////////////////////////////////////

EXP_END