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
// GuiWnd - 窗口类
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-19
// Version:	1.0.0001.1100
//
// History:
//	- 1.0.0001.1100(2011-05-19)	= 将CGuiWnd与事件处理分离,处理过程放入CGuiWndEvent
//////////////////////////////////////////////////////////////////

#ifndef __GuiWnd_hpp__
#define __GuiWnd_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiCtrl/GuiCtrl.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiWnd : public IGuiThunk
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiWnd, IGuiThunk)

public:
	void* Execute(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent)
	{
		Create(
			xml.GetAttr(_T("text"), ite), 
			ExStringToRect(xml.GetAttr(_T("rect"), ite)), SW_HIDE);

		CString t = xml.GetAttr(_T("style"), ite); t.Upper();
		CArrayT<CString> sa;
		ExStringToArray(t, sa);
		for(int i = 0; i < (int)sa.GetCount(); ++i)
		{
			if (sa[i] == _T("WS_OVERLAPPED"))
				ModifyStyle(0, WS_OVERLAPPED);
			else
			if (sa[i] == _T("WS_POPUP"))
				ModifyStyle(0, WS_POPUP);
			else
			if (sa[i] == _T("WS_CHILD"))
				ModifyStyle(0, WS_CHILD);
			else
			if (sa[i] == _T("WS_MINIMIZE"))
				ModifyStyle(0, WS_MINIMIZE);
			else
			if (sa[i] == _T("WS_VISIBLE"))
				ModifyStyle(0, WS_VISIBLE);
			else
			if (sa[i] == _T("WS_DISABLED"))
				ModifyStyle(0, WS_DISABLED);
			else
			if (sa[i] == _T("WS_CLIPSIBLINGS"))
				ModifyStyle(0, WS_CLIPSIBLINGS);
			else
			if (sa[i] == _T("WS_CLIPCHILDREN"))
				ModifyStyle(0, WS_CLIPCHILDREN);
			else
			if (sa[i] == _T("WS_MAXIMIZE"))
				ModifyStyle(0, WS_MAXIMIZE);
			else
			if (sa[i] == _T("WS_CAPTION"))
				ModifyStyle(0, WS_CAPTION);
			else
			if (sa[i] == _T("WS_BORDER"))
				ModifyStyle(0, WS_BORDER);
			else
			if (sa[i] == _T("WS_DLGFRAME"))
				ModifyStyle(0, WS_DLGFRAME);
			else
			if (sa[i] == _T("WS_VSCROLL"))
				ModifyStyle(0, WS_VSCROLL);
			else
			if (sa[i] == _T("WS_HSCROLL"))
				ModifyStyle(0, WS_HSCROLL);
			else
			if (sa[i] == _T("WS_SYSMENU"))
				ModifyStyle(0, WS_SYSMENU);
			else
			if (sa[i] == _T("WS_THICKFRAME"))
				ModifyStyle(0, WS_THICKFRAME);
			else
			if (sa[i] == _T("WS_GROUP"))
				ModifyStyle(0, WS_GROUP);
			else
			if (sa[i] == _T("WS_TABSTOP"))
				ModifyStyle(0, WS_TABSTOP);
			else
			if (sa[i] == _T("WS_MINIMIZEBOX"))
				ModifyStyle(0, WS_MINIMIZEBOX);
			else
			if (sa[i] == _T("WS_MAXIMIZEBOX"))
				ModifyStyle(0, WS_MAXIMIZEBOX);
			else
			if (sa[i] == _T("WS_TILED"))
				ModifyStyle(0, WS_TILED);
			else
			if (sa[i] == _T("WS_ICONIC"))
				ModifyStyle(0, WS_ICONIC);
			else
			if (sa[i] == _T("WS_SIZEBOX"))
				ModifyStyle(0, WS_SIZEBOX);
			else
			if (sa[i] == _T("WS_TILEDWINDOW"))
				ModifyStyle(0, WS_TILEDWINDOW);
			else
			if (sa[i] == _T("WS_OVERLAPPEDWINDOW"))
				ModifyStyle(0, WS_OVERLAPPEDWINDOW);
			else
			if (sa[i] == _T("WS_POPUPWINDOW"))
				ModifyStyle(0, WS_POPUPWINDOW);
			else
			if (sa[i] == _T("WS_CHILDWINDOW"))
				ModifyStyle(0, WS_CHILDWINDOW);
		}

		t = xml.GetAttr(_T("pos"), ite); t.Lower();
		if (t == _T("center")) CenterWindow();

		t = xml.GetAttr(_T("icon"), ite);
		UINT i = _ttoi(t);
		HICON ic_wnd = ::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(i));
		SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)ic_wnd);
		SendMessage(WM_SETICON, (WPARAM)FALSE, (LPARAM)ic_wnd);

		BOOL clr_key = FALSE; pixel_t key = ExRGB(255, 0, 255);
		t = xml.GetAttr(_T("mask"), ite);
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

		t = xml.GetAttr(_T("visible"), ite); t.Lower();
		if (t == _T("false"))
			ShowWindow(SW_HIDE);
		else
		if (t == _T("true"))
			ShowWindow(SW_SHOW);

		return this;
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiWnd, IGuiThunk)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiWnd_hpp__*/