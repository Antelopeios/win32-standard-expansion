// Copyright 2011-2012, ľͷ��
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
// Date:	2012-04-17
// Version:	1.0.0015.1449
//
// History:
//	- 1.0.0001.1054(2011-05-11)	+ ���IGuiBoard::Attach()��IGuiBoard::Detach()�ӿ�
//	- 1.0.0002.1525(2011-05-13)	+ ���IGuiBoard::Send()��IGuiBoard::Post()�ӿ�
//	- 1.0.0003.1705(2011-05-18)	+ ���IGuiBoard::Layer()����ͼ�㻯�ӿ�
//	- 1.0.0004.1000(2011-05-19)	= IGuiBoard�Ļ����ΪIGuiBase
//								+ ���IGuiBoard::LayeredWindow();IGuiBoard::DefProc();IGuiBoard::GethWnd()
//	- 1.0.0005.2202(2011-05-23)	= ����IGuiBoard::LayeredWindow()�ӿ�
//	- 1.0.0006.1411(2011-05-26)	+ ���IGuiBoard::GetRealRect()�ӿ�ʵ��
//	- 1.0.0007.0942(2011-06-17)	= ��IGuiBoardBase�ӿ��ƶ���GuiBoard.h��,ʹ�ⲿ����ʹ�ô˽ӿ�
//	- 1.0.0008.1253(2011-06-24)	+ ��IGuiBoard֧��ͨ��Create�ӿڿ���WNDCLASSEX��style
//	- 1.0.0009.2008(2011-07-16)	= ����SetLayered()�ӿ�,֧���ڷǰ�͸��ͼ�㻯ģʽ������͸��ɫ
//	- 1.0.0010.1818(2011-08-26)	+ ��ӿ����Ƿ񴰿��Ի�Ľӿ�
//	- 1.0.0011.1802(2012-02-02)	+ ���IGuiBoard::SetParent()
//	- 1.0.0012.1200(2012-02-22)	= ��IGuiBoard����ΪIGuiWnd
//	- 1.0.0013.1558(2012-02-27)	+ ���֧�ֽ��ƶ�����λ�ö����ı��С��MoveWindow����
//	- 1.0.0014.2230(2012-03-02)	+ ����֧��ͨ��DoModal��������ģ̬����
//	- 1.0.0015.1449(2012-04-17)	^ �򻯴��ڶ���Ĳ�νṹ,�Ƴ�IGuiBoardBase�ӿ�,�������й��ܲ���IGuiWnd
//////////////////////////////////////////////////////////////////

#ifndef __GuiBoard_h__
#define __GuiBoard_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiCommon/GuiCommon.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// �Զ���Ķ��������
struct _GuiBoardAlloc
{
	EXP_INLINE static void Free(void* pPtr)
	{
		if (!pPtr) return;
		::DestroyWindow((HWND)pPtr);
	}
};

// GUI ���ڶ���ӿ�
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

	int m_ModalResult;
	BOOL m_IsModalLoop;

public:
	IGuiWnd(void);
	IGuiWnd(wnd_t hWnd);

public:
	// ȫ�ֹ��߽ӿ�
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

	// �Ƿ��ͼ
	void SetCusPaint(BOOL bPaint);
	BOOL IsCusPaint() const;

	// ������Ϣ
	virtual LRESULT DefProc(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0) = 0;
	LRESULT SendMessage(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0);
	BOOL PostMessage(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0);

	// ���������޸�
	DWORD GetStyle() const;
	DWORD GetExStyle() const;
	BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
	BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0, int nStyleOffset = GWL_EXSTYLE);
	LONG SetWindowLong(int nIndex, LONG dwNewLong);
	LONG GetWindowLong(int nIndex) const;

	// ����ˢ��
	void Invalidate();
	void InvalidateRect(CRect& rcInv);
	void InvalidateRgn(HRGN hRgn);
	BOOL ShowWindow(int nCmdShow = SW_SHOW);
	BOOL UpdateWindow();
	BOOL IsVisible() const;

	// ����DC
	dc_t GetDC() const;
	BOOL ReleaseDC(dc_t hdc);

	// ��ô��ڴ�С
	BOOL GetWindowRect(CRect& lpRect) const;
	BOOL GetClientRect(CRect& lpRect) const;
	BOOL GetRealRect(CRect& rc) const;

	// �����ƶ�
	void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	void MoveWindow(CRect& lpRect, BOOL bRepaint = TRUE);
	void MoveWindow(int x, int y, BOOL bRepaint = TRUE);
	void MoveWindow(CPoint& lpPoint, BOOL bRepaint = TRUE);
	void CenterWindow(wnd_t hWndCenter = NULL);

	// ��������ת��
	void ClientToScreen(CPoint& lpPoint) const;
	void ClientToScreen(CRect& lpRect) const;
	void ScreenToClient(CPoint& lpPoint) const;
	void ScreenToClient(CRect& lpRect) const;

	// ���ڹ�ϵ����
	wnd_t GetParent() const;
	wnd_t SetParent(wnd_t wndParent = NULL);

	// ���ý���
	virtual wnd_t SetFocus();
	static wnd_t GetFocus();
	virtual BOOL IsFocus() const;

	// ����ͼ�㻯
	void SetLayered(BOOL bLayered = TRUE, BOOL bColorKey = TRUE, pixel_t crKey = ExRGB(255, 0, 255));
	BOOL IsLayered() const;
	BOOL IsColorKey() const;
	pixel_t GetColorKey() const;
	void LayeredWindow(HDC hDC, HDC hdc);
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiBoard_h__*/