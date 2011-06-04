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
// GuiWndEvent - 窗口事件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-27
// Version:	1.0.0004.1103
//
// History:
//	- 1.0.0001.2202(2011-05-23)	+ 添加控件消息转发时的特殊消息处理(WM_PAINT)
//	- 1.0.0002.1611(2011-05-25)	# 修正当控件被设置为不可见并且其特效正在处理时,特效定时器将不会自动关闭的问题
//	- 1.0.0003.1911(2011-05-26)	+ 添加针对性的控件消息转发
//								+ 添加WM_MOUSELEAVE的消息发送
//	- 1.0.0004.1103(2011-05-27)	+ 添加Tab键焦点切换的响应
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
	// 鼠标离开检测
	static UINT_PTR s_MLCheckID;
	static HWND s_MLCheckWD;
	static IGuiCtrl* s_MLMove;
	static void CALLBACK MouseLeaveCheck(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
	{
		// 得到鼠标坐标
		POINT pt_tmp = {0};
		::GetCursorPos(&pt_tmp);
		// 得到对应的窗口句柄
		hWnd = ::WindowFromPoint(pt_tmp);
		// 给原先的窗口句柄发送消息
		if (s_MLCheckWD && s_MLCheckWD != hWnd)
		{
			::SendMessage(s_MLCheckWD, WM_MOUSELEAVE, 0, NULL);
			s_MLCheckWD = NULL;
		}
	}

	// 消息转发
	bool m_ShiftDown;
	LRESULT WndSend(IGuiBoard* pGui, UINT nMessage, WPARAM wParam, LPARAM lParam, LRESULT lrDef = 0)
	{
		if (!pGui) return 0;
		IGuiCtrl* ctrl = NULL;
		// 向控件转发消息
		switch (nMessage)
		{
			// 鼠标消息
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
					// 预存鼠标移动
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
					// 设置控件焦点
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
				// 初始化返回值
				ctrl->SetResult(lrDef); // 发送消息时,让控件对象收到上一个控件的处理结果
				// 转发消息
				ctrl->Send(ExDynCast<IGuiObject>(ctrl), nMessage, wParam, lParam);
				// 判断返回值
				lrDef = ctrl->GetResult(lrDef);
			}
			break;
		case WM_MOUSELEAVE:
			// 给尚未离开的控件发送消息
			if (s_MLMove)
			{
				s_MLMove->Send(ExDynCast<IGuiObject>(s_MLMove), WM_MOUSELEAVE);
				s_MLMove = NULL;
			}
			break;
			// 键盘消息
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
				// 初始化返回值
				ctrl->SetResult(lrDef); // 发送消息时,让控件对象收到上一个控件的处理结果
				// 转发消息
				ctrl->Send(ExDynCast<IGuiObject>(ctrl), nMessage, wParam, lParam);
				// 判断返回值
				lrDef = ctrl->GetResult(lrDef);
				// 处理焦点切换
				if (WM_KEYDOWN == nMessage)
				{
					if (wParam == VK_TAB)
					{
						IGuiComp* comp = ctrl->GetParent();
						IGuiComp::list_t::iterator_t ite = comp->Find(ExDynCast<IGuiComp>(ctrl));
						if (ite == comp->GetChildren().Tail()) break;
						if (m_ShiftDown)
						{
							if (ite == comp->GetChildren().Head())
								ite = comp->GetChildren().Last();
							else
								--ite;
						}
						else
						{
							if (ite == comp->GetChildren().Last())
								ite = comp->GetChildren().Head();
							else
								++ite;
						}
						IGuiCtrl* next = ExDynCast<IGuiCtrl>(*ite);
						if (!next) break;
						next->SetFocus();
					}
					else
					if (wParam == VK_SHIFT)
						m_ShiftDown = true;
				}
				else
				if (WM_KEYUP == nMessage)
					if (wParam == VK_SHIFT)
						m_ShiftDown = false;
			}
			break;
			// 焦点消息
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			pGui->Invalidate();
			if (WM_KILLFOCUS == nMessage)
				m_ShiftDown = false;
			break;
		case WM_PAINT:
			{
				// 得到全局图片
				CImage* glb_img = (CImage*)lParam;
				if (!glb_img) break;
				// 遍历控件列表
				for(IGuiBoard::list_t::iterator_t ite = pGui->GetChildren().Head(); ite != pGui->GetChildren().Tail(); ++ite)
				{
					ctrl = ExDynCast<IGuiCtrl>(*ite);
					if (!ctrl) continue;
					// 初始化返回值
					ctrl->SetResult(lrDef); // 发送消息时,让控件对象收到上一个控件的处理结果
					// 转发消息
					if (ctrl->IsVisible())
					{
						// 创建控件图片
						CRect ctl_rct;
						ctrl->GetWindowRect(ctl_rct);
						CImage ctl_img;
						ctl_img.Create(ctl_rct.Width(), ctl_rct.Height());
						// 控件绘图
						IGuiEffect* eff = ctrl->GetEffect();
						if (eff)
						{
							if(!eff->IsInit())
								eff->Init(ctl_img);
							ctrl->Send(*ite, nMessage, wParam, (LPARAM)&ctl_img);
							eff->Show(*ite, ctl_img);
						}
						else
							ctrl->Send(*ite, nMessage, wParam, (LPARAM)&ctl_img);
						// 覆盖全局绘图
						CImgRenderer::Render(glb_img->Get(), ctl_img, ctl_rct, CPoint());
					}
					else
					{
						// 停止特效
						IGuiEffect* eff = ctrl->GetEffect();
						if (eff) eff->KillTimer(pGui->GethWnd());
					}
					// 判断返回值
					lrDef = ctrl->GetResult(lrDef);
				}
			}
			break;
		default:
			for(IGuiBoard::list_t::iterator_t ite = pGui->GetChildren().Head(); ite != pGui->GetChildren().Tail(); ++ite)
			{
				ctrl = ExDynCast<IGuiCtrl>(*ite);
				if (!ctrl) continue;
				// 初始化返回值
				ctrl->SetResult(lrDef); // 发送消息时,让控件对象收到上一个控件的处理结果
				// 转发消息
				ctrl->Send(*ite, nMessage, wParam, lParam);
				// 判断返回值
				lrDef = ctrl->GetResult(lrDef);
			}
		}
		return lrDef;
	}

public:
	CGuiWndEvent()
		: m_ShiftDown(false)
	{}

	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiBoard* board = ExDynCast<IGuiBoard>(pGui);
		if (!board) return;
		// 筛选消息
		LRESULT ret = 0;
		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			// 鼠标离开事件检测定时器
			if (s_MLCheckID == 0)
				s_MLCheckID = ::SetTimer(NULL, 0, 100, MouseLeaveCheck);
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps = {0};
				HDC hdc = ::BeginPaint(board->GethWnd(), &ps);
				// 构建绘图缓存
				CRect rect;
				board->GetClientRect(rect);
				CImage mem_img;
				mem_img.Create(rect.Width(), rect.Height());
				// 覆盖控件绘图
				ret = WndSend(board, nMessage, wParam, (LPARAM)&mem_img);
				// 覆盖缓存绘图
				CGraph mem_grp;
				mem_grp.Create();
				mem_grp.SetObject(mem_img.Get());
				board->LayeredWindow(hdc, mem_grp);
				// 结束绘图
				mem_grp.Delete();
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