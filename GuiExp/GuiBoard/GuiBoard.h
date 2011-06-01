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
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-26
// Version:	1.0.0006.1411
//
// History:
//	- 1.0.0001.1054(2011-05-11)	+ 添加IGuiBoard::Attach()和IGuiBoard::Detach()接口
//	- 1.0.0002.1525(2011-05-13)	+ 添加IGuiBoard::Send()和IGuiBoard::Post()接口
//	- 1.0.0003.1705(2011-05-18)	+ 添加IGuiBoard::Layer()窗口图层化接口
//	- 1.0.0004.1000(2011-05-19)	= IGuiBoard的基类改为IGuiBase
//								+ 添加IGuiBoard::LayeredWindow();IGuiBoard::DefProc();IGuiBoard::GethWnd()
//	- 1.0.0005.2202(2011-05-23)	= 调整IGuiBoard::LayeredWindow()接口
//	- 1.0.0006.1411(2011-05-26)	+ 添加IGuiBoard::GetRealRect()接口实现
//////////////////////////////////////////////////////////////////

#ifndef __GuiBoard_h__
#define __GuiBoard_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiCommon/GuiCommon.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI 窗口对象接口
interface EXP_API IGuiBoard : public IGuiBase
{
	EXP_DECLARE_DYNAMIC_MULT(IGuiBoard, IGuiBase)

public:
	IGuiBoard(void) {}
	virtual ~IGuiBoard(void) {}

public:
	virtual bool Create(LPCTSTR sWndName, CRect& rcWnd, 
						int nCmdShow = SW_SHOWNORMAL, DWORD dwStyle = WS_POPUP, DWORD dwExStyle = NULL, 
						wnd_t wndParent = NULL) = 0;
	virtual bool IsNull() const = 0;

	virtual bool Attach(wnd_t hWnd) = 0;
	virtual wnd_t Detach() = 0;
	virtual wnd_t GethWnd() = 0;

	// 窗口消息
	virtual LRESULT DefProc(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0) = 0;
	virtual LRESULT SendMessage(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0) = 0;
	virtual bool PostMessage(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0) = 0;

	// 窗口属性修改
	virtual DWORD GetStyle() = 0;
	virtual DWORD GetExStyle() = 0;
	virtual bool ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0) = 0;
	virtual bool ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0, int nStyleOffset = GWL_EXSTYLE) = 0;
	virtual LONG SetWindowLong(int nIndex, LONG dwNewLong) = 0;
	virtual LONG GetWindowLong(int nIndex) = 0;

	// 窗口刷新
	virtual void Invalidate() = 0;
	virtual void InvalidateRect(CRect& rcInv) = 0;
	virtual void InvalidateRgn(HRGN hRgn) = 0;
	virtual bool ShowWindow(int nCmdShow) = 0;
	virtual bool UpdateWindow() = 0;

	// 窗口DC
	virtual graph_t GetDC() = 0;
	virtual bool ReleaseDC(graph_t hdc) = 0;

	// 获得窗口大小
	virtual bool GetWindowRect(CRect& lpRect) = 0;
	virtual bool GetClientRect(CRect& lpRect) = 0;
	virtual bool GetRealRect(CRect& rc) { return GetClientRect(rc); }

	// 窗口移动
	virtual void MoveWindow(int x, int y, int nWidth, int nHeight, bool bRepaint = true) = 0;
	virtual void MoveWindow(CRect& lpRect, bool bRepaint = true) = 0;
	virtual void CenterWindow(wnd_t hWndCenter = NULL) = 0;

	// 窗口坐标转换
	virtual void ClientToScreen(CPoint& lpPoint) = 0;
	virtual void ClientToScreen(CRect& lpRect) = 0;
	virtual void ScreenToClient(CPoint& lpPoint) = 0;
	virtual void ScreenToClient(CRect& lpRect) = 0;

	// 窗口关系控制
	virtual wnd_t GetParent() = 0;

	// 设置焦点
	virtual wnd_t SetFocus() = 0;
	static wnd_t GetFocus() { return (wnd_t)::GetFocus(); }
	virtual bool IsFocus() = 0;

	// 窗口图层化
	virtual void SetLayered(bool bLayered = true) = 0;
	virtual bool IsLayered() = 0;
	virtual void LayeredWindow(HDC hDC, HDC tGrp) = 0;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiBoard_h__*/