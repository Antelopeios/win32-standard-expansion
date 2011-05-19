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
// GuiWndEvent - 窗口事件
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2010-05-19
// Version:	1.0.0000.1020
//////////////////////////////////////////////////////////////////

#ifndef __GuiWndEvent_hpp__
#define __GuiWndEvent_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiBoard/GuiBoard.h"
#include "GuiCtrl/GuiCtrl.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiWndEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiWndEvent, IGuiEvent)

protected:
	// 消息转发
	LRESULT WndSend(IGuiBoard* pGui, UINT nMessage, WPARAM wParam, LPARAM lParam, LRESULT lrDef = 0)
	{
		if (!pGui) return 0;
		// 向控件转发消息
		for(IGuiBoard::list_t::iterator_t ite = pGui->GetChildren().Head(); ite != pGui->GetChildren().Tail(); ++ite)
		{
			IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(*ite);
			if (!ctrl) continue;
			// 初始化返回值
			ctrl->SetResult(lrDef); // 发送消息时,让控件对象收到上一个控件的处理结果
			// 转发消息
			ctrl->Send(*ite, nMessage, wParam, lParam);
			// 判断返回值
			ctrl->GetResult(lrDef);
		}
		return lrDef;
	}

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiBoard* board = ExDynCast<IGuiBoard>(pGui);
		if (!board) return;
		// 筛选消息
		LRESULT ret = 0;
		switch( nMessage )
		{
		case WM_PAINT:
			{
				// 构建绘图缓存
				CRect rect;
				board->GetClientRect(rect);
				CImage mem_img;
				mem_img.Create(rect.Width(), rect.Height());
				// 发送绘图消息
				ret = WndSend(board, nMessage, wParam, (LPARAM)&mem_img);
				// 覆盖缓存绘图
				CGraph mem_grp;
				mem_grp.Create();
				mem_grp.SetObject(mem_img.Get());
				board->LayeredWindow(mem_grp);
				mem_grp.Delete();
				mem_img.Delete();
			}
			break;
		case WM_ERASEBKGND:
			ret = WndSend(board, nMessage, wParam, lParam);
			break;
		default:
			ret = WndSend(board, nMessage, wParam, lParam, board->DefProc(nMessage, wParam, lParam));
		}
		SetResult(ret);
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiWndEvent, IGuiEvent);

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiWndEvent_hpp__*/