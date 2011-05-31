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
// GuiButtonEvent - 按钮控件事件
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2010-05-27
// Version:	1.0.0002.1020
//
// History:
//	- 1.0.0000.2258(2010-05-25)	@ 开始构建CGuiButtonEvent
//								@ 基本完成CGuiButtonEvent的绘图部分
//	- 1.0.0001.2302(2010-05-26)	# 修正CGuiButtonEvent绘图中的错误
//								+ CGuiButtonEvent添加状态变化消息处理
//								# 当鼠标按下后移动,按钮状态会变为未按下的状态
//								+ 添加按钮单击消息转发及Enter键响应
//	- 1.0.0002.1020(2010-05-27)	# 修正CGuiButtonEvent当WM_KEYUP之后无法响应焦点切换绘图
//////////////////////////////////////////////////////////////////

#ifndef __GuiButtonEvent_hpp__
#define __GuiButtonEvent_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiCtrl/GuiCtrl.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiButtonEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiButtonEvent, IGuiEvent)

protected:
	CRect m_rcOld;
	CImage m_imgTmp[9];

public:
	~CGuiButtonEvent()
	{}

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		// 处理消息
		switch( nMessage )
		{
		case WM_MOUSEMOVE:
			{
				CGC gc;
				IGuiCtrl::state_t* state = ctrl->GetState(_T("status"), &gc);
				if (!state) break;
				DWORD status = *(DWORD*)(((void**)state->sta_arr)[0]);
				if (status != 2)
				{
					status = 1;
					ctrl->SetState(_T("status"), &status);
				}
			}
			break;
		case WM_KEYDOWN:
			if (wParam != VK_RETURN) break; // Enter
		case WM_LBUTTONDOWN:
			{
				DWORD status = 2;
				ctrl->SetState(_T("status"), &status);
			}
			break;
		case WM_KEYUP:
			if (wParam != VK_RETURN) break; // Enter
		case WM_LBUTTONUP:
			{
				IGuiBoard* board = ctrl->GetBoard();
				if (!board) break;
				POINT pt_tmp = {0};
				::GetCursorPos(&pt_tmp);
				CPoint pt(pt_tmp);
				board->ScreenToClient(pt);
				CRect rc;
				ctrl->GetRealRect(rc);

				CGC gc;
				IGuiCtrl::state_t* state = ctrl->GetState(_T("status"), &gc);
				if (!state) break;
				DWORD status = *(DWORD*)(((void**)state->sta_arr)[0]);
				if (status == 2) // 当按下后抬起,视为一次Click
					ctrl->Send(ExDynCast<IGuiObject>(ctrl), WM_COMMAND, BN_CLICKED);

				if (rc.PtInRect(pt))
					status = 1;
				else
					status = 0;
				ctrl->SetState(_T("status"), &status);
			}
			break;
		case WM_MOUSELEAVE:
			{
				DWORD status = 0;
				ctrl->SetState(_T("status"), &status);
			}
			break;
		case WM_PAINT:
			if (lParam)
			{
				CGC gc;

				// 获得属性
				IGuiCtrl::state_t* state = ctrl->GetState(_T("status"), &gc);
				if (!state) break;
				DWORD status = *(DWORD*)(((void**)state->sta_arr)[0]);
				if (ctrl->IsFocus() && status == 0) status = 3;
				if (!ctrl->IsEnabled()) status = 4;

				state = ctrl->GetState(_T("image"), &gc);
				if (!state) break;
				CImage* (image[9]);
				for(int i = 0; i < _countof(image); ++i)
				{
					image[i] = (CImage*)(((void**)state->sta_arr)[i]);
					if (!image[i] || image[i]->IsNull()) break;
				}

				state = ctrl->GetState(_T("color"), &gc);
				if (!state) break;
				pixel_t* pixel = (pixel_t*)(((void**)state->sta_arr)[status]);
				if (!pixel) break;

				state = ctrl->GetState(_T("text"), &gc);
				if (!state) break;
				CText* text = (CText*)(((void**)state->sta_arr)[status]);
				if (!text) break;

				CImage* mem_img = (CImage*)lParam;
				if (!mem_img || mem_img->IsNull()) break;
				CRect rect;
				ctrl->GetRealRect(rect);

				// 处理
				if (m_rcOld != rect)
				{
					LONG r_h = rect.Height() * 5;
					// l-t
					m_imgTmp[0].Set(image[0]->Get());
					// m-t
					m_imgTmp[1].Set
						(
						CImgDeformer::ZomDeform
							(
							image[1]->Get(), 
							rect.Width() - image[0]->GetWidth() - image[2]->GetWidth(), 
							image[1]->GetHeight()
							)
						);
					// r-t
					m_imgTmp[2].Set(image[2]->Get());
					// l-m
					m_imgTmp[3].Set
						(
						CImgDeformer::ZomDeform
							(
							image[3]->Get(), 
							image[3]->GetWidth(), 
							r_h - image[0]->GetHeight() - image[6]->GetHeight()
							)
						);
					// m-m
					m_imgTmp[4].Set
						(
						CImgDeformer::ZomDeform
							(
							image[4]->Get(), 
							rect.Width() - image[3]->GetWidth() - image[5]->GetWidth(), 
							r_h - image[1]->GetHeight() - image[7]->GetHeight()
							)
						);
					// r-m
					m_imgTmp[5].Set
						(
						CImgDeformer::ZomDeform
							(
							image[5]->Get(), 
							image[5]->GetWidth(), 
							r_h - image[2]->GetHeight() - image[8]->GetHeight()
							)
						);
					// l-b
					m_imgTmp[6].Set(image[6]->Get());
					// m-b
					m_imgTmp[7].Set
						(
						CImgDeformer::ZomDeform
							(
							image[7]->Get(), 
							rect.Width() - image[6]->GetWidth() - image[8]->GetWidth(), 
							image[7]->GetHeight()
							)
						);
					// r-b
					m_imgTmp[8].Set(image[8]->Get());
					// Save
					m_rcOld = rect;
				}

				// 绘图
				CImgRenderer::Render(mem_img->Get(), mem_img->Get(), rect, CPoint(), &CFilterFill(*pixel));
				// l-t
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp[0], 
					CRect
						(
						rect.Left(), 
						rect.Top(), 
						rect.Left() + m_imgTmp[0].GetWidth(), 
						rect.Top() + m_imgTmp[0].GetHeight() / 5
						), 
					CPoint(0, m_imgTmp[0].GetHeight() * status / 5)
					);
				// m-t
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp[1], 
					CRect
						(
						rect.Left() + m_imgTmp[0].GetWidth(), 
						rect.Top(), 
						rect.Right() - m_imgTmp[2].GetWidth(), 
						rect.Top() + m_imgTmp[1].GetHeight() / 5
						), 
					CPoint(0, m_imgTmp[1].GetHeight() * status / 5)
					);
				// r-t
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp[2], 
					CRect
						(
						rect.Right() - m_imgTmp[2].GetWidth(), 
						rect.Top(), 
						rect.Right(), 
						rect.Top() + m_imgTmp[2].GetHeight() / 5
						), 
					CPoint(0, m_imgTmp[2].GetHeight() * status / 5)
					);
				// l-m
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp[3], 
					CRect
						(
						rect.Left(), 
						rect.Top() + m_imgTmp[0].GetHeight() / 5, 
						rect.Left() + m_imgTmp[3].GetWidth(), 
						rect.Bottom() - m_imgTmp[6].GetHeight() / 5
						), 
					CPoint(0, m_imgTmp[3].GetHeight() * status / 5)
					);
				// m-m
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp[4], 
					CRect
						(
						rect.Left() + m_imgTmp[3].GetWidth(), 
						rect.Top() + m_imgTmp[1].GetHeight() / 5, 
						rect.Right() - m_imgTmp[5].GetWidth(), 
						rect.Bottom() - m_imgTmp[7].GetHeight() / 5
						), 
					CPoint(0, m_imgTmp[4].GetHeight() * status / 5)
					);
				// r-m
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp[5], 
					CRect
						(
						rect.Right() - m_imgTmp[5].GetWidth(), 
						rect.Top() + m_imgTmp[2].GetHeight() / 5, 
						rect.Right(), 
						rect.Bottom() - m_imgTmp[8].GetHeight() / 5
						), 
					CPoint(0, m_imgTmp[5].GetHeight() * status / 5)
					);
				// l-b
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp[6], 
					CRect
						(
						rect.Left(), 
						rect.Bottom() - m_imgTmp[6].GetHeight() / 5, 
						rect.Left() + m_imgTmp[6].GetWidth(), 
						rect.Bottom()
						), 
					CPoint(0, m_imgTmp[6].GetHeight() * status / 5)
					);
				// m-b
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp[7], 
					CRect
						(
						rect.Left() + m_imgTmp[6].GetWidth(), 
						rect.Bottom() - m_imgTmp[7].GetHeight() / 5, 
						rect.Right() - m_imgTmp[8].GetWidth(), 
						rect.Bottom()
						), 
					CPoint(0, m_imgTmp[7].GetHeight() * status / 5)
					);
				// r-b
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp[8], 
					CRect
						(
						rect.Right() - m_imgTmp[8].GetWidth(), 
						rect.Bottom() - m_imgTmp[8].GetHeight() / 5, 
						rect.Right(), 
						rect.Bottom()
						), 
					CPoint(0, m_imgTmp[8].GetHeight() * status / 5)
					);
				// 绘文字
				CImage txt_img(text->GetImage());
				if (!txt_img.IsNull())
					CImgRenderer::Render
						(
						mem_img->Get(), txt_img, 
						CRect
							(
							rect.Left() + (rect.Width() - txt_img.GetWidth()) / 2, 
							rect.Top() + (rect.Height() - txt_img.GetHeight()) / 2, 
							rect.Right(), 
							rect.Bottom()
							), 
						CPoint()
						);
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiButtonEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiButtonEvent_hpp__*/