// Copyright 2011-2012, 木头云
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
// Date:	2012-05-01
// Version:	1.0.0016.1545
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
//	- 1.0.0009.2008(2011-07-16)	= 调整SetLayered()接口,支持在非半透明图层化模式下设置透明色
//	- 1.0.0010.1818(2011-08-26)	+ 添加控制是否窗口自绘的接口
//	- 1.0.0011.1802(2012-02-02)	+ 添加IGuiBoard::SetParent()
//	- 1.0.0012.1200(2012-02-22)	= 将IGuiBoard改名为IGuiWnd
//	- 1.0.0013.1558(2012-02-27)	+ 添加支持仅移动窗口位置而不改变大小的MoveWindow方法
//	- 1.0.0014.2230(2012-03-02)	+ 窗口支持通过DoModal方法进行模态调用
//	- 1.0.0015.1449(2012-04-17)	^ 简化窗口对象的层次结构,移除IGuiBoardBase接口,将其所有功能并入IGuiWnd
//	- 1.0.0016.1545(2012-05-01)	+ 添加控制图层化窗口透明度的接口
//////////////////////////////////////////////////////////////////

#ifndef __GuiBoard_h__
#define __GuiBoard_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiCommon/GuiCommon.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// 自定义的对象分配器
struct _GuiBoardAlloc
{
	EXP_INLINE static void Free(void* pPtr)
	{
		if (!pPtr) return;
		::DestroyWindow((HWND)pPtr);
	}
};

// GUI 窗口对象接口
EXP_INTERFACE IGuiWnd : public IGuiBase, public ITypeObjectT<wnd_t, _GuiBoardAlloc>
{
	EXP_DECLARE_DYNAMIC_MULT(IGuiWnd, IGuiBase)
		
public:
	typedef ITypeObjectT<wnd_t, _GuiBoardAlloc> type_base_t;

protected:
	static const LPCTSTR s_ClassName;
	HINSTANCE m_hIns;

	BOOL m_bPaint;

	BOOL m_bLayered;
	BOOL m_bColorKey;
	pixel_t m_crKey;
	chann_t m_cAlpha;

	int m_ModalResult;
	BOOL m_IsModalLoop;

public:
	IGuiWnd(void);
	IGuiWnd(wnd_t hWnd);

public:
	// 全局工具接口
	static BOOL GetDesktopRect(CRect& rc, const CPoint& pt = CPoint(), BOOL bWork = FALSE);

protected:
	static LRESULT CALLBACK BoardProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam);
	ATOM RegisterWndClass(LPCTSTR sClassName, UINT uStyle);

public:
	BOOL Create(LPCTSTR sWndName, CRect& rcWnd, 
				int nCmdShow = SW_SHOWNORMAL, DWORD dwStyle = WS_POPUP, DWORD dwExStyle = NULL, 
				wnd_t wndParent = NULL, UINT uStyle = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW);
	BOOL Create();
	BOOL IsNull() const;
	wnd_t GethWnd() const;

	BOOL IsModalLoop();
	int RunModalLoop();
	void EndModalLoop(int nResult);
	int DoModal();
	void EndModal(int nResult);

	wnd_t operator=(wnd_t tType);

	virtual BOOL Attach(wnd_t hWnd);
	virtual wnd_t Detach();

	// 是否绘图
	void SetCusPaint(BOOL bPaint);
	BOOL IsCusPaint() const;

	// 窗口消息
	virtual LRESULT DefProc(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0) = 0;
	LRESULT SendMessage(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0);
	BOOL PostMessage(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0);

	// 窗口属性修改
	DWORD GetStyle() const;
	DWORD GetExStyle() const;
	BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
	BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0, int nStyleOffset = GWL_EXSTYLE);
	LONG SetWindowLong(int nIndex, LONG dwNewLong);
	LONG GetWindowLong(int nIndex) const;

	// 窗口刷新
	void Invalidate();
	void InvalidateRect(CRect& rcInv);
	void InvalidateRgn(HRGN hRgn);
	BOOL ShowWindow(int nCmdShow = SW_SHOW);
	BOOL UpdateWindow();
	BOOL IsVisible() const;

	// 窗口DC
	dc_t GetDC() const;
	BOOL ReleaseDC(dc_t hdc);

	// 获得窗口大小
	BOOL GetWindowRect(CRect& lpRect) const;
	BOOL GetClientRect(CRect& lpRect) const;
	BOOL GetRealRect(CRect& rc) const;

	// 窗口移动
	void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	void MoveWindow(CRect& lpRect, BOOL bRepaint = TRUE);
	void MoveWindow(int x, int y, BOOL bRepaint = TRUE);
	void MoveWindow(CPoint& lpPoint, BOOL bRepaint = TRUE);
	void CenterWindow(wnd_t hWndCenter = NULL);

	// 窗口坐标转换
	void ClientToScreen(CPoint& lpPoint) const;
	void ClientToScreen(CRect& lpRect) const;
	void ScreenToClient(CPoint& lpPoint) const;
	void ScreenToClient(CRect& lpRect) const;

	// 窗口关系控制
	wnd_t GetParent() const;
	wnd_t SetParent(wnd_t wndParent = NULL);

	// 设置焦点
	virtual wnd_t SetFocus();
	static wnd_t GetFocus();
	virtual BOOL IsFocus() const;

	// 窗口图层化
	void SetLayered(BOOL bLayered = TRUE, BOOL bColorKey = TRUE, pixel_t crKey = ExRGB(255, 0, 255));
	BOOL IsLayered() const;
	BOOL IsColorKey() const;
	pixel_t GetColorKey() const;
	void SetAlpha(chann_t cAlpha);
	chann_t GetAlpha() const;
	void LayeredWindow(HDC hDC, HDC hdc);
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiBoard_h__*/