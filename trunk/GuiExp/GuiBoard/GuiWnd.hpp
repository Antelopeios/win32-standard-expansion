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
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2010-05-16
// Version:	1.0.0000.1434
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
	EXP_DECLARE_DYNCREATE_CLS(CGuiWnd, IGuiThunk)

protected:
	// 消息转发
	LRESULT WndSend(UINT nMessage, WPARAM wParam, LPARAM lParam)
	{
		LRESULT ret = 0;
		for(list_t::iterator_t ite = list_t::Head(); ite != list_t::Tail(); ++ite)
		{
			IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(*ite);
			if (!ctrl) continue;
			ctrl->Send(*ite, nMessage, wParam, lParam);
			// 判断返回值
			for(IGuiCtrl::evt_list_t::iterator_t ite = ctrl->GetEvent().Head(); ite != ctrl->GetEvent().Tail(); ++ite)
			{
				if (!(*ite)) continue;
				LRESULT r = (*ite)->GetResult();
				if (ret != r) ret = r;
			}
		}
		return ret;
	}
	LRESULT WndProc(UINT nMessage, WPARAM wParam, LPARAM lParam)
	{
		LRESULT ret = 0;
		// 筛选消息
		switch( nMessage )
		{
		case WM_PAINT:
			{
				PAINTSTRUCT ps = {0};
				HDC hdc = BeginPaint(Get(), &ps);

				// 构建绘图缓存
				CRect rect;
				GetClientRect(rect);
				CImage mem_img;
				mem_img.Create(rect.Width(), rect.Height());

				// 发送绘图消息
				ret = WndSend(nMessage, wParam, (LPARAM)&mem_img);

				// 覆盖缓存绘图
				CGraph mem_grp;
				mem_grp.Create(hdc);
				mem_grp.SetObject(mem_img.Get());
				if (m_bLayered)
					LayerWindow(mem_grp);
				else
					::BitBlt(hdc, rect.Left(), rect.Top(), rect.Width(), rect.Height(), mem_grp, 0, 0, SRCCOPY);
				mem_grp.Delete();
				mem_img.Delete();

				EndPaint(Get(), &ps);
			}
			break;
		case WM_ERASEBKGND:
			ret = WndSend(nMessage, wParam, lParam);
			break;
		default:
			{
				ret = EXP_BASE::WndProc(nMessage, wParam, lParam);
				LRESULT r = WndSend(nMessage, wParam, lParam);
				if (ret != r) ret = r;
			}
		}
		return ret;
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiWnd, IGuiThunk)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiWnd_hpp__*/