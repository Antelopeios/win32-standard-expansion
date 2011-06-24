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
// Date:	2011-06-24
// Version:	1.0.0008.1253
//
// History:
//	- 1.0.0001.1054(2011-05-11)	+ 添加IGuiBoard::Attach()和IGuiBoard::Detach()接口
//	- 1.0.0002.1525(2011-05-13)	+ 添加IGuiBoard::Send()和IGuiBoard::Post()接口
//	- 1.0.0003.1705(2011-05-18)	+ 添加IGuiBoard::Layer()窗口图层化接口
//	- 1.0.0004.1000(2011-05-19)	= IGuiBoard的基类改为IGuiBase
//								+ 添加IGuiBoard::LayeredWindow();IGuiBoard::DefProc();IGuiBoard::GethWnd()
//	- 1.0.0005.2202(2011-05-23)	= 调整IGuiBoard::LayeredWindow()接口
//	- 1.0.0006.1411(2011-05-26)	+ 添加IGuiBoard::GetRealRect()接口实现
//	- 1.0.0007.0942(2011-06-17)	= 将IGuiBoardBase接口移动到GuiBoard.h中,使外部可以使用此接口
//	- 1.0.0008.1253(2011-06-24)	+ 将IGuiBoard支持通过Create接口控制WNDCLASSEX的style
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
						wnd_t wndParent = NULL, UINT uStyle = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW) = 0;
	virtual bool IsNull() const = 0;

	virtual bool Attach(wnd_t hWnd) = 0;
	virtual wnd_t Detach() = 0;

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

// 自定义的内存分配器
struct _GuiBoardAlloc
{
	EXP_INLINE static void Free(void* pPtr)
	{
		if (!pPtr) return;
		::DestroyWindow((HWND)pPtr);
	}
};

// 导出的基础类定义
template <typename TypeT, typename AllocT = EXP_MEMORY_ALLOC, typename ModelT = EXP_THREAD_MODEL>
class EXP_API CSmartPtrT;
template <typename TypeT, typename TypeAllocT = _TypeAlloc>
interface EXP_API ITypeObjectT;

// GUI 窗口对象
interface EXP_API IGuiBoardBase : public IGuiBoard, public ITypeObjectT<wnd_t, _GuiBoardAlloc>
{
	EXP_DECLARE_DYNAMIC_MULT(IGuiBoardBase, IGuiBoard)

	typedef ITypeObjectT<wnd_t, _GuiBoardAlloc> type_base_t;

protected:
	static const LPCTSTR s_ClassName;
	HINSTANCE m_hIns;

	bool m_bLayered;

public:
	IGuiBoardBase(void);
	IGuiBoardBase(wnd_t hWnd);
	virtual ~IGuiBoardBase(void);

protected:
	static LRESULT CALLBACK BoardProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam);
	ATOM RegisterWndClass(LPCTSTR sClassName, UINT uStyle);

public:
	bool Create(LPCTSTR sWndName, CRect& rcWnd, 
				int nCmdShow = SW_SHOWNORMAL, DWORD dwStyle = WS_POPUP, DWORD dwExStyle = NULL, 
				wnd_t wndParent = NULL, UINT uStyle = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW);

	bool IsNull() const;

	wnd_t operator=(wnd_t tType);

	bool Attach(wnd_t hWnd);
	wnd_t Detach();
	wnd_t GethWnd();

	// 窗口消息
	LRESULT SendMessage(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0);
	bool PostMessage(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0);

	// 窗口属性修改
	DWORD GetStyle();
	DWORD GetExStyle();
	bool ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
	bool ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0, int nStyleOffset = GWL_EXSTYLE);
	LONG SetWindowLong(int nIndex, LONG dwNewLong);
	LONG GetWindowLong(int nIndex);

	// 窗口移动
	void MoveWindow(int x, int y, int nWidth, int nHeight, bool bRepaint = true);
	void MoveWindow(CRect& lpRect, bool bRepaint = true);
	void CenterWindow(wnd_t hWndCenter = NULL);

	// 窗口坐标转换
	void ClientToScreen(CPoint& lpPoint);
	void ClientToScreen(CRect& lpRect);
	void ScreenToClient(CPoint& lpPoint);
	void ScreenToClient(CRect& lpRect);

	// 窗口刷新
	void Invalidate();
	void InvalidateRect(CRect& rcInv);
	void InvalidateRgn(HRGN hRgn);
	bool ShowWindow(int nCmdShow);
	bool UpdateWindow();

	// 窗口DC
	graph_t GetDC();
	bool ReleaseDC(graph_t hdc);

	// 获得窗口大小
	bool GetWindowRect(CRect& lpRect);
	bool GetClientRect(CRect& lpRect);

	// 窗口关系控制
	wnd_t GetParent();

	// 设置焦点
	wnd_t SetFocus();
	bool IsFocus();

	// 窗口图层化
	void SetLayered(bool bLayered = true);
	bool IsLayered();
	void LayeredWindow(HDC hDC, HDC tGrp);
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiBoard_h__*/