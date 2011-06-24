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
// GuiEditEvent - 编辑框控件事件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-06-21
// Version:	1.0.0000.1055
//
// History:
//	- 1.0.0000.1055(2011-06-21)	@ 开始构建GuiEditEvent
//////////////////////////////////////////////////////////////////

#ifndef __GuiEditEvent_h__
#define __GuiEditEvent_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiCtrl/GuiCtrl.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiEditEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiEditEvent, IGuiEvent)

protected:
	bool m_bFlicker;
	UINT_PTR m_uFlicker;
	IGuiCtrl* m_Ctrl;
	CString::iterator_t m_iteFlicker;

	typedef CMapT<UINT_PTR, CGuiEditEvent*> map_t;
	static map_t s_TimerToEvent;

protected:
	static void CALLBACK FlickerTimer(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
	{
		CGuiEditEvent* pEvt = s_TimerToEvent[idEvent];
		if (!pEvt) return;
		pEvt->m_bFlicker = !(pEvt->m_bFlicker);
		if (pEvt->m_Ctrl) pEvt->m_Ctrl->Refresh();
	}

public:
	CGuiEditEvent()
		: m_bFlicker(false)
		, m_uFlicker(0)
		, m_Ctrl(NULL)
	{}
	~CGuiEditEvent()
	{
		if (m_uFlicker == 0) return;
		::KillTimer(NULL, m_uFlicker);
		s_TimerToEvent.Del(m_uFlicker);
	}

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;
		m_Ctrl = ctrl;

		// 处理消息
		switch( nMessage )
		{
		case WM_LBUTTONDOWN:
			// 鼠标点击时定位光标
			{
				CGC gc;

				// 获得属性
				IGuiCtrl::state_t* state = ctrl->GetState(_T("status"), &gc);
				if (!state) break;
				DWORD status = *(DWORD*)(((void**)state->sta_arr)[0]);
				if (ctrl->IsFocus() && status == 0) status = 3;
				if (!ctrl->IsEnabled()) status = 4;

				state = ctrl->GetState(_T("text"), &gc);
				if (!state) break;
				CText* text = (CText*)(((void**)state->sta_arr)[status]);
				if (!text) break;

				state = ctrl->GetState(_T("edit"), &gc);
				if (!state) break;
				CString* edit = (CString*)(((void**)state->sta_arr)[0]);
				if (!edit) break;

				// 搜索字符位置
				CPoint point(ExLoWord(lParam), ExHiWord(lParam));
				if (!m_iteFlicker->InThis(edit)) m_iteFlicker = edit->Tail();
				CString::iterator_t ite1 = text->Head(), ite2 = text->Head(), ite3 = text->Head();
				ite2->nIndx = m_iteFlicker->nIndx;
				ite3->nIndx = edit->Tail()->nIndx;

				CGraph tmp_grp;
				tmp_grp.Create();
				CSize sz;

				do 
				{
					text->SetString(*edit);
					text->GetAt(ite2) = _T('\0');
					text->GetSize(tmp_grp, sz);
					if (sz.cx > point.x)
						ite3->nIndx = ite2->nIndx;
					else
					if (sz.cx < point.x)
						ite1->nIndx = ite2->nIndx;
					else
						break;
					ite2->nIndx = ((ite1->nIndx + ite3->nIndx) >> 1);
				} while(ite2->nIndx > ite1->nIndx);

				m_iteFlicker->nIndx = ite2->nIndx;
				text->SetString(_T(""));
				tmp_grp.Delete();
				ctrl->Refresh();
			}
			break;
		case WM_CHAR:
			// 字符输入
			if (wParam != VK_RETURN)
			{
				CGC gc;
				IGuiCtrl::state_t* state = ctrl->GetState(_T("edit"), &gc);
				if (!state) break;
				CString* edit = (CString*)(((void**)state->sta_arr)[0]);
				if (!edit) break;

				if (!m_iteFlicker->InThis(edit)) m_iteFlicker = edit->Tail();
				edit->Add((TCHAR)wParam);
				++m_iteFlicker;

				m_bFlicker = true;
				ctrl->Refresh();
			}
			break;
		case WM_SETFOCUS:
			// 获得焦点时显示光标
			if (m_uFlicker != 0) break;
			m_uFlicker = ::SetTimer(NULL, 0, 500, FlickerTimer);
			s_TimerToEvent[m_uFlicker] = this;
			break;
		case WM_KILLFOCUS:
			// 失去焦点时隐藏光标
			if (m_uFlicker == 0) break;
			::KillTimer(NULL, m_uFlicker);
			s_TimerToEvent.Del(m_uFlicker);
			m_uFlicker = 0;
			m_bFlicker = false;
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

				state = ctrl->GetState(_T("text"), &gc);
				if (!state) break;
				CText* text = (CText*)(((void**)state->sta_arr)[status]);
				if (!text) break;

				state = ctrl->GetState(_T("edit"), &gc);
				if (!state) break;
				CString* edit = (CString*)(((void**)state->sta_arr)[0]);
				if (!edit) break;

				CImage* mem_img = (CImage*)lParam;
				if (!mem_img || mem_img->IsNull()) break;
				CRect rect;
				ctrl->GetClientRect(rect);

				// 绘文字
				text->SetString(*edit);
				CImage txt_img(text->GetImage());
				if (!txt_img.IsNull())
					CImgRenderer::Render(mem_img->Get(), txt_img, rect, CPoint());

				// 绘光标
				if (m_bFlicker)
				{
					// 获得光标高度
					text->SetString(_T("lq"));
					CGraph tmp_grp;
					tmp_grp.Create();
					CSize sz_flk;
					text->GetSize(tmp_grp, sz_flk);
					// 获得文字宽度
					text->SetString(*edit);
					text->GetAt(m_iteFlicker->nIndx) = _T('\0');
					CSize sz_txt;
					text->GetSize(tmp_grp, sz_txt);
					tmp_grp.Delete();
					// 画线
					CImgDrawer::Line(
						mem_img->Get(), 
						CLine(sz_txt.cx, 0, sz_txt.cx, sz_flk.cy), 
						ExRGBA(0, 0, 0, EXP_CM));
				}
				text->SetString(_T(""));
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiEditEvent, IGuiEvent)
CGuiEditEvent::map_t CGuiEditEvent::s_TimerToEvent;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiEditEvent_h__*/