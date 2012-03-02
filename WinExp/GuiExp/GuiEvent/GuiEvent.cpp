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
// GuiEvent - 事件对象
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-02
// Version:	1.0.0002.1630
//
// History:
//	- 1.0.0001.1750(2012-02-21)	+ 添加CQuitEvent事件对象
//	- 1.0.0002.1630(2012-03-02)	+ 添加CMoveEvent事件对象
//////////////////////////////////////////////////////////////////

#include "GuiCommon/GuiCommon.h"

//////////////////////////////////////////////////////////////////

#include "GuiEvent/GuiWndEvent.hpp"
#include "GuiEvent/GuiPictureEvent.hpp"
#include "GuiEvent/GuiButtonEvent.hpp"
#include "GuiEvent/GuiEditEvent.hpp"
#include "GuiEvent/GuiListEvent.hpp"
#include "GuiEvent/GuiLCEvent.hpp"
#include "GuiEvent/GuiLVEvent.hpp"
#include "GuiEvent/GuiScrollEvent.hpp"
#include "GuiEvent/GuiMenuEvent.hpp"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CQuitEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CQuitEvent, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		switch( nMessage )
		{
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CQuitEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

class CMoveEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CMoveEvent, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		switch( nMessage )
		{
		case WM_NCHITTEST:
			{
				LRESULT hit_test = GetResult();
				SetResult((hit_test == HTCLIENT) ? HTCAPTION : hit_test);
				SetState(return_next);
			}
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CMoveEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

EXP_END