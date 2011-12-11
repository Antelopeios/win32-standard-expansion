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
// GuiWndEvent - �����¼�
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-08-15
// Version:	1.0.0015.2236
//
// History:
//	- 1.0.0001.2202(2011-05-23)	+ ��ӿؼ���Ϣת��ʱ��������Ϣ����(WM_PAINT)
//	- 1.0.0002.1611(2011-05-25)	# �������ؼ�������Ϊ���ɼ���������Ч���ڴ���ʱ,��Ч��ʱ���������Զ��رյ�����
//	- 1.0.0003.1911(2011-05-26)	+ �������ԵĿؼ���Ϣת��
//								+ ���WM_MOUSELEAVE����Ϣ����
//	- 1.0.0004.1103(2011-05-27)	+ ���Tab�������л�����Ӧ
//	- 1.0.0005.0047(2011-06-08)	+ ��GuiWndEvent��չΪȫ��ͨ����ϢԤ�����¼���
//	- 1.0.0006.1616(2011-06-09)	# �����������ͼ�����ؼ����ӿؼ��ڰ�͸����ͼʱ���ֵ���ɫʧ��
//	- 1.0.0007.1232(2011-06-14)	# �������˳�CGuiWndEvent�ľ�̬������������,MouseLeaveCheck��Ȼ�����õ����ڴ�����쳣������
//	- 1.0.0008.1654(2011-06-22)	# ����GuiWndEventû�����ӿؼ�֪ͨ����ı��¼�������
//	- 1.0.0009.1630(2011-06-28)	# ����GuiWndEvent�޷����·���WM_SHOWWINDOW��Ϣ������
//								+ ����¼�����֧��,����ؼ�����������Ϣ�Ĳ���
//	- 1.0.0010.1535(2011-07-01)	+ WM_SIZE���ܼ�������ת��
//								+ ����ؼ���ϢʱWM_SHOWWINDOW��Ϣ��Ӧ������ת��
//	- 1.0.0011.1330(2011-07-07)	# ����WM_SHOWWINDOW��Ϣ�ᱻ��һ��ؼ��ضϵ�����(WM_SHOWWINDOWӦ�ò�������ת��)
//	- 1.0.0012.1702(2011-07-15)	^ �Ż�WM_PAINTʱλͼ���ǵ��ڴ����ļ�ʱ��Ч��
//	- 1.0.0013.1958(2011-07-16)	^ ���ü�������ʽ�Ż�WM_PAINTʱ�����ڴ滺��λͼ��Ч��
//	- 1.0.0014.1538(2011-08-04)	= ����WM_COMMAND��WM_NOTIFY��Ϣ���ӿؼ��������ؼ�����Ϣ,��˲�������ת����������Ϣ
//	- 1.0.0015.2236(2011-08-15)	# ������ĳ���ؼ����ɼ����޷�ӵ�н���ʱ,�����л�����������ǰһ���ؼ������޷�����������
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
	// ����뿪���
	static UINT_PTR s_MLCheckID, s_MLCheckCT;
	static HWND s_MLCheckWD;
	static void CALLBACK MouseLeaveCheck(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
	{
		// �õ��������
		POINT pt_tmp = {0};
		::GetCursorPos(&pt_tmp);
		// �õ���Ӧ�Ĵ��ھ��
		hWnd = ::WindowFromPoint(pt_tmp);
		// ��ԭ�ȵĴ��ھ��������Ϣ
		if (s_MLCheckWD && s_MLCheckWD != hWnd)
		{
			// ����δ�뿪�Ŀؼ�������Ϣ
			if (s_MLMove)
			{
				s_MLMove->Send(ExDynCast<IGuiObject>(s_MLMove), WM_MOUSELEAVE);
				s_MLMove = NULL;
			}
			s_MLCheckWD = NULL;
		}
	}

	// ��Ϣת��
	BOOL m_ShiftDown;
	IGuiCtrl* m_pOldFoc;
	LRESULT WndSend(IGuiBoard* pGui, UINT nMessage, WPARAM wParam, LPARAM lParam, LRESULT lrDef = 0)
	{
		if (!pGui) return 0;
		// ��ؼ�ת����Ϣ
		if (nMessage >= WM_MOUSEFIRST && 
			nMessage <= WM_MOUSELAST || 
			nMessage >= WM_NCMOUSEMOVE && 
			nMessage <= WM_NCMBUTTONDBLCLK || 
			nMessage == WM_NCHITTEST)
		{
			IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(IGuiBase::GetCapture());
			if (!ctrl)
			{
				POINT pt_tmp = {0};
				::GetCursorPos(&pt_tmp);
				CPoint pt(pt_tmp);
				pGui->ScreenToClient(pt);
				ctrl = ExDynCast<IGuiCtrl>(pGui->GetPtCtrl(pt));
			}
			switch (nMessage)
			{
				// Ԥ������ƶ�
			case WM_MOUSEMOVE:
			case WM_NCMOUSEMOVE:
				{
					s_MLCheckWD = pGui->GethWnd();
					IGuiCtrl* cur = ctrl;
					if (cur == s_MLMove) break;
					if (s_MLMove)
						s_MLMove->Send(ExDynCast<IGuiObject>(s_MLMove), WM_MOUSELEAVE);
					s_MLMove = cur;
				}
				break;
				// ���ÿؼ�����
			case WM_LBUTTONDOWN:
			case WM_LBUTTONDBLCLK:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONDBLCLK:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONDBLCLK:
			case WM_NCLBUTTONDOWN:
			case WM_NCLBUTTONDBLCLK:
			case WM_NCRBUTTONDOWN:
			case WM_NCRBUTTONDBLCLK:
			case WM_NCMBUTTONDOWN:
			case WM_NCMBUTTONDBLCLK:
				if (ctrl && ctrl->IsEnabled()) ctrl->SetFocus();
				break;
			}
			if (!ctrl || !ctrl->IsEnabled()) goto EndWndSend;
			// ��ʼ������ֵ
			ctrl->SetResult(lrDef); // ������Ϣʱ,�ÿؼ������յ���һ���ؼ��Ĵ�����
			// ת����Ϣ
			ctrl->Send(ExDynCast<IGuiObject>(ctrl), nMessage, wParam, lParam);
			// �жϷ���ֵ
			lrDef = ctrl->GetResult(lrDef);
		}
		else
		if (nMessage >= WM_KEYFIRST && 
			nMessage <= WM_KEYLAST)
		{
			IGuiCtrl* ctrl = IGuiCtrl::GetFocus();
			if (!ctrl || !ctrl->IsEnabled()) goto EndWndSend;
			// ��ʼ������ֵ
			ctrl->SetResult(lrDef); // ������Ϣʱ,�ÿؼ������յ���һ���ؼ��Ĵ�����
			// ת����Ϣ
			ctrl->Send(ExDynCast<IGuiObject>(ctrl), nMessage, wParam, lParam);
			// �жϷ���ֵ
			lrDef = ctrl->GetResult(lrDef);
			// �������л�
			if (WM_KEYDOWN == nMessage)
			{
				if (wParam == VK_TAB)
				{
					IGuiComp* comp = ctrl->GetParent();
					IGuiComp::list_t::iterator_t ite = comp->FindComp(ExDynCast<IGuiComp>(ctrl));
					if (ite == comp->GetChildren().Tail()) goto EndWndSend;
					if (m_ShiftDown)
					{
						if (ite == comp->GetChildren().Head())
							ite = comp->GetChildren().Last();
						else
							--ite;
						if (!IGuiCtrl::IsEffect(ExDynCast<IGuiCtrl>(*ite)))
						{
							if (ite == comp->GetChildren().Head())
								ite = comp->GetChildren().Last();
							else
								--ite;
						}
					}
					else
					{
						if (ite == comp->GetChildren().Last())
							ite = comp->GetChildren().Head();
						else
							++ite;
						if (!IGuiCtrl::IsEffect(ExDynCast<IGuiCtrl>(*ite)))
						{
							if (ite == comp->GetChildren().Last())
								ite = comp->GetChildren().Head();
							else
								++ite;
						}
					}
					IGuiCtrl* next = ExDynCast<IGuiCtrl>(*ite);
					if (!next) goto EndWndSend;
					next->SetFocus();
				}
				else
				if (wParam == VK_SHIFT)
					m_ShiftDown = TRUE;
			}
			else
			if (WM_KEYUP == nMessage)
				if (wParam == VK_SHIFT)
					m_ShiftDown = FALSE;
		}
		else
		if (nMessage == WM_SETFOCUS)
		{
			IGuiCtrl::SetFocus(m_pOldFoc);
			pGui->Invalidate();
		}
		else
		if (nMessage == WM_KILLFOCUS)
		{
			m_ShiftDown = FALSE;
			m_pOldFoc = IGuiCtrl::SetFocus(NULL);
			pGui->Invalidate();
		}
		else
			lrDef = BaseSend((IGuiBase*)pGui, nMessage, wParam, lParam, lrDef);
	EndWndSend:
		return lrDef;
	}
	// ����ȫ����Ϣת��
	LRESULT BaseSend(IGuiBase* pGui, UINT nMessage, WPARAM wParam, LPARAM lParam, LRESULT lrDef = 0)
	{
		if (!pGui) return 0;
		// ��ؼ�ת����Ϣ
		if (nMessage >= WM_MOUSEFIRST && 
			nMessage <= WM_MOUSELAST || 
			nMessage >= WM_NCMOUSEMOVE && 
			nMessage <= WM_NCMBUTTONDBLCLK || 
			nMessage >= WM_KEYFIRST && 
			nMessage <= WM_KEYLAST || 
			nMessage == WM_SETFOCUS || 
			nMessage == WM_KILLFOCUS || 
			nMessage == WM_NCHITTEST || 
			nMessage == WM_COMMAND || 
			nMessage == WM_NOTIFY)
		{
		//	ExTrace(_T("0x%04X\n"), nMessage);
			goto EndBaseSend;
		}
		else
		if (nMessage == WM_PAINT)
		{
			// �õ�ȫ��ͼƬ
			CImage* mem_img = (CImage*)lParam;
			if (!mem_img || mem_img->IsNull()) goto EndBaseSend;
			// �����ؼ��б�
			for(IGuiBase::list_t::iterator_t ite = pGui->GetChildren().Head(); ite != pGui->GetChildren().Tail(); ++ite)
			{
				IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(*ite);
				if (!ctrl) continue;
				// ��ʼ������ֵ
				ctrl->SetResult(lrDef); // ������Ϣʱ,�ÿؼ������յ���һ���ؼ��Ĵ�����
				// ת����Ϣ
				if (ctrl->IsVisible())
				{
					// ��ȡ�ؼ�����
					CRect ctl_rct;
					ctrl->GetWindowRect(ctl_rct);
					CRect rect(ctl_rct), clp_rct;
					pGui->GetClipBox(clp_rct);
					rect.Inter(clp_rct);
					if (rect.Width() > 0 && rect.Height() > 0)
					{
						// ���ü�����
						rect.Offset(-ctl_rct.pt1);
						ctrl->SetClipBox(rect);
						// �����ؼ�ͼƬ
						CImage ctl_img;
						ctl_img.Create(rect.Width(), rect.Height());
						// �ؼ���ͼ
						IGuiEffect* eff = ctrl->GetEffect();
						if (eff)
						{
							if (!eff->IsInit()) eff->Init(ctl_img);
							ctrl->Send(*ite, nMessage, wParam, (LPARAM)&ctl_img);
							eff->Show(*ite, ctl_img);
						}
						else
							ctrl->Send(*ite, nMessage, wParam, (LPARAM)&ctl_img);
						// ����ȫ�ֻ�ͼ
						ctl_rct.Inter(clp_rct);
						ctl_rct.Offset(-clp_rct.pt1);
						IRenderObject* render = NULL;
						if (ExDynCast<IGuiBoard>(pGui))
							render = ExMem::Alloc<CRenderNormal>();
						CImgRenderer::Render(mem_img->Get(), ctl_img, ctl_rct, CPoint(), render);
						ExMem::Free(render);
					}
				}
				else
				{
					// ֹͣ��Ч
					IGuiEffect* eff = ctrl->GetEffect();
					if (eff) eff->KillTimer(pGui->GethWnd());
				}
				// �жϷ���ֵ
				lrDef = ctrl->GetResult(lrDef);
			}
		}
		else
		if (nMessage == WM_SHOWWINDOW)
		{
		//	ExTrace(_T("0x%04X\n"), nMessage);
			for(IGuiBase::list_t::iterator_t ite = pGui->GetChildren().Head(); ite != pGui->GetChildren().Tail(); ++ite)
			{
				IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(*ite);
				if (!ctrl || !ctrl->IsVisible()) continue;
				// ��ʼ������ֵ
				ctrl->SetResult(lrDef); // ������Ϣʱ,�ÿؼ������յ���һ���ؼ��Ĵ�����
				// ת����Ϣ
				ctrl->Send(*ite, nMessage, wParam, lParam);
				// �жϷ���ֵ
				lrDef = ctrl->GetResult(lrDef);
			}
		}
		else
		{
		//	ExTrace(_T("0x%04X\n"), nMessage);
			for(IGuiBase::list_t::iterator_t ite = pGui->GetChildren().Head(); ite != pGui->GetChildren().Tail(); ++ite)
			{
				IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(*ite);
				if (!ctrl || !ctrl->IsEnabled()) continue;
				// ��ʼ������ֵ
				ctrl->SetResult(lrDef); // ������Ϣʱ,�ÿؼ������յ���һ���ؼ��Ĵ�����
				// ת����Ϣ
				ctrl->Send(*ite, nMessage, wParam, lParam);
				// �жϷ���ֵ
				lrDef = ctrl->GetResult(lrDef);
			}
		}
	EndBaseSend:
		return lrDef;
	}

public:
	CGuiWndEvent()
		: m_ShiftDown(FALSE)
		, m_pOldFoc(NULL)
	{}
	~CGuiWndEvent()
	{
		if (--s_MLCheckCT == 0)
			::KillTimer(NULL, s_MLCheckID);
	}

	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiBase* base = ExDynCast<IGuiBase>(pGui);
		if (!base) return;
		// ɸѡ��Ϣ
		if (nMessage == WM_SIZE) return; /*WM_SIZE��Ϣ��Ӧ������ת��*/
		LRESULT ret = 0;
		IGuiBoard* board = ExDynCast<IGuiBoard>(pGui);
		if (board)
		{
			switch( nMessage )
			{
			case WM_SHOWWINDOW:
				// ����뿪�¼���ⶨʱ��
				if (s_MLCheckID == 0)
					s_MLCheckID = ::SetTimer(NULL, 0, 100, MouseLeaveCheck);
				++s_MLCheckCT;
				ret = WndSend(board, nMessage, wParam, lParam);
				break;
			case WM_PAINT:
				if (board->IsCusPaint())
				{
					PAINTSTRUCT ps = {0};
					HDC hdc = ::BeginPaint(board->GethWnd(), &ps);
					// ��ȡ��������
					CRect rect;
					::GetClipBox(hdc, (LPRECT)&rect);
				//	ExTrace(_T("%d, %d, %d, %d\n"), rect.Left(), rect.Right(), rect.Top(), rect.Bottom());
					if (rect.IsNull())
						board->GetClientRect(rect);
				//	else /*�˶ν��ڵ���ʱ���ϵ�ʹ��*/
				//	if (rect.pt1 != CPoint())
				//		rect = rect;
					board->SetClipBox(rect);
					// ������ͼ����
					CImage mem_img;
					mem_img.Create(rect.Width(), rect.Height());
					CGraph mem_grp;
					mem_grp.Create();
					mem_grp.SetObject(mem_img.Get());
					if (board->IsColorKey())
					{
						LOGBRUSH br = {BS_SOLID, board->GetColorKey(), NULL};
						HBRUSH brh = ::CreateBrushIndirect(&br);
						board->GetClientRect(rect);
						::FillRect(mem_grp, &(RECT)rect, brh);
						::DeleteObject(brh);
					}
					// ���ǿؼ���ͼ
					ret = WndSend(board, nMessage, wParam, (LPARAM)&mem_img);
					// ���ǻ����ͼ
					board->LayeredWindow(hdc, mem_grp);
					// ������ͼ
					mem_grp.Delete();
					::EndPaint(board->GethWnd(), &ps);
				}
				else
					ret = board->DefProc(nMessage, wParam, lParam);
				break;
			case WM_ERASEBKGND:
				ret = WndSend(board, nMessage, wParam, lParam);
				break;
			default:
				ret = WndSend(board, nMessage, wParam, lParam, board->DefProc(nMessage, wParam, lParam));
			}
		}
		else
			ret = BaseSend(base, nMessage, wParam, lParam);
		SetResult(ret);
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiWndEvent, IGuiEvent)
UINT_PTR CGuiWndEvent::s_MLCheckID = 0, CGuiWndEvent::s_MLCheckCT = 0;
HWND CGuiWndEvent::s_MLCheckWD = NULL;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiWndEvent_hpp__*/