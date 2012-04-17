// Copyright 2011-2012, 木头云
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
// Date:	2012-03-16
// Version:	1.0.0018.2349
//
// History:
//	- 1.0.0001.2202(2011-05-23)	+ 添加控件消息转发时的特殊消息处理(WM_PAINT)
//	- 1.0.0002.1611(2011-05-25)	# 修正当控件被设置为不可见并且其特效正在处理时,特效定时器将不会自动关闭的问题
//	- 1.0.0003.1911(2011-05-26)	+ 添加针对性的控件消息转发
//								+ 添加WM_MOUSELEAVE的消息发送
//	- 1.0.0004.1103(2011-05-27)	+ 添加Tab键焦点切换的响应
//	- 1.0.0005.0047(2011-06-08)	+ 将GuiWndEvent拓展为全局通用消息预处理事件类
//	- 1.0.0006.1616(2011-06-09)	# 采用三缓冲绘图修正控件的子控件在半透明贴图时出现的颜色失真
//	- 1.0.0007.1232(2011-06-14)	# 当程序退出CGuiWndEvent的静态变量均析构后,MouseLeaveCheck仍然被调用导致内存访问异常的问题
//	- 1.0.0008.1654(2011-06-22)	# 修正GuiWndEvent没有向子控件通知焦点改变事件的问题
//	- 1.0.0009.1630(2011-06-28)	# 修正GuiWndEvent无法向下发送WM_SHOWWINDOW消息的问题
//								+ 添加事件捕获支持,允许控件设置所有消息的捕获
//	- 1.0.0010.1535(2011-07-01)	+ WM_SIZE不能继续向下转发
//								+ 处理控件消息时WM_SHOWWINDOW消息不应该向下转发
//	- 1.0.0011.1330(2011-07-07)	# 修正WM_SHOWWINDOW消息会被第一层控件截断的问题(WM_SHOWWINDOW应该部分向下转发)
//	- 1.0.0012.1702(2011-07-15)	^ 优化WM_PAINT时位图覆盖的内存消耗及时间效率
//	- 1.0.0013.1958(2011-07-16)	^ 采用剪切区方式优化WM_PAINT时所有内存缓冲位图的效率
//	- 1.0.0014.1538(2011-08-04)	= 由于WM_COMMAND与WM_NOTIFY消息是子控件发给父控件的消息,因此不再向下转发这两个消息
//	- 1.0.0015.2236(2011-08-15)	# 修正当某个控件不可见或无法拥有焦点时,焦点切换将被卡在它前一个控件那里无法继续的问题
//	- 1.0.0016.1724(2012-02-29)	^ 将CGuiWndEvent::BaseSend()中与控件相关的部分独立
//								% 调整并完善CGuiWndEvent中的滚动条控制逻辑
//	- 1.0.0017.1601(2012-03-01)	- 不再在GuiWndEvent的滚动条逻辑中自动控制滚动条关联窗口的大小
//	- 1.0.0018.2349(2012-03-16)	^ 绘图时传递的参数由图片对象改为带有独立剪切区与坐标的画布对象,将绘图工作彻底浓缩到一张全局位图上进行
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
			// 给尚未离开的控件发送消息
			if (s_MLMove)
			{
				s_MLMove->SendMessage(WM_MOUSELEAVE);
				s_MLMove = NULL;
			}
			s_MLCheckWD = NULL;
		}
	}

	// 窗口消息转发
	BOOL m_ShiftDown;
	IGuiCtl* m_pOldFoc;
	LRESULT WndSend(IGuiWnd* pGui, UINT nMessage, WPARAM wParam, LPARAM lParam, LRESULT lrDef = 0)
	{
		if (!pGui) return NULL;
		// 向控件转发消息
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
				// 预存鼠标移动
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
				if (ctl && ctl->IsEnabled()) ctl->SetFocus();
				break;
			}
			if (!ctl || !ctl->IsEnabled()) goto EndWndSend;
			// 初始化返回值
			ctl->SetResult(lrDef); // 发送消息时,让控件对象收到上一个控件的处理结果
			// 转发消息
			ctl->SendMessage(nMessage, wParam, lParam);
			if(!ctl->IsValid()) return NULL;
			// 判断返回值
			lrDef = ctl->GetResult(lrDef);
		}
		else
		if (nMessage >= WM_KEYFIRST && 
			nMessage <= WM_KEYLAST)
		{
			IGuiCtl* ctl = IGuiCtl::GetFocus();
			if (!ctl || !ctl->IsEnabled()) goto EndWndSend;
			// 初始化返回值
			ctl->SetResult(lrDef); // 发送消息时,让控件对象收到上一个控件的处理结果
			// 转发消息
			ctl->SendMessage(nMessage, wParam, lParam);
			if(!ctl->IsValid()) return NULL;
			// 判断返回值
			lrDef = ctl->GetResult(lrDef);
			// 处理焦点切换
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
	// 控件消息转发
	LRESULT CtlSend(IGuiCtl* pGui, UINT nMessage, WPARAM wParam, LPARAM lParam, LRESULT lrDef = 0)
	{
		if (!pGui || !pGui->IsVisible()) return lrDef;
		// 当控件可透过时过滤消息
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
		// 向控件转发消息
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
	// 基础全局消息转发
	LRESULT BaseSend(IGuiBase* pGui, UINT nMessage, WPARAM wParam, LPARAM lParam, LRESULT lrDef = 0)
	{
		if (!pGui) return 0;
		// 向控件转发消息
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
			// 得到全局图片
			CGraph* mem_img = (CGraph*)lParam;
			if (!mem_img || mem_img->IsNull()) goto EndBaseSend;
			// 调校剪切区
			CRect clp_rct, rect;
			pGui->GetClipBox(clp_rct);
			if (ExDynCast<IGuiCtl>(pGui))
			{
				pGui->GetWindowRect(rect);
				clp_rct.Offset(-rect.pt1);
			}
			pGui->GetClientRect(rect);
			// 遍历控件列表
			for(IGuiBase::list_t::iterator_t ite = pGui->GetComp().Head(); ite != pGui->GetComp().Tail(); ++ite)
			{
				IGuiCtl* ctl = (IGuiCtl*)(*ite);
				if(!ctl) continue;
				// 转发消息
				if (ctl->IsVisible() && ctl->IsDisplayed())
				{
					// 初始化返回值
					ctl->SetResult(lrDef); // 发送消息时,让控件对象收到上一个控件的处理结果
					// 获取控件区域
					ctl->SetClipBox(clp_rct);
					CRect ctl_rct; CPoint clp_pnt;
					ctl->GetWindowRect(ctl_rct);
					clp_pnt = ctl_rct.pt1;
					clp_pnt.Offset(-clp_rct.pt1);
					ctl_rct.Inter(rect);
					ctl_rct.Offset(-clp_rct.pt1);
					if (ctl_rct.Width() > 0 && ctl_rct.Height() > 0)
					{
						// 创建控件图片
						CGraph ctl_img(mem_img->GetImage(), ctl_rct, clp_pnt);
						// 控件绘图
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
					// 判断返回值
					lrDef = ctl->GetResult(lrDef);
				}
				else
				{
					// 停止特效
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
				// 初始化返回值
				ctl->SetResult(lrDef); // 发送消息时,让控件对象收到上一个控件的处理结果
				// 转发消息
				ctl->SendMessage(nMessage, wParam, lParam);
				if(!ctl->IsValid()) return NULL;
				// 判断返回值
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
				// 初始化返回值
				ctl->SetResult(lrDef); // 发送消息时,让控件对象收到上一个控件的处理结果
				// 转发消息
				ctl->SendMessage(nMessage, wParam, lParam);
				if(!ctl->IsValid()) return NULL;
				// 判断返回值
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
		// 筛选消息
		LRESULT ret = 0;
		IGuiWnd* wnd = ExDynCast<IGuiWnd>(pGui);
		if (wnd)
		{
			switch( nMessage )
			{
			case WM_SHOWWINDOW:
				// 鼠标离开事件检测定时器
				if (s_MLCheckID == 0)
					s_MLCheckID = ::SetTimer(NULL, 0, 100, MouseLeaveCheck);
				ret = WndSend(wnd, nMessage, wParam, lParam);
				break;
			case WM_PAINT:
				if (wnd->IsCusPaint())
				{
					PAINTSTRUCT ps = {0};
					HDC hdc = ::BeginPaint(wnd->GethWnd(), &ps);
					// 获取缓存区域
					CRect rect;
					::GetClipBox(hdc, (LPRECT)&rect);
				//	ExTrace(_T("%d, %d, %d, %d\n"), rect.Left(), rect.Right(), rect.Top(), rect.Bottom());
					if (rect.IsNull())
						wnd->GetClientRect(rect);
				//	else /*此段仅在调试时做断点使用*/
				//	if (rect.pt1 != CPoint())
				//		rect = rect;
					wnd->SetClipBox(rect);
					// 构建绘图缓存
					CImage mem_img;
					mem_img.Create(rect.Width(), rect.Height());
					if (wnd->IsColorKey())
						CImgFilter::Filter(mem_img, rect, &CFilterBrush(wnd->GetColorKey()));
					// 覆盖控件绘图
					CGraph wnd_img(mem_img);
					ret = WndSend(wnd, nMessage, wParam, (LPARAM)&wnd_img);
					// 覆盖缓存绘图
					CDC mem_dc;
					mem_dc.Create();
					mem_dc.SetObject(mem_img.Get());
					wnd->LayeredWindow(hdc, mem_dc);
					mem_dc.Delete();
					// 结束绘图
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