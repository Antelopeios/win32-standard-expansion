// Copyright 2012, 木头云
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
// GuiMenu - 菜单类
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-02-22
// Version:	1.0.0000.1500
//
// History:
//	- 1.0.0000.1500(2012-02-22)	@ 开始构建GuiMenu
//////////////////////////////////////////////////////////////////

#ifndef __GuiMenu_hpp__
#define __GuiMenu_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiMenu : public CGuiWnd
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiMenu, CGuiWnd)

public:
	CGuiMenu()
	{
		// 添加事件对象
		AddEvent(ExGui(_T("CGuiMenuEvent"), GetGC()));
	}

public:
	void* Execute(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent)
	{
		Create();

		BOOL clr_key = FALSE; pixel_t key = ExRGB(255, 0, 255);
		CString t = xml.GetAttr(_T("mask"), ite);
		if(!t.Empty())
		{
			t.Lower();
			clr_key = TRUE;
			key = ExStringToColor(t);
		}
		t = xml.GetAttr(_T("layer"), ite); t.Lower();
		if (t == _T("false"))
			SetLayered(FALSE, clr_key, key);
		else
		if (t == _T("true"))
			SetLayered(TRUE, clr_key, key);

		return this;
	}
	BOOL Create()
	{
		return EXP_BASE::Create(NULL, CRect(), SW_HIDE, WS_POPUP, WS_EX_TOOLWINDOW);
	}
	void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
	{
		if(!GetChildren().Empty())
		{
			IGuiCtl* grd = ExDynCast<IGuiCtl>(GetChildren().HeadItem());
			if (grd)
			{
				CRect rc;
				grd->GetClientRect(rc);
				nWidth = rc.Width();
				nHeight = rc.Height();
			}
		}
		EXP_BASE::MoveWindow(x, y, nWidth, nHeight, bRepaint);
	}
	BOOL ShowWindow(int nCmdShow)
	{
		if (nCmdShow == SW_HIDE)
		{
			::ReleaseCapture();
			return EXP_BASE::ShowWindow(SW_HIDE);
		}
		CRect rc_pop, rc_wnd, rc_dsk;
		if (nCmdShow != NULL && 
			nCmdShow > SW_MAX) rc_pop = *(CRect*)nCmdShow;
		GetWindowRect(rc_wnd);
		GetDesktopRect(rc_dsk, rc_wnd.LeftTop());
		// 调整窗口位置
		if (rc_pop.IsEmpty())
		{
			if (rc_wnd.Right() > rc_dsk.Right())
				rc_wnd.Offset(CPoint(min(-rc_wnd.Width(), rc_dsk.Right() - rc_wnd.Right()), 0));
			if (rc_wnd.Bottom() > rc_dsk.Bottom())
				rc_wnd.Offset(CPoint(0, min(-rc_wnd.Height(), rc_dsk.Bottom() - rc_wnd.Bottom())));
		}
		else
		if (rc_wnd.LeftTop() == rc_pop.LeftBottom())
		{
			if (rc_wnd.Right() > rc_dsk.Right())
				rc_wnd.Offset(CPoint(rc_dsk.Right() - rc_wnd.Right(), 0));
			if (rc_wnd.Bottom() > rc_dsk.Bottom())
				rc_wnd.Offset(CPoint(0, min(-(rc_wnd.Height() + rc_pop.Height()), rc_dsk.Bottom() - rc_wnd.Bottom())));
		}
		else
		if (rc_wnd.LeftTop() == rc_pop.RightTop())
		{
			if (rc_wnd.Right() > rc_dsk.Right())
				rc_wnd.Offset(CPoint(min(-(rc_wnd.Width() + rc_pop.Width()), rc_dsk.Right() - rc_wnd.Right()), 0));
			if (rc_wnd.Bottom() > rc_dsk.Bottom())
				rc_wnd.Offset(CPoint(0, min(-(rc_wnd.Height() - rc_pop.Height()), rc_dsk.Bottom() - rc_wnd.Bottom())));
		}
		// 设置鼠标绑定
		::SetCapture(GethWnd());
		// 调整窗口状态
		return ::SetWindowPos(GethWnd(), HWND_TOPMOST, 
			rc_wnd.Left(), rc_wnd.Top(), rc_wnd.Width(), rc_wnd.Height(), 
			SWP_NOACTIVATE | SWP_SHOWWINDOW);
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiMenu, CGuiWnd)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiMenu_hpp__*/
