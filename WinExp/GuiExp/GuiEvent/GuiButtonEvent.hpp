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
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-08-30
// Version:	1.0.0012.1716
//
// History:
//	- 1.0.0000.2258(2011-05-25)	@ 开始构建CGuiButtonEvent
//								@ 基本完成CGuiButtonEvent的绘图部分
//	- 1.0.0001.2302(2011-05-26)	# 修正CGuiButtonEvent绘图中的错误
//								+ CGuiButtonEvent添加状态变化消息处理
//								# 当鼠标按下后移动,按钮状态会变为未按下的状态
//								+ 添加按钮单击消息转发及Enter键响应
//	- 1.0.0002.1020(2011-05-27)	# 修正CGuiButtonEvent当WM_KEYUP之后无法响应焦点切换绘图
//	- 1.0.0003.1426(2011-06-08)	# 修正CGuiButtonEvent不响应非工作区鼠标事件的问题
//	- 1.0.0004.1708(2011-06-21)	# 修正当按钮控件没有加载图片时CGuiButtonEvent绘图出现的内存异常
//	- 1.0.0005.1430(2011-07-01)	= 根据GuiButton的更新调整GuiButtonEvent
//	- 1.0.0006.1616(2011-08-03)	= 调整按钮控件的事件发送,向自身发送BM_CLICK消息,并向父窗口发送BN_CLICKED的WM_COMMAND消息
//	- 1.0.0007.1708(2011-08-05)	+ GuiButtonEvent向父窗口发送BN_SETFOCUS与BN_KILLFOCUS的WM_COMMAND消息
//	- 1.0.0008.1755(2011-08-10)	^ GuiButtonEvent将自动对超出剪裁区域的文字做省略处理
//	- 1.0.0009.1808(2011-08-11)	^ 改进GuiButtonEvent的文字绘制算法,在高度足够时自动对文字折行显示
//	- 1.0.0010.1533(2011-08-12)	^ GuiButtonEvent的文字折行算法支持自动适应任意高度(上一版本仅能对折)
//								# 修正一处GuiButtonEvent状态判断的逻辑错误
//	- 1.0.0011.1606(2011-08-15)	+ 在GuiButtonEvent中实现icon相关属性的绘图处理
//	- 1.0.0012.1716(2011-08-30)	+ 添加双击按钮时的BN_DOUBLECLICKED消息发送
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
	CText m_txtTmp;
	CImage m_imgClp;
	DWORD m_LocOld;
	LONG m_OffOld;
	CImage m_IconOld, m_IconTmp;

public:
	CGuiButtonEvent()
		: m_LocOld(0)
		, m_OffOld(0)
	{}
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
		case WM_LBUTTONDBLCLK:
		case WM_NCLBUTTONDBLCLK:
			{
				IGuiCtrl* pare = ExDynCast<IGuiCtrl>(ctrl->GetParent());
				if (pare)
					pare->Send(ExDynCast<IGuiObject>(pare), WM_COMMAND, BN_DOUBLECLICKED, (LPARAM)ctrl);
			}
			break;
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			{
				DWORD status = (DWORD)ctrl->GetState(_T("status"));
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

				DWORD status = (DWORD)ctrl->GetState(_T("status"));
				if (status == 2) // 当按下后抬起,视为一次Click
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
				// 获得属性
				int thr_sta = (int)ctrl->GetState(_T("thr_sta"));
				LONG sta_tim = (thr_sta == 1 ? 3 : (thr_sta == -1 ? 1 : 5));

				DWORD status = 0;
				if (thr_sta != -1)
				{
					status = (DWORD)ctrl->GetState(_T("status"));
					if (thr_sta == 0)
					{
						if (ctrl->IsFocus() && status == 0) status = 3;
						if (!ctrl->IsEnabled()) status = 4;
					}
				}

				CImage* image = (CImage*)ctrl->GetState(_T("image"));
				pixel_t pixel = ((pixel_t*)ctrl->GetState(_T("color")))[status];
				CText* text = (CText*)ctrl->GetState(_T("text")) + status;
				CImage* icon = (CImage*)ctrl->GetState(_T("icon"));
				bool glow = (bool)(LONG_PTR)ctrl->GetState(_T("glow"));

				CImage* mem_img = (CImage*)lParam;
				if (!mem_img || mem_img->IsNull()) break;
				CRect rect, clt_rct;
				ctrl->GetClipRect(rect);
				ctrl->GetClientRect(clt_rct);

				// 处理
				if (m_rcOld != clt_rct)
				{
					LONG r_h = clt_rct.Height() * sta_tim;
					// l-t
					m_imgTmp[0].Set(image[0].Get());
					// m-t
					m_imgTmp[1].Set
						(
						CImgDeformer::ZomDeform
							(
							image[1].Get(), 
							clt_rct.Width() - image[0].GetWidth() - image[2].GetWidth(), 
							image[1].GetHeight()
							)
						);
					// r-t
					m_imgTmp[2].Set(image[2].Get());
					// l-m
					m_imgTmp[3].Set
						(
						CImgDeformer::ZomDeform
							(
							image[3].Get(), 
							image[3].GetWidth(), 
							r_h - image[0].GetHeight() - image[6].GetHeight()
							)
						);
					// m-m
					m_imgTmp[4].Set
						(
						CImgDeformer::ZomDeform
							(
							image[4].Get(), 
							clt_rct.Width() - image[3].GetWidth() - image[5].GetWidth(), 
							r_h - image[1].GetHeight() - image[7].GetHeight()
							)
						);
					// r-m
					m_imgTmp[5].Set
						(
						CImgDeformer::ZomDeform
							(
							image[5].Get(), 
							image[5].GetWidth(), 
							r_h - image[2].GetHeight() - image[8].GetHeight()
							)
						);
					// l-b
					m_imgTmp[6].Set(image[6].Get());
					// m-b
					m_imgTmp[7].Set
						(
						CImgDeformer::ZomDeform
							(
							image[7].Get(), 
							clt_rct.Width() - image[6].GetWidth() - image[8].GetWidth(), 
							image[7].GetHeight()
							)
						);
					// r-b
					m_imgTmp[8].Set(image[8].Get());
					// Save
					m_rcOld = clt_rct;
				}
				LONG radius = 0;
				if (glow)
				{
					CFilterGauss filter;
					radius = filter.m_Radius;
					if (m_IconOld.Get() != icon->Get())
					{
						if (icon->IsNull())
							m_IconTmp.Delete();
						else
						{
							CPoint pt_flt(radius << 1, radius << 1);
							// 将图片扩大
							CRect rc(0, 0, icon->GetWidth(), icon->GetHeight());
							m_IconTmp = icon->Clone(rc + pt_flt);
							// 阴影化
							CImgFilter::Filter(m_IconTmp, CRect(), &CFilterFill(0, 0xe));
							CImgFilter::Filter(m_IconTmp, CRect(), &filter);
							// 阴影叠加
							rc.Offset(pt_flt);
							CImgRenderer::Render(m_IconTmp, icon->Get(), rc, CPoint(), &CRenderOverlay());
						}
						// 保存指针
						m_IconOld = icon->Get();
					}
					icon = &m_IconTmp;
				}

				// 绘图
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

				// 绘文字
				DWORD locate = (DWORD)ctrl->GetState(_T("locate"));
				LONG loc_off = (LONG)ctrl->GetState(_T("loc_off"));
				CRect txt_rct, img_rct(rect);
				CSize txt_clp;
				text->GetSize(txt_clp);
				if (txt_clp.cx != 0 && txt_clp.cy != 0)
				{
					GetTxtRect(rect, txt_clp, locate, loc_off, txt_rct);
					img_rct.pt1.y = txt_rct.Top();
					img_rct.pt2.y = txt_rct.Bottom();
					if (m_txtTmp != (*text) || 
						m_LocOld != locate || 
						m_OffOld != loc_off)
					{
						m_txtTmp = (*text);
						m_LocOld = locate;
						m_OffOld = loc_off;
						CText clp_txt;
						clp_txt = m_txtTmp;
						CString clp_str(m_txtTmp.GetCStr());
						m_imgClp.Create(img_rct.Width(), img_rct.Height());
						LONG txt_off = 0;
						while(txt_off + txt_clp.cy <= img_rct.Height() - 4)
						{
							// 计算剪切文本
							clp_txt.GetSize(txt_clp);
							if (txt_clp.cx == 0 || txt_clp.cy == 0) break;
							if (txt_clp.cx > img_rct.Width() - 4)
							{
								// 计算下一行是否显示高度不够
								if (txt_off + ((txt_clp.cy + 2) << 1) <= img_rct.Height() - 2)
								{
									do
									{
										clp_txt.LastItem() = 0;
										clp_txt.GetSize(txt_clp);
									} while (!clp_txt.Empty() && txt_clp.cx > img_rct.Width() - 4);
								}
								else
								{
									CString tmp(clp_txt.GetCStr());
									do
									{
										tmp.LastItem() = 0;
										clp_txt.SetString(tmp);
										clp_txt += _T("...");
										clp_txt.GetSize(txt_clp);
									} while (!tmp.Empty() && txt_clp.cx > img_rct.Width() - 4);
								}
							}
							// 覆盖剪切文本
							CImage img_tmp(clp_txt.GetImage());
							CImgRenderer::Render(m_imgClp, img_tmp, 
								CRect
									(
									(img_rct.Width() - img_tmp.GetWidth()) >> 1, 
									txt_off, 
									(img_rct.Width() + img_tmp.GetWidth()) >> 1, 
									txt_off + txt_clp.cy
									), 
								CPoint(), &CRenderCopy());
							// 折行
							clp_str = ((LPCTSTR)clp_str) + clp_txt.GetLength();
							clp_txt.SetString(clp_str);
							txt_off += (txt_clp.cy + 2);
						}
					}
					CImgRenderer::Render(mem_img->Get(), m_imgClp, img_rct);
				}

				// 绘图标
				if (icon->IsNull()) break;
				loc_off = (LONG)ctrl->GetState(_T("ico_off")) - radius;
				CRect icon_rct;
				switch(locate)
				{
				case 0:	// center
					icon_rct.Set
						(
						CPoint
							(
							rect.Left() + (rect.Width() - icon->GetWidth()) / 2, 
							rect.Top() + (rect.Height() - icon->GetHeight()) / 2
							), 
						CPoint(rect.Right(), rect.Bottom())
						);
					break;
				case 1:	// top
					icon_rct.Set
						(
						CPoint
							(
							rect.Left() + (rect.Width() - icon->GetWidth()) / 2, 
							rect.Bottom() - icon->GetHeight() - loc_off
							), 
						CPoint(rect.Right(), rect.Bottom())
						);
					break;
				case 2:	// bottom
					icon_rct.Set
						(
						CPoint
							(
							rect.Left() + (rect.Width() - icon->GetWidth()) / 2, 
							rect.Top() + loc_off
							), 
						CPoint(rect.Right(), rect.Bottom())
						);
					break;
				case 3:	// left
					icon_rct.Set
						(
						CPoint
							(
							rect.Right() - icon->GetWidth() - loc_off, 
							rect.Top() + (rect.Height() - icon->GetHeight()) / 2
							), 
						CPoint(rect.Right(), rect.Bottom())
						);
					break;
				case 4:	// right
					icon_rct.Set
						(
						CPoint
							(
							rect.Left() + loc_off, 
							rect.Top() + (rect.Height() - icon->GetHeight()) / 2
							), 
						CPoint(rect.Right(), rect.Bottom())
						);
					break;
				}
				CImgRenderer::Render(mem_img->Get(), icon->Get(), icon_rct, CPoint());
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
	CText m_txtTmp;
	CImage m_imgClp;
	DWORD m_LocOld;
	LONG m_OffOld;

public:
	CGuiPushBtnEvent()
		: m_LocOld(0)
		, m_OffOld(0)
	{}
	~CGuiPushBtnEvent()
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
		case WM_NCMOUSEMOVE:
			{
				DWORD status = (DWORD)ctrl->GetState(_T("status"));
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
				DWORD status = (DWORD)ctrl->GetState(_T("status"));
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
				DWORD status = (DWORD)ctrl->GetState(_T("status"));
				if (status == 3) break;

				IGuiBoard* board = ctrl->GetBoard();
				if (!board) break;
				POINT pt_tmp = {0};
				::GetCursorPos(&pt_tmp);
				CPoint pt(pt_tmp);
				board->ScreenToClient(pt);
				CRect rc;
				ctrl->GetRealRect(rc);

				if (status == 2) // 当按下后抬起,视为一次Click
				{
					ctrl->Send(ExDynCast<IGuiObject>(ctrl), BM_CLICK);
					IGuiCtrl* pare = ExDynCast<IGuiCtrl>(ctrl->GetParent());
					if (pare)
						pare->Send(ExDynCast<IGuiObject>(pare), WM_COMMAND, BN_CLICKED, (LPARAM)ctrl);
				}

				status = (DWORD)ctrl->GetState(_T("status"));
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
				DWORD status = (DWORD)ctrl->GetState(_T("status"));
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
				// 获得属性
				LONG sta_tim = 4;

				DWORD status = (DWORD)ctrl->GetState(_T("status"));

				CImage* image = (CImage*)ctrl->GetState(_T("image"));
				pixel_t pixel = ((pixel_t*)ctrl->GetState(_T("color")))[status];
				CText* text = (CText*)ctrl->GetState(_T("text")) + status;

				CImage* mem_img = (CImage*)lParam;
				if (!mem_img || mem_img->IsNull()) break;
				CRect rect, clt_rct;
				ctrl->GetClipRect(rect);
				ctrl->GetClientRect(clt_rct);

				// 处理
				if (m_rcOld != clt_rct)
				{
					LONG r_h = clt_rct.Height() * sta_tim;
					// m-m
					m_imgTmp.Set(CImgDeformer::ZomDeform(image->Get(), clt_rct.Width(), r_h));
					// Save
					m_rcOld = clt_rct;
				}

				// 绘图
				CImgDrawer::Fill(mem_img->Get(), rect, pixel);
				// m-m
				CImgRenderer::Render
					(
					mem_img->Get(), m_imgTmp, rect, 
					CPoint(0, m_imgTmp.GetHeight() * status / sta_tim)
					);

				// 绘文字
				DWORD locate = (DWORD)ctrl->GetState(_T("locate"));
				LONG loc_off = (LONG)ctrl->GetState(_T("loc_off"));
				CRect txt_rct, img_rct(rect);
				CSize txt_clp;
				text->GetSize(txt_clp);
				if (txt_clp.cx == 0 || txt_clp.cy == 0) break;
				GetTxtRect(rect, txt_clp, locate, loc_off, txt_rct);
				img_rct.pt1.y = txt_rct.Top();
				img_rct.pt2.y = txt_rct.Bottom();
				if (m_txtTmp != (*text) || 
					m_LocOld != locate || 
					m_OffOld != loc_off)
				{
					m_txtTmp = (*text);
					m_LocOld = locate;
					m_OffOld = loc_off;
					CText clp_txt;
					clp_txt = m_txtTmp;
					CString clp_str(m_txtTmp.GetCStr());
					m_imgClp.Create(img_rct.Width(), img_rct.Height());
					LONG txt_off = 0;
					while(txt_off + txt_clp.cy <= img_rct.Height() - 4)
					{
						// 计算剪切文本
						clp_txt.GetSize(txt_clp);
						if (txt_clp.cx == 0 || txt_clp.cy == 0) break;
						if (txt_clp.cx > img_rct.Width() - 4)
						{
							// 计算下一行是否显示高度不够
							if (txt_off + ((txt_clp.cy + 2) << 1) <= img_rct.Height() - 2)
							{
								do
								{
									clp_txt.LastItem() = 0;
									clp_txt.GetSize(txt_clp);
								} while (!clp_txt.Empty() && txt_clp.cx > img_rct.Width() - 4);
							}
							else
							{
								CString tmp(clp_txt.GetCStr());
								do
								{
									tmp.LastItem() = 0;
									clp_txt.SetString(tmp);
									clp_txt += _T("...");
									clp_txt.GetSize(txt_clp);
								} while (!tmp.Empty() && txt_clp.cx > img_rct.Width() - 4);
							}
						}
						// 覆盖剪切文本
						CImage img_tmp(clp_txt.GetImage());
						CImgRenderer::Render(m_imgClp, img_tmp, 
							CRect
								(
								(img_rct.Width() - img_tmp.GetWidth()) >> 1, 
								txt_off, 
								(img_rct.Width() + img_tmp.GetWidth()) >> 1, 
								txt_off + txt_clp.cy
								), 
							CPoint(), &CRenderCopy());
						// 折行
						clp_str = ((LPCTSTR)clp_str) + clp_txt.GetLength();
						clp_txt.SetString(clp_str);
						txt_off += (txt_clp.cy + 2);
					}
				}
				CImgRenderer::Render(mem_img->Get(), m_imgClp, img_rct);
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