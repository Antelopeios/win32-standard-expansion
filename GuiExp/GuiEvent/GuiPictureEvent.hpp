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
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2010-05-18
// Version:	1.0.0000.1150
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

protected:
	CRect m_rcOld;
	CImage m_imgTmp;

public:
	~CGuiPictureEvent() { m_imgTmp.Delete(); }

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		// 处理消息
		switch( nMessage )
		{
		case WM_PAINT:
			if (lParam)
			{
				CGC gc;
				IGuiCtrl::state_t* state = ctrl->GetState(_T("image"), &gc);
				if (!state) break;
				CImage* image = (CImage*)(((void**)state->sta_arr)[0]);
				if (!image || image->IsNull()) break;
				CImage* mem_img = (CImage*)lParam;
				CRect rect;
				ctrl->GetRealRect(rect);

				CGraph mem_grp;
				mem_grp.Create();
				mem_grp.SetObject(mem_img->Get());
				FillRect(mem_grp, &(RECT)rect, (HBRUSH)GetStockObject(GRAY_BRUSH));
				mem_grp.Delete();

				// 处理
				if (m_rcOld != rect)
				{
					m_imgTmp.Delete();
					m_imgTmp.Set(CImgDeformer::ZomDeform(image->Get(), rect.Width(), rect.Height()));
					m_rcOld = rect;
				}
				// 绘图
				CImgRenderer::Render(mem_img->Get(), m_imgTmp, rect, CPoint());
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiPictureEvent, IGuiEvent);

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiPictureEvent_hpp__*/