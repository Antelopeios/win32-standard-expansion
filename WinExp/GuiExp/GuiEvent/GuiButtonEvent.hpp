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
// GuiButtonEvent - ��ť�ؼ��¼�
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-08-11
// Version:	1.0.0009.1808
//
// History:
//	- 1.0.0000.2258(2011-05-25)	@ ��ʼ����CGuiButtonEvent
//								@ �������CGuiButtonEvent�Ļ�ͼ����
//	- 1.0.0001.2302(2011-05-26)	# ����CGuiButtonEvent��ͼ�еĴ���
//								+ CGuiButtonEvent����״̬�仯��Ϣ����
//								# ����갴�º��ƶ�,��ť״̬���Ϊδ���µ�״̬
//								+ ���Ӱ�ť������Ϣת����Enter����Ӧ
//	- 1.0.0002.1020(2011-05-27)	# ����CGuiButtonEvent��WM_KEYUP֮���޷���Ӧ�����л���ͼ
//	- 1.0.0003.1426(2011-06-08)	# ����CGuiButtonEvent����Ӧ�ǹ���������¼�������
//	- 1.0.0004.1708(2011-06-21)	# ��������ť�ؼ�û�м���ͼƬʱCGuiButtonEvent��ͼ���ֵ��ڴ��쳣
//	- 1.0.0005.1430(2011-07-01)	= ����GuiButton�ĸ��µ���GuiButtonEvent
//	- 1.0.0006.1616(2011-08-03)	= ������ť�ؼ����¼�����,����������BM_CLICK��Ϣ,���򸸴��ڷ���BN_CLICKED��WM_COMMAND��Ϣ
//	- 1.0.0007.1708(2011-08-05)	+ GuiButtonEvent�򸸴��ڷ���BN_SETFOCUS��BN_KILLFOCUS��WM_COMMAND��Ϣ
//	- 1.0.0008.1755(2011-08-10)	^ GuiButtonEvent���Զ��Գ������������������ʡ�Դ���
//	- 1.0.0009.1808(2011-08-11)	^ �Ľ�GuiButtonEvent�����ֻ����㷨,�ڸ߶��㹻ʱ�Զ�������������ʾ
//////////////////////////////////////////////////////////////////

#ifndef __GuiButtonEvent_hpp__
#define __GuiButtonEvent_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiCtrl/GuiCtrl.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

EXP_INLINE void GetTxtRect(const CRect& rect, const CSize& txt_clp, DWORD locate, LONG loc_off, CRect& txt_rct)
{
	switch(locate)
	{
	case 0:	// center
		txt_rct.Set
			(
			CPoint
				(
				rect.Left() + (rect.Width() - txt_clp.cx) / 2, 
				rect.Top() + (rect.Height() - txt_clp.cy) / 2
				), 
			CPoint(rect.Right(), rect.Bottom())
			);
		break;
	case 1:	// top
		txt_rct.Set
			(
			CPoint
				(
				rect.Left() + (rect.Width() - txt_clp.cx) / 2, 
				rect.Top() + loc_off
				), 
			CPoint(rect.Right(), rect.Bottom())
			);
		break;
	case 2:	// bottom
		txt_rct.Set
			(
			CPoint
				(
				rect.Left() + (rect.Width() - txt_clp.cx) / 2, 
				rect.Bottom() - txt_clp.cy - loc_off
				), 
			CPoint(rect.Right(), rect.Bottom())
			);
		break;
	case 3:	// left
		txt_rct.Set
			(
			CPoint
				(
				rect.Left() + loc_off, 
				rect.Top() + (rect.Height() - txt_clp.cy) / 2
				), 
			CPoint(rect.Right(), rect.Bottom())
			);
		break;
	case 4:	// right
		txt_rct.Set
			(
			CPoint
				(
				rect.Right() - txt_clp.cx - loc_off, 
				rect.Top() + (rect.Height() - txt_clp.cy) / 2
				), 
			CPoint(rect.Right(), rect.Bottom())
			);
		break;
	}
}

//////////////////////////////////////////////////////////////////

class CGuiButtonEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiButtonEvent, IGuiEvent)

protected:
	CRect m_rcOld;
	CImage m_imgTmp[9];
	CText m_txtTmp, m_txtClp[2];

public:
	~CGuiButtonEvent()
	{}

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		// ������Ϣ
		switch( nMessage )
		{
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			{
				CGC gc;
				IGuiCtrl::state_t* state = ctrl->GetState(_T("status"), &gc);
				if (!state) break;
				DWORD status = (DWORD)state->sta_arr[0];
				if (status != 2)
				{
					status = 1;
					ctrl->SetState(_T("status"), (void*)status);
				}
			}
			break;
		case WM_KEYDOWN:
			if (wParam != VK_RETURN) break; // Enter
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			{
				DWORD status = 2;
				ctrl->SetState(_T("status"), (void*)status);
			}
			break;
		case WM_KEYUP:
			if (wParam != VK_RETURN) break; // Enter
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
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
				DWORD status = (DWORD)(state->sta_arr[0]);
				if (status == 2) // �����º�̧��,��Ϊһ��Click
				{
					ctrl->Send(ExDynCast<IGuiObject>(ctrl), BM_CLICK);
					IGuiCtrl* pare = ExDynCast<IGuiCtrl>(ctrl->GetParent());
					if (pare)
						pare->Send(ExDynCast<IGuiObject>(pare), WM_COMMAND, BN_CLICKED, (LPARAM)ctrl);
				}

				if (rc.PtInRect(pt))
					status = 1;
				else
					status = 0;
				ctrl->SetState(_T("status"), (void*)status);
			}
			break;
		case WM_MOUSELEAVE:
			{
				DWORD status = 0;
				ctrl->SetState(_T("status"), (void*)status);
			}
			break;
		case WM_SETFOCUS:
			{
				IGuiCtrl* pare = ExDynCast<IGuiCtrl>(ctrl->GetParent());
				if (pare)
					pare->Send(ExDynCast<IGuiObject>(pare), WM_COMMAND, BN_SETFOCUS, (LPARAM)ctrl);
			}
			break;
		case WM_KILLFOCUS:
			{
				IGuiCtrl* pare = ExDynCast<IGuiCtrl>(ctrl->GetParent());
				if (pare)
					pare->Send(ExDynCast<IGuiObject>(pare), WM_COMMAND, BN_KILLFOCUS, (LPARAM)ctrl);
			}
			break;
		case WM_PAINT:
			if (lParam)
			{
				CGC gc;

				// �������
				IGuiCtrl::state_t* state = ctrl->GetState(_T("thr_sta"), &gc);
				if (!state) break;
				bool thr_sta = (bool)(state->sta_arr[0]);
				LONG sta_tim = thr_sta ? 3 : 5;

				state = ctrl->GetState(_T("status"), &gc);
				if (!state) break;
				DWORD status = (DWORD)(state->sta_arr[0]);
				if (!thr_sta)
				{
					if (ctrl->IsFocus() && status == 0) status = 3;
					if (!ctrl->IsEnabled()) status = 4;
				}

				state = ctrl->GetState(_T("image"), &gc);
				if (!state) break;
				CImage* (image[9]);
				for(int i = 0; i < _countof(image); ++i)
					image[i] = (CImage*)(state->sta_arr[i]);

				state = ctrl->GetState(_T("color"), &gc);
				if (!state) break;
				pixel_t pixel = *(pixel_t*)(state->sta_arr[status]);

				state = ctrl->GetState(_T("text"), &gc);
				if (!state) break;
				CText* text = (CText*)(state->sta_arr[status]);
				if (!text) break;

				CImage* mem_img = (CImage*)lParam;
				if (!mem_img || mem_img->IsNull()) break;
				CRect rect, clt_rct;
				ctrl->GetClipRect(rect);
				ctrl->GetClientRect(clt_rct);

				// ����
				if (m_rcOld != clt_rct)
				{
					LONG r_h = clt_rct.Height() * sta_tim;
					// l-t
					m_imgTmp[0].Set(image[0]->Get());
					// m-t
					m_imgTmp[1].Set
						(
						CImgDeformer::ZomDeform
							(
							image[1]->Get(), 
							clt_rct.Width() - image[0]->GetWidth() - image[2]->GetWidth(), 
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
							clt_rct.Width() - image[3]->GetWidth() - image[5]->GetWidth(), 
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
							clt_rct.Width() - image[6]->GetWidth() - image[8]->GetWidth(), 
							image[7]->GetHeight()
							)
						);
					// r-b
					m_imgTmp[8].Set(image[8]->Get());
					// Save
					m_rcOld = clt_rct;
				}

				// ��ͼ
				CImgDrawer::Fill(mem_img->Get(), rect, pixel);
				// l-t
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp[0], 
					CRect
						(
						rect.Left(), 
						rect.Top(), 
						rect.Left() + m_imgTmp[0].GetWidth(), 
						rect.Top() + m_imgTmp[0].GetHeight() / sta_tim
						), 
					CPoint(0, m_imgTmp[0].GetHeight() * status / sta_tim)
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
						rect.Top() + m_imgTmp[1].GetHeight() / sta_tim
						), 
					CPoint(0, m_imgTmp[1].GetHeight() * status / sta_tim)
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
						rect.Top() + m_imgTmp[2].GetHeight() / sta_tim
						), 
					CPoint(0, m_imgTmp[2].GetHeight() * status / sta_tim)
					);
				// l-m
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp[3], 
					CRect
						(
						rect.Left(), 
						rect.Top() + m_imgTmp[0].GetHeight() / sta_tim, 
						rect.Left() + m_imgTmp[3].GetWidth(), 
						rect.Bottom() - m_imgTmp[6].GetHeight() / sta_tim
						), 
					CPoint(0, m_imgTmp[3].GetHeight() * status / sta_tim)
					);
				// m-m
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp[4], 
					CRect
						(
						rect.Left() + m_imgTmp[3].GetWidth(), 
						rect.Top() + m_imgTmp[1].GetHeight() / sta_tim, 
						rect.Right() - m_imgTmp[5].GetWidth(), 
						rect.Bottom() - m_imgTmp[7].GetHeight() / sta_tim
						), 
					CPoint(0, m_imgTmp[4].GetHeight() * status / sta_tim)
					);
				// r-m
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp[5], 
					CRect
						(
						rect.Right() - m_imgTmp[5].GetWidth(), 
						rect.Top() + m_imgTmp[2].GetHeight() / sta_tim, 
						rect.Right(), 
						rect.Bottom() - m_imgTmp[8].GetHeight() / sta_tim
						), 
					CPoint(0, m_imgTmp[5].GetHeight() * status / sta_tim)
					);
				// l-b
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp[6], 
					CRect
						(
						rect.Left(), 
						rect.Bottom() - m_imgTmp[6].GetHeight() / sta_tim, 
						rect.Left() + m_imgTmp[6].GetWidth(), 
						rect.Bottom()
						), 
					CPoint(0, m_imgTmp[6].GetHeight() * status / sta_tim)
					);
				// m-b
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp[7], 
					CRect
						(
						rect.Left() + m_imgTmp[6].GetWidth(), 
						rect.Bottom() - m_imgTmp[7].GetHeight() / sta_tim, 
						rect.Right() - m_imgTmp[8].GetWidth(), 
						rect.Bottom()
						), 
					CPoint(0, m_imgTmp[7].GetHeight() * status / sta_tim)
					);
				// r-b
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp[8], 
					CRect
						(
						rect.Right() - m_imgTmp[8].GetWidth(), 
						rect.Bottom() - m_imgTmp[8].GetHeight() / sta_tim, 
						rect.Right(), 
						rect.Bottom()
						), 
					CPoint(0, m_imgTmp[8].GetHeight() * status / sta_tim)
					);

				// ������
				state = ctrl->GetState(_T("locate"), &gc);
				if (!state) break;
				DWORD locate = (DWORD)(state->sta_arr[0]);
				state = ctrl->GetState(_T("loc_off"), &gc);
				if (!state) break;
				LONG loc_off = (LONG)(state->sta_arr[0]);
				CRect txt_rct;
				CSize txt_clp[2];
				CGraph tmp_grp;
				tmp_grp.Create();
				text->GetSize(tmp_grp, txt_clp[0]);
				GetTxtRect(rect, txt_clp[0], locate, loc_off, txt_rct);
				if ((txt_rct.Height() - 6) < (txt_clp[0].cy << 1))
				{
					if (m_txtTmp == (*text) && m_txtClp[1].Empty())
						m_txtClp[0].GetSize(tmp_grp, txt_clp[0]);
					else
					{
						m_txtClp[1].Clear();
						m_txtTmp = (*text);

						m_txtClp[0] = m_txtTmp;
						m_txtClp[0].GetSize(tmp_grp, txt_clp[0]);
						if (txt_clp[0].cx > rect.Width() - 4)
						{
							CString tmp(m_txtClp[0].GetCStr());
							do
							{
								tmp.LastItem() = 0;
								m_txtClp[0].SetString(tmp);
								m_txtClp[0] += _T("...");
								m_txtClp[0].GetSize(tmp_grp, txt_clp[0]);
							} while (!tmp.Empty() && txt_clp[0].cx > rect.Width() - 4);
						}
					}
					GetTxtRect(rect, txt_clp[0], locate, loc_off, txt_rct);
					CImgRenderer::Render(mem_img->Get(), m_txtClp[0].GetImage(), txt_rct, CPoint());
				}
				else
				{
					if (m_txtTmp == (*text) && !m_txtClp[1].Empty())
					{
						m_txtClp[0].GetSize(tmp_grp, txt_clp[0]);
						m_txtClp[1].GetSize(tmp_grp, txt_clp[1]);
					}
					else
					{
						m_txtClp[1].Clear();
						m_txtTmp = (*text);

						m_txtClp[0] = m_txtTmp;
						m_txtClp[0].GetSize(tmp_grp, txt_clp[0]);
						if (txt_clp[0].cx > rect.Width() - 4)
						{
							do
							{
								m_txtClp[0].LastItem() = 0;
								m_txtClp[0].GetSize(tmp_grp, txt_clp[0]);
							} while (!m_txtClp[0].Empty() && txt_clp[0].cx > rect.Width() - 4);
						}

						CString tmp(((LPCTSTR)m_txtTmp) + m_txtClp[0].GetLength());
						m_txtClp[1] = m_txtTmp;
						m_txtClp[1].SetString(tmp);
						m_txtClp[1].GetSize(tmp_grp, txt_clp[1]);
						if (txt_clp[1].cx > rect.Width() - 4)
						{
							do
							{
								tmp.LastItem() = 0;
								m_txtClp[1].SetString(tmp);
								m_txtClp[1] += _T("...");
								m_txtClp[1].GetSize(tmp_grp, txt_clp[1]);
							} while (!tmp.Empty() && txt_clp[1].cx > rect.Width() - 4);
						}
					}
					GetTxtRect(rect, txt_clp[0], locate, loc_off, txt_rct);
					CImgRenderer::Render(mem_img->Get(), m_txtClp[0].GetImage(), txt_rct, CPoint());
					if (!m_txtClp[1].Empty())
					{
						GetTxtRect(rect, txt_clp[1], locate, loc_off - (2 + txt_clp[0].cy), txt_rct);
						CImgRenderer::Render(mem_img->Get(), m_txtClp[1].GetImage(), txt_rct, CPoint());
					}
				}
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CGuiPushBtnEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiPushBtnEvent, IGuiEvent)

protected:
	CRect m_rcOld;
	CImage m_imgTmp;
	CText m_txtTmp, m_txtClp[2];

public:
	~CGuiPushBtnEvent()
	{}

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		// ������Ϣ
		switch( nMessage )
		{
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			{
				CGC gc;
				IGuiCtrl::state_t* state = ctrl->GetState(_T("status"), &gc);
				if (!state) break;
				DWORD status = (DWORD)(state->sta_arr[0]);
				if (status != 2 && status != 3)
				{
					status = 1;
					ctrl->SetState(_T("status"), (void*)status);
				}
			}
			break;
		case WM_KEYDOWN:
			if (wParam != VK_RETURN) break; // Enter
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			{
				CGC gc;
				IGuiCtrl::state_t* state = ctrl->GetState(_T("status"), &gc);
				if (!state) break;
				DWORD status = (DWORD)(state->sta_arr[0]);
				if (status == 3) break;
				status = 2;
				ctrl->SetState(_T("status"), (void*)status);
			}
			break;
		case WM_KEYUP:
			if (wParam != VK_RETURN) break; // Enter
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			{
				CGC gc;
				IGuiCtrl::state_t* state = ctrl->GetState(_T("status"), &gc);
				if (!state) break;
				DWORD status = (DWORD)(state->sta_arr[0]);
				if (status == 3) break;

				IGuiBoard* board = ctrl->GetBoard();
				if (!board) break;
				POINT pt_tmp = {0};
				::GetCursorPos(&pt_tmp);
				CPoint pt(pt_tmp);
				board->ScreenToClient(pt);
				CRect rc;
				ctrl->GetRealRect(rc);

				if (status == 2) // �����º�̧��,��Ϊһ��Click
				{
					ctrl->Send(ExDynCast<IGuiObject>(ctrl), BM_CLICK);
					IGuiCtrl* pare = ExDynCast<IGuiCtrl>(ctrl->GetParent());
					if (pare)
						pare->Send(ExDynCast<IGuiObject>(pare), WM_COMMAND, BN_CLICKED, (LPARAM)ctrl);
				}

				state = ctrl->GetState(_T("status"), &gc);
				if (!state) break;
				status = (DWORD)(state->sta_arr[0]);
				if (status == 3) break;

				if (rc.PtInRect(pt))
					status = 1;
				else
					status = 0;
				ctrl->SetState(_T("status"), (void*)status);
			}
			break;
		case WM_MOUSELEAVE:
			{
				CGC gc;
				IGuiCtrl::state_t* state = ctrl->GetState(_T("status"), &gc);
				if (!state) break;
				DWORD status = (DWORD)(state->sta_arr[0]);
				if (status == 3) break;
				status = 0;
				ctrl->SetState(_T("status"), (void*)status);
			}
			break;
		case WM_SETFOCUS:
			{
				IGuiCtrl* pare = ExDynCast<IGuiCtrl>(ctrl->GetParent());
				if (pare)
					pare->Send(ExDynCast<IGuiObject>(pare), WM_COMMAND, BN_SETFOCUS, (LPARAM)ctrl);
			}
			break;
		case WM_KILLFOCUS:
			{
				IGuiCtrl* pare = ExDynCast<IGuiCtrl>(ctrl->GetParent());
				if (pare)
					pare->Send(ExDynCast<IGuiObject>(pare), WM_COMMAND, BN_KILLFOCUS, (LPARAM)ctrl);
			}
			break;
		case WM_PAINT:
			if (lParam)
			{
				CGC gc;

				// �������
				LONG sta_tim = 4;

				IGuiCtrl::state_t* state = ctrl->GetState(_T("status"), &gc);
				if (!state) break;
				DWORD status = (DWORD)(state->sta_arr[0]);

				state = ctrl->GetState(_T("image"), &gc);
				if (!state) break;
				CImage* image = (CImage*)(state->sta_arr[0]);

				state = ctrl->GetState(_T("color"), &gc);
				if (!state) break;
				pixel_t pixel = *(pixel_t*)(state->sta_arr[status]);

				state = ctrl->GetState(_T("text"), &gc);
				if (!state) break;
				CText* text = (CText*)(state->sta_arr[status]);
				if (!text) break;

				CImage* mem_img = (CImage*)lParam;
				if (!mem_img || mem_img->IsNull()) break;
				CRect rect, clt_rct;
				ctrl->GetClipRect(rect);
				ctrl->GetClientRect(clt_rct);

				// ����
				if (m_rcOld != clt_rct)
				{
					LONG r_h = clt_rct.Height() * sta_tim;
					// m-m
					m_imgTmp.Set(CImgDeformer::ZomDeform(image->Get(), clt_rct.Width(), r_h));
					// Save
					m_rcOld = clt_rct;
				}

				// ��ͼ
				CImgDrawer::Fill(mem_img->Get(), rect, pixel);
				// m-m
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp, rect, 
					CPoint(0, m_imgTmp.GetHeight() * status / sta_tim)
					);

				// ������
				state = ctrl->GetState(_T("locate"), &gc);
				if (!state) break;
				DWORD locate = (DWORD)(state->sta_arr[0]);
				state = ctrl->GetState(_T("loc_off"), &gc);
				if (!state) break;
				LONG loc_off = (LONG)(state->sta_arr[0]);
				CRect txt_rct;
				CSize txt_clp[2];
				CGraph tmp_grp;
				tmp_grp.Create();
				text->GetSize(tmp_grp, txt_clp[0]);
				GetTxtRect(rect, txt_clp[0], locate, loc_off, txt_rct);
				if ((txt_rct.Height() - 6) < (txt_clp[0].cy << 1))
				{
					if (m_txtTmp == (*text) && m_txtClp[1].Empty())
						m_txtClp[0].GetSize(tmp_grp, txt_clp[0]);
					else
					{
						m_txtClp[1].Clear();
						m_txtTmp = (*text);

						m_txtClp[0] = m_txtTmp;
						m_txtClp[0].GetSize(tmp_grp, txt_clp[0]);
						if (txt_clp[0].cx > rect.Width() - 4)
						{
							CString tmp(m_txtClp[0].GetCStr());
							do
							{
								tmp.LastItem() = 0;
								m_txtClp[0].SetString(tmp);
								m_txtClp[0] += _T("...");
								m_txtClp[0].GetSize(tmp_grp, txt_clp[0]);
							} while (!tmp.Empty() && txt_clp[0].cx > rect.Width() - 4);
						}
					}
					GetTxtRect(rect, txt_clp[0], locate, loc_off, txt_rct);
					CImgRenderer::Render(mem_img->Get(), m_txtClp[0].GetImage(), txt_rct, CPoint());
				}
				else
				{
					if (m_txtTmp == (*text) && !m_txtClp[1].Empty())
					{
						m_txtClp[0].GetSize(tmp_grp, txt_clp[0]);
						m_txtClp[1].GetSize(tmp_grp, txt_clp[1]);
					}
					else
					{
						m_txtClp[1].Clear();
						m_txtTmp = (*text);

						m_txtClp[0] = m_txtTmp;
						m_txtClp[0].GetSize(tmp_grp, txt_clp[0]);
						if (txt_clp[0].cx > rect.Width() - 4)
						{
							do
							{
								m_txtClp[0].LastItem() = 0;
								m_txtClp[0].GetSize(tmp_grp, txt_clp[0]);
							} while (!m_txtClp[0].Empty() && txt_clp[0].cx > rect.Width() - 4);
						}

						CString tmp(((LPCTSTR)m_txtTmp) + m_txtClp[0].GetLength());
						m_txtClp[1] = m_txtTmp;
						m_txtClp[1].SetString(tmp);
						m_txtClp[1].GetSize(tmp_grp, txt_clp[1]);
						if (txt_clp[1].cx > rect.Width() - 4)
						{
							do
							{
								tmp.LastItem() = 0;
								m_txtClp[1].SetString(tmp);
								m_txtClp[1] += _T("...");
								m_txtClp[1].GetSize(tmp_grp, txt_clp[1]);
							} while (!tmp.Empty() && txt_clp[1].cx > rect.Width() - 4);
						}
					}
					GetTxtRect(rect, txt_clp[0], locate, loc_off, txt_rct);
					CImgRenderer::Render(mem_img->Get(), m_txtClp[0].GetImage(), txt_rct, CPoint());
					if (!m_txtClp[1].Empty())
					{
						GetTxtRect(rect, txt_clp[1], locate, loc_off - (2 + txt_clp[0].cy), txt_rct);
						CImgRenderer::Render(mem_img->Get(), m_txtClp[1].GetImage(), txt_rct, CPoint());
					}
				}
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiButtonEvent, IGuiEvent)
EXP_IMPLEMENT_DYNCREATE_CLS(CGuiPushBtnEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiButtonEvent_hpp__*/