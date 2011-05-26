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
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2010-05-26
// Version:	1.0.0003.1911
//
// History:
//	- 1.0.0001.2202(2010-05-23)	+ ��ӿؼ���Ϣת��ʱ��������Ϣ����(WM_PAINT)
//	- 1.0.0002.1611(2010-05-25)	# �������ؼ�������Ϊ���ɼ���������Ч���ڴ���ʱ,��Ч��ʱ���������Զ��رյ�����
//	- 1.0.0003.1911(2010-05-26)	+ �������ԵĿؼ���Ϣת��
//								+ ���WM_MOUSELEAVE����Ϣ����
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
	static UINT_PTR s_MLCheckID;
	static HWND s_MLCheckWD;
	static IGuiCtrl* s_MLMove;
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
			::SendMessage(s_MLCheckWD, WM_MOUSELEAVE, 0, NULL);
			s_MLCheckWD = NULL;
		}
	}

	// ��Ϣת��
	LRESULT WndSend(IGuiBoard* pGui, UINT nMessage, WPARAM wParam, LPARAM lParam, LRESULT lrDef = 0)
	{
		if (!pGui) return 0;
		IGuiCtrl* ctrl = NULL;
		// ��ؼ�ת����Ϣ
		switch (nMessage)
		{
			// �����Ϣ
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
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_NCHITTEST:
		case WM_NCMOUSEMOVE:
		case WM_NCLBUTTONUP:
		case WM_NCRBUTTONUP:
		case WM_NCMBUTTONUP:
			{
				POINT pt_tmp = {0};
				::GetCursorPos(&pt_tmp);
			//	ExTrace(_T("0x%04X, (%d, %d)\n"), nMessage, pt_tmp.x, pt_tmp.y);
				CPoint pt(pt_tmp);
				pGui->ScreenToClient(pt);
				ctrl = ExDynCast<IGuiCtrl>(pGui->GetPtCtrl(pt));
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
					if (ctrl) ctrl->SetFocus();
					break;
				}
				if (!ctrl) break;
				// ��ʼ������ֵ
				ctrl->SetResult(lrDef); // ������Ϣʱ,�ÿؼ������յ���һ���ؼ��Ĵ�����
				// ת����Ϣ
				ctrl->Send(ExDynCast<IGuiObject>(ctrl), nMessage, wParam, lParam);
				// �жϷ���ֵ
				lrDef = ctrl->GetResult(lrDef);
			}
			break;
		case WM_MOUSELEAVE:
			// ����δ�뿪�Ŀؼ�������Ϣ
			if (s_MLMove)
			{
				s_MLMove->Send(ExDynCast<IGuiObject>(s_MLMove), WM_MOUSELEAVE);
				s_MLMove = NULL;
			}
			break;
			// ������Ϣ
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
		case WM_DEADCHAR:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_SYSCHAR:
		case WM_SYSDEADCHAR:
			{
				ctrl = IGuiCtrl::GetFocus();
				if (!ctrl) break;
				// ��ʼ������ֵ
				ctrl->SetResult(lrDef); // ������Ϣʱ,�ÿؼ������յ���һ���ؼ��Ĵ�����
				// ת����Ϣ
				ctrl->Send(ExDynCast<IGuiObject>(ctrl), nMessage, wParam, lParam);
				// �жϷ���ֵ
				lrDef = ctrl->GetResult(lrDef);
			}
			break;
			// ������Ϣ
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			pGui->Invalidate();
			break;
		default:
			for(IGuiBoard::list_t::iterator_t ite = pGui->GetChildren().Head(); ite != pGui->GetChildren().Tail(); ++ite)
			{
				ctrl = ExDynCast<IGuiCtrl>(*ite);
				if (!ctrl) continue;
				// ��ʼ������ֵ
				ctrl->SetResult(lrDef); // ������Ϣʱ,�ÿؼ������յ���һ���ؼ��Ĵ�����
				// ת����Ϣ
				switch( nMessage )
				{
				case WM_PAINT:
					if (ctrl->IsVisible())
					{
						IGuiEffect* eff = ctrl->GetEffect();
						if (eff)
						{
							if(!eff->IsInit())
								eff->Init(*(CImage*)lParam);
							ctrl->Send(*ite, nMessage, wParam, lParam);
							eff->Show(*ite, *(CImage*)lParam);
						}
						else
							ctrl->Send(*ite, nMessage, wParam, lParam);
					}
					else
					{
						IGuiEffect* eff = ctrl->GetEffect();
						if (eff) eff->KillTimer(pGui->GethWnd());
					}
					break;
				default:
					ctrl->Send(*ite, nMessage, wParam, lParam);
				}
				// �жϷ���ֵ
				lrDef = ctrl->GetResult(lrDef);
			}
		}
		return lrDef;
	}

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiBoard* board = ExDynCast<IGuiBoard>(pGui);
		if (!board) return;
		// ɸѡ��Ϣ
		LRESULT ret = 0;
		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			// ����뿪�¼���ⶨʱ��
			if (s_MLCheckID == 0)
				s_MLCheckID = ::SetTimer(NULL, 0, 100, MouseLeaveCheck);
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps = {0};
				HDC hdc = ::BeginPaint(board->GethWnd(), &ps);
				// ������ͼ����
				CRect rect;
				board->GetClientRect(rect);
				CImage mem_img;
				mem_img.Create(rect.Width(), rect.Height());
				// ���ǿؼ���ͼ
				ret = WndSend(board, nMessage, wParam, (LPARAM)&mem_img);
				// ���ǻ����ͼ
				CGraph mem_grp;
				mem_grp.Create();
				mem_grp.SetObject(mem_img.Get());
				board->LayeredWindow(hdc, mem_grp);
				// ������ͼ
				mem_grp.Delete();
				mem_img.Delete();
				::EndPaint(board->GethWnd(), &ps);
			}
			break;
		case WM_ERASEBKGND:
			ret = WndSend(board, nMessage, wParam, lParam);
			break;
		default:
			ret = WndSend(board, nMessage, wParam, lParam, board->DefProc(nMessage, wParam, lParam));
		}
		SetResult(ret);
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiWndEvent, IGuiEvent)
UINT_PTR CGuiWndEvent::s_MLCheckID = 0;
HWND CGuiWndEvent::s_MLCheckWD = NULL;
IGuiCtrl* CGuiWndEvent::s_MLMove = NULL;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiWndEvent_hpp__*/