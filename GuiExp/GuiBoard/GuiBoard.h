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
// Date:	2011-05-26
// Version:	1.0.0006.1411
//
// History:
//	- 1.0.0001.1054(2011-05-11)	+ ���IGuiBoard::Attach()��IGuiBoard::Detach()�ӿ�
//	- 1.0.0002.1525(2011-05-13)	+ ���IGuiBoard::Send()��IGuiBoard::Post()�ӿ�
//	- 1.0.0003.1705(2011-05-18)	+ ���IGuiBoard::Layer()����ͼ�㻯�ӿ�
//	- 1.0.0004.1000(2011-05-19)	= IGuiBoard�Ļ����ΪIGuiBase
//								+ ���IGuiBoard::LayeredWindow();IGuiBoard::DefProc();IGuiBoard::GethWnd()
//	- 1.0.0005.2202(2011-05-23)	= ����IGuiBoard::LayeredWindow()�ӿ�
//	- 1.0.0006.1411(2011-05-26)	+ ���IGuiBoard::GetRealRect()�ӿ�ʵ��
//////////////////////////////////////////////////////////////////

#ifndef __GuiBoard_h__
#define __GuiBoard_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiCommon/GuiCommon.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI ���ڶ���ӿ�
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

	// ������Ϣ
	virtual LRESULT DefProc(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0) = 0;
	virtual LRESULT SendMessage(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0) = 0;
	virtual bool PostMessage(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0) = 0;

	// ���������޸�
	virtual DWORD GetStyle() = 0;
	virtual DWORD GetExStyle() = 0;
	virtual bool ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0) = 0;
	virtual bool ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0, int nStyleOffset = GWL_EXSTYLE) = 0;
	virtual LONG SetWindowLong(int nIndex, LONG dwNewLong) = 0;
	virtual LONG GetWindowLong(int nIndex) = 0;

	// ����ˢ��
	virtual void Invalidate() = 0;
	virtual void InvalidateRect(CRect& rcInv) = 0;
	virtual void InvalidateRgn(HRGN hRgn) = 0;
	virtual bool ShowWindow(int nCmdShow) = 0;
	virtual bool UpdateWindow() = 0;

	// ����DC
	virtual graph_t GetDC() = 0;
	virtual bool ReleaseDC(graph_t hdc) = 0;

	// ��ô��ڴ�С
	virtual bool GetWindowRect(CRect& lpRect) = 0;
	virtual bool GetClientRect(CRect& lpRect) = 0;
	virtual bool GetRealRect(CRect& rc) { return GetClientRect(rc); }

	// �����ƶ�
	virtual void MoveWindow(int x, int y, int nWidth, int nHeight, bool bRepaint = true) = 0;
	virtual void MoveWindow(CRect& lpRect, bool bRepaint = true) = 0;
	virtual void CenterWindow(wnd_t hWndCenter = NULL) = 0;

	// ��������ת��
	virtual void ClientToScreen(CPoint& lpPoint) = 0;
	virtual void ClientToScreen(CRect& lpRect) = 0;
	virtual void ScreenToClient(CPoint& lpPoint) = 0;
	virtual void ScreenToClient(CRect& lpRect) = 0;

	// ���ڹ�ϵ����
	virtual wnd_t GetParent() = 0;

	// ���ý���
	virtual wnd_t SetFocus() = 0;
	static wnd_t GetFocus() { return (wnd_t)::GetFocus(); }
	virtual bool IsFocus() = 0;

	// ����ͼ�㻯
	virtual void SetLayered(bool bLayered = true) = 0;
	virtual bool IsLayered() = 0;
	virtual void LayeredWindow(HDC hDC, HDC tGrp) = 0;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiBoard_h__*/