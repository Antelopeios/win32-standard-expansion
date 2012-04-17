// Copyright 2011-2012, ľͷ��
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
// Date:	2012-03-16
// Version:	1.0.0018.2349
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
//	- 1.0.0016.1724(2012-02-29)	^ ��CGuiWndEvent::BaseSend()����ؼ���صĲ��ֶ���
//								% ����������CGuiWndEvent�еĹ����������߼�
//	- 1.0.0017.1601(2012-03-01)	- ������GuiWndEvent�Ĺ������߼����Զ����ƹ������������ڵĴ�С
//	- 1.0.0018.2349(2012-03-16)	^ ��ͼʱ���ݵĲ�����ͼƬ�����Ϊ���ж���������������Ļ�������,����ͼ��������Ũ����һ��ȫ��λͼ�Ͻ���
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
	static UINT_PTR s_MLCheckID;
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
				s_MLMove->SendMessage(WM_MOUSELEAVE);
				s_MLMove = NULL;
			}
			s_MLCheckWD = NULL;
		}
	}

	// ������Ϣת��
	BOOL m_ShiftDown;
	IGuiCtl* m_pOldFoc;
	LRESULT WndSend(IGuiWnd* pGui, UINT nMessage, WPARAM wParam, LPARAM lParam, LRESULT lrDef = 0)
	{
		if (!pGui) return NULL;
		// ��ؼ�ת����Ϣ
		if (nMessage >= WM_MOUSEFIRST && 
			nMessage <= WM_MOUSELAST || 
			nMessage >= WM_NCMOUSEMOVE && 
			nMessage <= WM_NCMBUTTONDBLCLK || 
			nMessage == WM_NCHITTEST)
		{
			IGuiCtl* ctl = ExDynCast<IGuiCtl>(IGuiBase::GetCapture());
			if (!ctl)
			{
				POINT pt_tmp = {0};
				::GetCursorPos(&pt_tmp);
				CPoint pt(pt_tmp);
				pGui->ScreenToClient(pt);
				ctl = ExDynCast<IGuiCtl>(pGui->GetPtCtrl(pt));
			}
			switch (nMessage)
			{
				// Ԥ������ƶ�
			case WM_MOUSEMOVE:
			case WM_NCMOUSEMOVE:
				{
					s_MLCheckWD = pGui->GethWnd();
					IGuiCtl* cur = ctl;
					if (cur == s_MLMove) break;
					if (s_MLMove && s_MLMove->IsValid())
					{
						s_MLMove->SendMessage(WM_MOUSELEAVE);
						if(!s_MLMove->IsValid()) return NULL;
					}
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
				if (ctl && ctl->IsEnabled()) ctl->SetFocus();
				break;
			}
			if (!ctl || !ctl->IsEnabled()) goto EndWndSend;
			// ��ʼ������ֵ
			ctl->SetResult(lrDef); // ������Ϣʱ,�ÿؼ������յ���һ���ؼ��Ĵ�����
			// ת����Ϣ
			ctl->SendMessage(nMessage, wParam, lParam);
			if(!ctl->IsValid()) return NULL;
			// �жϷ���ֵ
			lrDef = ctl->GetResult(lrDef);
		}
		else
		if (nMessage >= WM_KEYFIRST && 
			nMessage <= WM_KEYLAST)
		{
			IGuiCtl* ctl = IGuiCtl::GetFocus();
			if (!ctl || !ctl->IsEnabled()) goto EndWndSend;
			// ��ʼ������ֵ
			ctl->SetResult(lrDef); // ������Ϣʱ,�ÿؼ������յ���һ���ؼ��Ĵ�����
			// ת����Ϣ
			ctl->SendMessage(nMessage, wParam, lParam);
			if(!ctl->IsValid()) return NULL;
			// �жϷ���ֵ
			lrDef = ctl->GetResult(lrDef);
			// �������л�
			if (WM_KEYDOWN == nMessage)
			{
				if (wParam == VK_TAB)
				{
					IGuiComp* comp = ctl->GetParent();
					IGuiComp::list_t::iterator_t ite = comp->FindComp(ExDynCast<IGuiComp>(ctl));
					if (ite == comp->GetComp().Tail()) goto EndWndSend;
					if (m_ShiftDown)
					{
						if (ite == comp->GetComp().Head())
							ite = comp->GetComp().Last();
						else
							--ite;
						if (!IGuiCtl::IsEffect(ExDynCast<IGuiCtl>(*ite)))
						{
							if (ite == comp->GetComp().Head())
								ite = comp->GetComp().Last();
							else
								--ite;
						}
					}
					else
					{
						if (ite == comp->GetComp().Last())
							ite = comp->GetComp().Head();
						else
							++ite;
						if (!IGuiCtl::IsEffect(ExDynCast<IGuiCtl>(*ite)))
						{
							if (ite == comp->GetComp().Last())
								ite = comp->GetComp().Head();
							else
								++ite;
						}
					}
					IGuiCtl* next = ExDynCast<IGuiCtl>(*ite);
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
			IGuiCtl::SetFocus(m_pOldFoc);
			pGui->Invalidate();
		}
		else
		if (nMessage == WM_KILLFOCUS)
		{
			m_ShiftDown = FALSE;
			m_pOldFoc = IGuiCtl::SetFocus(NULL);
			pGui->Invalidate();
		}
		else
			lrDef = BaseSend((IGuiBase*)pGui, nMessage, wParam, lParam, lrDef);
	EndWndSend:
		return lrDef;
	}
	// �ؼ���Ϣת��
	LRESULT CtlSend(IGuiCtl* pGui, UINT nMessage, WPARAM wParam, LPARAM lParam, LRESULT lrDef = 0)
	{
		if (!pGui || !pGui->IsVisible()) return lrDef;
		// ���ؼ���͸��ʱ������Ϣ
		if (pGui->IsThrough())
		{
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
				return lrDef;
			}
		}
		// ��ؼ�ת����Ϣ
		if (nMessage == WM_SIZE)
		{
			CSize all_line, fra_line;
			pGui->GetAllRect(all_line);
			pGui->GetFraRect(fra_line);
			if (pGui->GetScroll(TRUE))
			{
				IGuiCtl* scr = pGui->GetScroll(TRUE);
				if (all_line.cy > fra_line.cy)
				{
					if(!scr->IsVisible())
						scr->SetVisible(TRUE);
				}
				else
				{
					if (scr->IsVisible())
						scr->SetVisible(FALSE);
				}
			}
			if (pGui->GetScroll(FALSE))
			{
				IGuiCtl* scr = pGui->GetScroll(FALSE);
				if (all_line.cx > fra_line.cx)
				{
					if(!scr->IsVisible())
						scr->SetVisible(TRUE);
				}
				else
				{
					if (scr->IsVisible())
						scr->SetVisible(FALSE); 
				}
			}
		}
		else
		if (nMessage == WM_MOUSEWHEEL)
		{
			if (pGui->GetScroll(TRUE))
				pGui->GetScroll(TRUE)->SendMessage(nMessage, wParam, lParam);
			if (pGui->GetScroll(FALSE))
				pGui->GetScroll(FALSE)->SendMessage(nMessage, wParam, lParam);
		}
		else
			lrDef = BaseSend((IGuiBase*)pGui, nMessage, wParam, lParam, lrDef);
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
			CGraph* mem_img = (CGraph*)lParam;
			if (!mem_img || mem_img->IsNull()) goto EndBaseSend;
			// ��У������
			CRect clp_rct, rect;
			pGui->GetClipBox(clp_rct);
			if (ExDynCast<IGuiCtl>(pGui))
			{
				pGui->GetWindowRect(rect);
				clp_rct.Offset(-rect.pt1);
			}
			pGui->GetClientRect(rect);
			// �����ؼ��б�
			for(IGuiBase::list_t::iterator_t ite = pGui->GetComp().Head(); ite != pGui->GetComp().Tail(); ++ite)
			{
				IGuiCtl* ctl = (IGuiCtl*)(*ite);
				if(!ctl) continue;
				// ת����Ϣ
				if (ctl->IsVisible() && ctl->IsDisplayed())
				{
					// ��ʼ������ֵ
					ctl->SetResult(lrDef); // ������Ϣʱ,�ÿؼ������յ���һ���ؼ��Ĵ�����
					// ��ȡ�ؼ�����
					ctl->SetClipBox(clp_rct);
					CRect ctl_rct; CPoint clp_pnt;
					ctl->GetWindowRect(ctl_rct);
					clp_pnt = ctl_rct.pt1;
					clp_pnt.Offset(-clp_rct.pt1);
					ctl_rct.Inter(rect);
					ctl_rct.Offset(-clp_rct.pt1);
					if (ctl_rct.Width() > 0 && ctl_rct.Height() > 0)
					{
						// �����ؼ�ͼƬ
						CGraph ctl_img(mem_img->GetImage(), ctl_rct, clp_pnt);
						// �ؼ���ͼ
						IGuiEffect* eff = ctl->GetEffect();
						if (eff)
						{
							if (!eff->IsInit()) eff->Init(ctl_img.GetImage());
							ctl->Send(*ite, nMessage, wParam, (LPARAM)&ctl_img);
							if (!ctl->IsValid()) return NULL;
							eff->Show(*ite, ctl_img.GetImage());
						}
						else
						{
							ctl->Send(*ite, nMessage, wParam, (LPARAM)&ctl_img);
							if(!ctl->IsValid()) return NULL;
						}
					}
					// �жϷ���ֵ
					lrDef = ctl->GetResult(lrDef);
				}
				else
				{
					// ֹͣ��Ч
					IGuiEffect* eff = ctl->GetEffect();
					if (eff) eff->KillTimer(pGui->GethWnd());
				}
			}
		}
		else
		if (nMessage == WM_SHOWWINDOW)
		{
		//	ExTrace(_T("0x%04X\n"), nMessage);
			for(IGuiBase::list_t::iterator_t ite = pGui->GetComp().Head(); ite != pGui->GetComp().Tail(); ++ite)
			{
				IGuiCtl* ctl = (IGuiCtl*)(*ite);
				if (!ctl || !ctl->IsVisible()) continue;
				// ��ʼ������ֵ
				ctl->SetResult(lrDef); // ������Ϣʱ,�ÿؼ������յ���һ���ؼ��Ĵ�����
				// ת����Ϣ
				ctl->SendMessage(nMessage, wParam, lParam);
				if(!ctl->IsValid()) return NULL;
				// �жϷ���ֵ
				lrDef = ctl->GetResult(lrDef);
			}
		}
		else
		{
		//	ExTrace(_T("0x%04X\n"), nMessage);
			for(IGuiBase::list_t::iterator_t ite = pGui->GetComp().Head(); ite != pGui->GetComp().Tail(); ++ite)
			{
				IGuiCtl* ctl = (IGuiCtl*)(*ite);
				if (!ctl || !ctl->IsVisible() || !ctl->IsEnabled()) continue;
				// ��ʼ������ֵ
				ctl->SetResult(lrDef); // ������Ϣʱ,�ÿؼ������յ���һ���ؼ��Ĵ�����
				// ת����Ϣ
				ctl->SendMessage(nMessage, wParam, lParam);
				if(!ctl->IsValid()) return NULL;
				// �жϷ���ֵ
				lrDef = ctl->GetResult(lrDef);
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
		if (s_MLCheckID != 0)
			::KillTimer(NULL, s_MLCheckID);
	}

	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		// ɸѡ��Ϣ
		LRESULT ret = 0;
		IGuiWnd* wnd = ExDynCast<IGuiWnd>(pGui);
		if (wnd)
		{
			switch( nMessage )
			{
			case WM_SHOWWINDOW:
				// ����뿪�¼���ⶨʱ��
				if (s_MLCheckID == 0)
					s_MLCheckID = ::SetTimer(NULL, 0, 100, MouseLeaveCheck);
				ret = WndSend(wnd, nMessage, wParam, lParam);
				break;
			case WM_PAINT:
				if (wnd->IsCusPaint())
				{
					PAINTSTRUCT ps = {0};
					HDC hdc = ::BeginPaint(wnd->GethWnd(), &ps);
					// ��ȡ��������
					CRect rect;
					::GetClipBox(hdc, (LPRECT)&rect);
				//	ExTrace(_T("%d, %d, %d, %d\n"), rect.Left(), rect.Right(), rect.Top(), rect.Bottom());
					if (rect.IsNull())
						wnd->GetClientRect(rect);
				//	else /*�˶ν��ڵ���ʱ���ϵ�ʹ��*/
				//	if (rect.pt1 != CPoint())
				//		rect = rect;
					wnd->SetClipBox(rect);
					// ������ͼ����
					CImage mem_img;
					mem_img.Create(rect.Width(), rect.Height());
					if (wnd->IsColorKey())
						CImgFilter::Filter(mem_img, rect, &CFilterBrush(wnd->GetColorKey()));
					// ���ǿؼ���ͼ
					CGraph wnd_img(mem_img);
					ret = WndSend(wnd, nMessage, wParam, (LPARAM)&wnd_img);
					// ���ǻ����ͼ
					CDC mem_dc;
					mem_dc.Create();
					mem_dc.SetObject(mem_img.Get());
					wnd->LayeredWindow(hdc, mem_dc);
					mem_dc.Delete();
					// ������ͼ
					::EndPaint(wnd->GethWnd(), &ps);
				}
				else
					ret = wnd->DefProc(nMessage, wParam, lParam);
				break;
			case WM_ERASEBKGND:
				ret = WndSend(wnd, nMessage, wParam, lParam);
				break;
			default:
				ret = WndSend(wnd, nMessage, wParam, lParam, wnd->DefProc(nMessage, wParam, lParam));
			}
		}
		else
			ret = CtlSend(ExDynCast<IGuiCtl>(pGui), nMessage, wParam, lParam);
		SetResult(ret);
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiWndEvent, IGuiEvent)
UINT_PTR CGuiWndEvent::s_MLCheckID = 0;
HWND CGuiWndEvent::s_MLCheckWD = NULL;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiWndEvent_hpp__*/