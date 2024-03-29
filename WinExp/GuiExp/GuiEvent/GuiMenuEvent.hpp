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
// GuiMenuEvent - 菜单事件逻辑
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-02-27
// Version:	1.0.0000.1100
//
// History:
//	- 1.0.0000.1100(2012-02-27)	@ 开始构建GuiMenuEvent
//////////////////////////////////////////////////////////////////

#ifndef __GuiMenuEvent_hpp__
#define __GuiMenuEvent_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiMenuEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiMenuEvent, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiWnd* wnd = ExDynCast<IGuiWnd>(pGui);
		if (!wnd) return;

		// 处理消息
		switch( nMessage )
		{
		case WM_MOUSEACTIVATE:
			{
				SetResult(MA_NOACTIVATE);
				SetState(return_next);
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			{
				CPoint pt(ExLoWord(lParam), ExHiWord(lParam));
				CRect rc;
				wnd->GetClientRect(rc);
				if(!rc.PtInRect(pt))
					wnd->ShowWindow(SW_HIDE);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiMenuEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiMenuEvent_hpp__*/
