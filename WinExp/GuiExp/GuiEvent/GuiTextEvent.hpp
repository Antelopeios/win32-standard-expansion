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
// GuiTextEvent - 文字控件事件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-13
// Version:	1.0.0000.0918
//
// History:
//	- 1.0.0000.0918(2012-03-13)	@ 开始构建GuiTextEvent
//////////////////////////////////////////////////////////////////

#ifndef __GuiTextEvent_hpp__
#define __GuiTextEvent_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiTextEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiTextEvent, IGuiEvent)
		
public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		// 处理消息
		switch( nMessage )
		{
		case WM_PAINT:
			if (lParam)
			{
				CGraph* mem_img = (CGraph*)lParam;
				if (!mem_img || mem_img->IsNull()) break;

				CRect rect;
				ctl->GetClientRect(rect);

				// 获得属性
				CImage* image = (CImage*)ctl->GetState(_T("image"));
				pixel_t pixel = (pixel_t)ctl->GetState(_T("color"));
				CText* text = (CText*)ctl->GetState(_T("font"));
				CString* str = (CString*)ctl->GetState(_T("text"));

				// 绘图
				CImgDrawer::Fill(*mem_img, rect, pixel);
				if (image && !image->IsNull())
					CImgDrawer::Draw(*mem_img, image->Get(), rect, 
						CPoint(), CSize(rect.Width(), rect.Height()));

				// 绘文字
				if (text && str)
				{
					CImage txt_img(text->GetImage(*str));
					if (!txt_img.IsNull())
						CImgDrawer::Draw(*mem_img, txt_img, CRect(
							(rect.Right() - txt_img.GetWidth()) / 2, 
							(rect.Bottom() - txt_img.GetHeight()) / 2, 
							rect.Right(), rect.Bottom()));
				}
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiTextEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiTextEvent_hpp__*/
