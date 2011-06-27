// Copyright 2011, ľͷ��
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
// GuiEditEvent - �༭��ؼ��¼�
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-06-21
// Version:	1.0.0000.1055
//
// History:
//	- 1.0.0000.1055(2011-06-21)	@ ��ʼ����GuiEditEvent
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
	bool m_ShiftDown;
	bool m_bFlicker;
	UINT_PTR m_uFlicker;
	IGuiCtrl* m_Ctrl;
	CString::iterator_t m_iteFlicker, m_iteSelect;

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
		: m_ShiftDown(false)
		, m_bFlicker(false)
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

		// ������Ϣ
		switch( nMessage )
		{
		case WM_LBUTTONDOWN:
			// �����ʱ��λ���
			{
				CGC gc;

				// �������
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

				// �����ַ�λ��
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
					if (text->Empty()) break;
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

				if (m_ShiftDown)
				{
					if (!m_iteSelect->InThis(edit)) m_iteSelect = m_iteFlicker;
					m_iteSelect->nIndx = ite2->nIndx;
				}
				else
				{
					m_iteFlicker->nIndx = ite2->nIndx;
					m_iteSelect = m_iteFlicker;
				}
				text->SetString(_T(""));
				tmp_grp.Delete();
				ctrl->Refresh();
			}
			break;
		case WM_KEYDOWN:
			// WM_CHAR���ղ������ַ�����
			if (wParam == VK_SHIFT)
				m_ShiftDown = true;
			else
			{
				CGC gc;
				IGuiCtrl::state_t* state = ctrl->GetState(_T("edit"), &gc);
				if (!state) break;
				CString* edit = (CString*)(((void**)state->sta_arr)[0]);
				if (!edit) break;
				if (!m_iteFlicker->InThis(edit)) m_iteFlicker = edit->Tail();
				// �ж��ַ�
				if (wParam == VK_LEFT)
				{
					if (edit->Empty()) break;
					if (m_ShiftDown)
					{
						if (!m_iteSelect->InThis(edit)) m_iteSelect = m_iteFlicker;
						--m_iteSelect;
					}
					else
					{
						--m_iteFlicker;
						m_iteSelect = m_iteFlicker;
					}
				}
				else
				if (wParam == VK_RIGHT)
				{
					if (edit->Empty()) break;
					if (m_ShiftDown)
					{
						if (!m_iteSelect->InThis(edit)) m_iteSelect = m_iteFlicker;
						++m_iteSelect;
					}
					else
					{
						++m_iteFlicker;
						m_iteSelect = m_iteFlicker;
					}
				}
				else
				if (wParam == VK_DELETE)
				{
					if (edit->Empty()) break;
					// ��λѡ��
					if (!m_iteSelect->InThis(edit)) m_iteSelect = m_iteFlicker;
					CString::iterator_t ite1, ite2;
					if (m_iteSelect->Index() < m_iteFlicker->Index())
					{
						ite1 = m_iteSelect;
						ite2 = m_iteFlicker;
					}
					else
					if (m_iteSelect->Index() > m_iteFlicker->Index())
					{
						ite1 = m_iteFlicker;
						ite2 = m_iteSelect;
					}
					else
						ite1 = ite2 = m_iteFlicker;
					// ɾ��ѡ��
					edit->Del(ite1, ite2->Index() - ite1->Index() + 1);
					m_iteSelect = m_iteFlicker;
				}
				// ˢ�½���
				m_bFlicker = true;
				ctrl->Refresh();
			}
			break;
		case WM_KEYUP:
			if (wParam == VK_SHIFT)
				m_ShiftDown = false;
			break;
		case WM_CHAR:
			// �ַ�����
			{
				CGC gc;
				IGuiCtrl::state_t* state = ctrl->GetState(_T("edit"), &gc);
				if (!state) break;
				CString* edit = (CString*)(((void**)state->sta_arr)[0]);
				if (!edit) break;
				if (!m_iteFlicker->InThis(edit)) m_iteFlicker = edit->Tail();
				// ��λѡ��
				if (!m_iteSelect->InThis(edit)) m_iteSelect = m_iteFlicker;
				CString::iterator_t ite1, ite2;
				if (m_iteSelect->Index() < m_iteFlicker->Index())
				{
					ite1 = m_iteSelect;
					ite2 = m_iteFlicker;
				}
				else
				if (m_iteSelect->Index() > m_iteFlicker->Index())
				{
					ite1 = m_iteFlicker;
					ite2 = m_iteSelect;
				}
				else
					ite1 = ite2 = m_iteFlicker;
				// �ж��ַ�
				if (wParam == VK_BACK)
				{
					if (edit->Empty()) break;
					if (ite1 == ite2)
					{
						--m_iteFlicker;
						edit->Del(m_iteFlicker);
					}
					else
						edit->Del(ite1, ite2->Index() - ite1->Index() + 1);
				}
				else
				if (wParam >= VK_SPACE)
				{
					edit->Del(ite1, ite2->Index() - ite1->Index() + 1);
					m_iteFlicker = ite1;
					edit->Add((TCHAR)wParam, m_iteFlicker);
					++m_iteFlicker;
				}
				m_iteSelect = m_iteFlicker;
				// ˢ�½���
				m_bFlicker = true;
				ctrl->Refresh();
			}
			break;
		case WM_SETFOCUS:
			// ��ý���ʱ��ʾ���
			if (m_uFlicker != 0) break;
			m_uFlicker = ::SetTimer(NULL, 0, 500, FlickerTimer);
			s_TimerToEvent[m_uFlicker] = this;
			break;
		case WM_KILLFOCUS:
			m_ShiftDown = false;
			// ʧȥ����ʱ���ع��
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

				// �������
				IGuiCtrl::state_t* state = ctrl->GetState(_T("status"), &gc);
				if (!state) break;
				DWORD status = *(DWORD*)(((void**)state->sta_arr)[0]);
				if (ctrl->IsFocus() && status == 0) status = 3;
				if (!ctrl->IsEnabled()) status = 4;

				state = ctrl->GetState(_T("text"), &gc);
				if (!state) break;
				CText* text = (CText*)(((void**)state->sta_arr)[status]);
				if (!text) break;
				CText tmp_text = (*text);

				state = ctrl->GetState(_T("edit"), &gc);
				if (!state) break;
				CString* edit = (CString*)(((void**)state->sta_arr)[0]);
				if (!edit) break;

				state = ctrl->GetState(_T("txt_sel_color"), &gc);
				if (!state) break;
				pixel_t* txt_sel_color = (pixel_t*)(((void**)state->sta_arr)[status]);
				if (!txt_sel_color) break;

				state = ctrl->GetState(_T("bkg_sel_color"), &gc);
				if (!state) break;
				pixel_t* bkg_sel_color = (pixel_t*)(((void**)state->sta_arr)[status]);
				if (!bkg_sel_color) break;

				CImage* mem_img = (CImage*)lParam;
				if (!mem_img || mem_img->IsNull()) break;
				CRect rect;
				ctrl->GetClientRect(rect);

				// ��λѡ��
				if (!m_iteSelect->InThis(edit)) m_iteSelect = m_iteFlicker;
				CString::iterator_t ite1, ite2;
				if (m_iteSelect->Index() < m_iteFlicker->Index())
				{
					ite1 = m_iteSelect;
					ite2 = m_iteFlicker;
				}
				else
				if (m_iteSelect->Index() > m_iteFlicker->Index())
				{
					ite1 = m_iteFlicker;
					ite2 = m_iteSelect;
				}
				else
					ite1 = ite2 = m_iteFlicker;
				CSize sz_off, sz_sel;
				if (ite1 != ite2)
				{
					CGraph tmp_grp;
					tmp_grp.Create();

					tmp_text.SetString(*edit);
					tmp_text[ite1] = _T('\0');
					tmp_text.GetSize(tmp_grp, sz_off);

					tmp_text.SetString(*edit);
					tmp_text[ite2] = _T('\0');
					tmp_text.Del(tmp_text.Head(), ite1->Index() + 1);
					tmp_text.GetSize(tmp_grp, sz_sel);

					tmp_grp.Delete();
				}

				// ������
				tmp_text.SetString(*edit);
				CImage txt_img(tmp_text.GetImage());
				if(!txt_img.IsNull())
				{
					CRect sel_rc(sz_off.cx, 0, sz_off.cx + sz_sel.cx, sz_sel.cy);
					CPoint sel_pt(sz_off.cx, 0);
					if (!sel_rc.IsEmpty())
					{
						CImgRenderer::Render
							(
							txt_img, txt_img, 
							sel_rc, sel_pt, 
							&CFilterFillT<CFilterCopy>(*txt_sel_color, 0xf, true)
							);
						CImgRenderer::Render
							(
							txt_img, txt_img, 
							sel_rc, sel_pt, 
							&CFilterFillT<CFilterCopy>(*bkg_sel_color, 0xf, true, tmp_text.GetColor())
							);
					}
					CImgRenderer::Render(mem_img->Get(), txt_img, rect, CPoint());
				}

				// ����
				if (m_bFlicker)
				{
					// ��ù��߶�
					tmp_text.SetString(_T("lq"));
					CGraph tmp_grp;
					tmp_grp.Create();
					CSize sz_flk;
					tmp_text.GetSize(tmp_grp, sz_flk);
					// ������ֿ��
					tmp_text.SetString(*edit);
					tmp_text.GetAt(m_iteFlicker->nIndx) = _T('\0');
					CSize sz_txt;
					tmp_text.GetSize(tmp_grp, sz_txt);
					tmp_grp.Delete();
					// ����
					CImgDrawer::Line(
						mem_img->Get(), 
						CLine(sz_txt.cx, 0, sz_txt.cx, sz_flk.cy), 
						ExRGBA(0, 0, 0, EXP_CM));
				}
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