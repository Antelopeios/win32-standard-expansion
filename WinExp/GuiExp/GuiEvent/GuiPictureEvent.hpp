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
// GuiPictureEvent - 绘图板控件事件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-10-21
// Version:	1.0.0004.2225
//
// History:
//	- 1.0.0001.1500(2011-05-24)	+ CGuiPictureEvent添加Color属性的处理
//	- 1.0.0002.1424(2011-05-25)	+ CGuiPictureEvent添加Text属性的处理
//								# 修正CGuiPictureEvent添对Text属性处理过程中的内存泄漏
//	- 1.0.0003.2006(2011-06-29)	# 修正当不设置背景图片时,CGuiPictureEvent将跳过所有绘图的bug
//	- 1.0.0004.2225(2011-10-21)	+ 添加关闭缓存时的绘图逻辑
//////////////////////////////////////////////////////////////////

#ifndef __GuiPictureEvent_hpp__
#define __GuiPictureEvent_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiCtrl/GuiCtrl.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiPictureEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiPictureEvent, IGuiEvent)

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
				// 获得属性
				CImage* image = (CImage*)ctl->GetState(_T("image"));
				pixel_t pixel = (pixel_t)ctl->GetState(_T("color"));
				CText* text = (CText*)ctl->GetState(_T("font"));
				CString* str = (CString*)ctl->GetState(_T("text"));

				CGraph* mem_img = (CGraph*)lParam;
				if (!mem_img || mem_img->IsNull()) break;
				CRect rect;
				ctl->GetClientRect(rect);

				// 绘图
				CImgDrawer::Fill(*mem_img, rect, pixel);
				if (image && !image->IsNull())
					CImgDrawer::Draw(*mem_img, image->Get(), rect, 
						CPoint(), CSize(rect.Width(), rect.Height()));
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

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiPictureEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiPictureEvent_hpp__*/