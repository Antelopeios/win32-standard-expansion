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
// GuiButtonEvent - 按钮控件事件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-02-28
// Version:	1.0.0017.1653
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
//	- 1.0.0013.1048(2011-09-01)	# 修正当按钮比文字预留区域小时将不会显示文字的问题
//	- 1.0.0014.1540(2011-09-22)	= 按钮状态最多支持8个变化
//	- 1.0.0015.1234(2011-09-23)	+ 添加shake_ico属性,支持点击时晃动图标及文字
//	- 1.0.0016.1710(2011-10-21)	+ 添加关闭缓存时的绘图逻辑
//	- 1.0.0017.1653(2012-02-28)	^ 优化并简化按钮相关事件逻辑的实现
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
	CText m_txtTmp;
	CImage m_imgClp;
	DWORD m_LocOld;
	LONG m_OffOld;
	CImage m_IconOld, m_IconTmp;
	
protected:
	void SendPare(IGuiCtl* pCtl, UINT nMsg)
	{
		IGuiCtl* pare = ExDynCast<IGuiCtl>(pCtl->GetParent());
		if (pare) pare->SendMessage(WM_COMMAND, nMsg, (LPARAM)pCtl);
	}
	
	void GetTxtRect(const CRect& rect, const CSize& txt_clp, DWORD locate, LONG loc_off, CRect& txt_rct)
	{
		switch(locate)
		{
		case 0:	// center
			txt_rct.Set(
				CPoint(
					rect.Left() + (rect.Width() - txt_clp.cx) / 2, 
					rect.Top() + (rect.Height() - txt_clp.cy) / 2), 
				CPoint(rect.Right(), rect.Bottom()));
			break;
		case 1:	// top
			txt_rct.Set(
				CPoint(
					rect.Left() + (rect.Width() - txt_clp.cx) / 2, 
					rect.Top() + loc_off), 
				CPoint(rect.Right(), rect.Bottom()));
			break;
		case 2:	// bottom
			txt_rct.Set(
				CPoint(
					rect.Left() + (rect.Width() - txt_clp.cx) / 2, 
					rect.Bottom() - txt_clp.cy - loc_off), 
				CPoint(rect.Right(), rect.Bottom()));
			break;
		case 3:	// left
			txt_rct.Set(
				CPoint(
					rect.Left() + loc_off, 
					rect.Top() + (rect.Height() - txt_clp.cy) / 2), 
				CPoint(rect.Right(), rect.Bottom()));
			break;
		case 4:	// right
			txt_rct.Set(
				CPoint(
					rect.Right() - txt_clp.cx - loc_off, 
					rect.Top() + (rect.Height() - txt_clp.cy) / 2), 
				CPoint(rect.Right(), rect.Bottom()));
			break;
		}
	}

	BOOL FmtTxtRect(const CRect& rect, CText* text, CString* str, DWORD locate, LONG loc_off, CRect& img_rct)
	{
		if (!text || !str) return FALSE;

		CSize txt_clp;
		text->GetSize(*str, txt_clp);
		if (txt_clp.cx == 0 || txt_clp.cy == 0) return FALSE;

	//	ExTrace(_T("0x%08x\n"), this);
		GetTxtRect(rect, txt_clp, locate, loc_off, img_rct);
		if (m_txtTmp == (*text) && m_LocOld == locate && m_OffOld == loc_off) return TRUE;
		m_txtTmp = (*text);
		m_LocOld = locate;
		m_OffOld = loc_off;

		// 绘制缓存
		CString clp_str(*str), clp_lne(*str);
		m_imgClp.Create(img_rct.Width(), img_rct.Height());
		LONG txt_off = 0;
		do
		{
			// 计算剪切文本
			text->GetSize(clp_str, txt_clp);
			if (txt_clp.cx == 0 || txt_clp.cy == 0) break;
			if (txt_clp.cx > img_rct.Width() - 4)
			{
				// 计算下一行是否显示高度不够
				if (txt_off + ((txt_clp.cy + 2) << 1) <= img_rct.Height() - 2)
				{
					do
					{
						clp_str.LastItem() = 0;
						text->GetSize(clp_str, txt_clp);
					} while (!clp_str.Empty() && txt_clp.cx > img_rct.Width() - 4);
				}
				else
				{
					CString tmp(clp_str);
					do
					{
						tmp.LastItem() = 0;
						clp_str = tmp;
						clp_str += _T("...");
						text->GetSize(clp_str, txt_clp);
					} while (!tmp.Empty() && txt_clp.cx > img_rct.Width() - 4);
				}
			}
			// 覆盖剪切文本
			CImage img_tmp(text->GetImage(clp_str));
			CImgDrawer::Cover(m_imgClp, img_tmp, CRect(0, txt_off, img_rct.Width(), txt_off + txt_clp.cy));
			// 折行
			clp_lne = ((LPCTSTR)clp_lne) + clp_str.GetLength();
			clp_str = clp_lne;
			txt_off += (txt_clp.cy + 2);
		} while(txt_off + txt_clp.cy <= img_rct.Height() - 4);

		return TRUE;
	}

	virtual void DblClk(IGuiCtl* pCtl)
	{
		SendPare(pCtl, BN_DOUBLECLICKED);
	}

	virtual void BtnDwn(IGuiCtl* pCtl)
	{
		pCtl->SetState(_T("status"), (void*)2);
	}

	virtual void BtnUp(IGuiCtl* pCtl)
	{
		IGuiWnd* wnd = pCtl->GetWnd();
		if (!wnd) return;
		POINT pt_tmp = {0};
		::GetCursorPos(&pt_tmp);
		CPoint pt(pt_tmp);
		wnd->ScreenToClient(pt);
		CRect rc;
		pCtl->GetRealRect(rc);

		DWORD status = (DWORD)pCtl->GetState(_T("status"));
		if (status == 2) // 当按下后抬起,视为一次Click
		{
			pCtl->SendMessage(BM_CLICK);
			if(!pCtl->IsValid()) return;
			SendPare(pCtl, BN_CLICKED);
		}

		if (rc.PtInRect(pt))
			status = 1;
		else
			status = 0;
		pCtl->SetState(_T("status"), (void*)status);
	}

	virtual void MusMov(IGuiCtl* pCtl)
	{
		if ((DWORD)pCtl->GetState(_T("status")) != 2)
			pCtl->SetState(_T("status"), (void*)1);
	}

	virtual void MusLev(IGuiCtl* pCtl)
	{
		pCtl->SetState(_T("status"), (void*)0);
	}

	virtual void Paint(IGuiCtl* pCtl, CImage* mem_img)
	{
		if (!mem_img || mem_img->IsNull()) return;

		// 获得属性
		int thr_sta = (int)pCtl->GetState(_T("thr_sta"));
		LONG sta_tim = (thr_sta == 1 ? 3 : (thr_sta == -1 ? 1 : 8));

		DWORD status = (DWORD)pCtl->GetState(_T("status"));
		if (thr_sta == 0)
		{	// 正常情况
			if(!pCtl->IsEnabled()) status = 3;
			if (pCtl->IsFocus()) status += 4;
		}
		else
		if (thr_sta == 1)
		{	// 无焦点状态
		//	if (!ctl->IsEnabled()) status = 3;
		}
		else
		if (thr_sta == -1)
		{	// 单态按钮
			status = 0;
		}

		int shake_ico = (int)pCtl->GetState(_T("shake_ico"));

		CImage** image = (CImage**)pCtl->GetState(_T("image"));
		pixel_t pixel = ((pixel_t*)pCtl->GetState(_T("color")))[status];
		CText* text = ((CText**)pCtl->GetState(_T("font")))[status];
		CString* str = (CString*)pCtl->GetState(_T("text"));
		CImage* icon = (CImage*)pCtl->GetState(_T("icon"));
		BOOL glow = (BOOL)(LONG_PTR)pCtl->GetState(_T("glow"));

		CRect rect, clt_rct;
		pCtl->GetClipRect(rect);
		pCtl->GetClientRect(clt_rct);

		CSize sz_res[9];
		for(int i = 0; i < _countof(sz_res); ++i)
		{
			if(!image[i]) continue;
			sz_res[i].Set(image[i]->GetWidth(), image[i]->GetHeight());
		}

		// 处理
		LONG r_h = clt_rct.Height() * sta_tim;
		CRect rc_mem[9];
		CSize sz_img[9];
		// l-t
		rc_mem[0].Set(
			rect.Left(), rect.Top(), 
			rect.Left() + sz_res[0].cx, 
			rect.Top() + sz_res[0].cy / sta_tim);
		sz_img[0].Set(
			sz_res[0].cx, 
			sz_res[0].cy);
		// m-t
		rc_mem[1].Set(
			rect.Left() + sz_res[0].cx, 
			rect.Top(), 
			rect.Right() - sz_res[2].cx, 
			rect.Top() + sz_res[1].cy / sta_tim);
		sz_img[1].Set(
			clt_rct.Width() - sz_res[0].cx - sz_res[2].cx, 
			sz_res[1].cy);
		// r-t
		rc_mem[2].Set(
			rect.Right() - sz_res[2].cx, 
			rect.Top(), 
			rect.Right(), 
			rect.Top() + sz_res[2].cy / sta_tim);
		sz_img[2].Set(
			sz_res[2].cx, 
			sz_res[2].cy);
		// l-m
		rc_mem[3].Set(
			rect.Left(), 
			rect.Top() + sz_res[0].cy / sta_tim, 
			rect.Left() + sz_res[3].cx, 
			rect.Bottom() - sz_res[6].cy / sta_tim);
		sz_img[3].Set(
			sz_res[3].cx, 
			r_h - sz_res[0].cy - sz_res[6].cy);
		// m-m
		rc_mem[4].Set(
			rect.Left() + sz_res[3].cx, 
			rect.Top() + sz_res[1].cy / sta_tim, 
			rect.Right() - sz_res[5].cx, 
			rect.Bottom() - sz_res[7].cy / sta_tim);
		sz_img[4].Set(
			clt_rct.Width() - sz_res[3].cx - sz_res[5].cx, 
			r_h - sz_res[1].cy - sz_res[7].cy);
		// r-m
		rc_mem[5].Set(
			rect.Right() - sz_res[5].cx, 
			rect.Top() + sz_res[2].cy / sta_tim, 
			rect.Right(), 
			rect.Bottom() - sz_res[8].cy / sta_tim);
		sz_img[5].Set(
			sz_res[5].cx, 
			r_h - sz_res[2].cy - sz_res[8].cy);
		// l-b
		rc_mem[6].Set(
			rect.Left(), 
			rect.Bottom() - sz_res[6].cy / sta_tim, 
			rect.Left() + sz_res[6].cx, 
			rect.Bottom());
		sz_img[6].Set(
			sz_res[8].cx, 
			sz_res[8].cy);
		// m-b
		rc_mem[7].Set(
			rect.Left() + sz_res[6].cx, 
			rect.Bottom() - sz_res[7].cy / sta_tim, 
			rect.Right() - sz_res[8].cx, 
			rect.Bottom());
		sz_img[7].Set(
			clt_rct.Width() - sz_res[6].cx - sz_res[8].cx, 
			sz_res[7].cy);
		// r-b
		rc_mem[8].Set(
			rect.Right() - sz_res[8].cx, 
			rect.Bottom() - sz_res[8].cy / sta_tim, 
			rect.Right(), 
			rect.Bottom());
		sz_img[8].Set(
			sz_res[8].cx, 
			sz_res[8].cy);
		// 图标阴影
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
					CImgDrawer::Draw(m_IconTmp, icon->Get(), rc);
				}
				// 保存指针
				m_IconOld = icon->Get();
			}
			icon = &m_IconTmp;
		}

		// 绘图
		CRect rc_tmp;
		CImgDrawer::Fill(mem_img->Get(), rect, pixel);
		for(int i = 0; i < _countof(rc_mem); ++i)
		{
			if(!image[i]) continue;
			CImgDrawer::Draw(mem_img->Get(), *(image[i]), rc_mem[i], 
				CPoint(0, image[i]->GetHeight() * status / sta_tim), sz_img[i]);
		}

		// 绘文字
		DWORD locate = (DWORD)pCtl->GetState(_T("locate"));
		LONG loc_off = (LONG)pCtl->GetState(_T("loc_off"));
		CRect img_rct;
		if (FmtTxtRect(rect, text, str, locate, loc_off, img_rct))
		{
			if (shake_ico != 0 && (status == 2 || status == 6)) img_rct.Offset(CPoint(1, 1));
			CImgDrawer::Draw(mem_img->Get(), m_imgClp, img_rct);
		}

		// 绘图标
		if (!icon || icon->IsNull()) return;
		loc_off = (LONG)pCtl->GetState(_T("ico_off")) - radius;
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
		if (shake_ico != 0 && (status == 2 || status == 6)) icon_rct.Offset(CPoint(1, 1));
		CImgDrawer::Draw(mem_img->Get(), icon->Get(), icon_rct);
	}

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
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		// 处理消息
		switch( nMessage )
		{
		case WM_LBUTTONDBLCLK:
		case WM_NCLBUTTONDBLCLK:
			DblClk(ctl);
			break;
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			MusMov(ctl);
			break;
		case WM_KEYDOWN:
			if (wParam != VK_RETURN) break; // Enter
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			BtnDwn(ctl);
			break;
		case WM_KEYUP:
			if (wParam != VK_RETURN) break; // Enter
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			BtnUp(ctl);
			break;
		case WM_MOUSELEAVE:
			MusLev(ctl);
			break;
		case WM_SETFOCUS:
			SendPare(ctl, BN_SETFOCUS);
			break;
		case WM_KILLFOCUS:
			SendPare(ctl, BN_KILLFOCUS);
			break;
		case WM_PAINT:
			Paint(ctl, (CImage*)lParam);
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CGuiPushBtnEvent : public CGuiButtonEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiPushBtnEvent, CGuiButtonEvent)

protected:
	void BtnDwn(IGuiCtl* pCtl)
	{
		if ((DWORD)pCtl->GetState(_T("status")) != 3)
			pCtl->SetState(_T("status"), (void*)2);
	}

	void BtnUp(IGuiCtl* pCtl)
	{
		DWORD status = (DWORD)pCtl->GetState(_T("status"));
		if (status == 3) return;

		IGuiWnd* wnd = pCtl->GetWnd();
		if (!wnd) return;
		POINT pt_tmp = {0};
		::GetCursorPos(&pt_tmp);
		CPoint pt(pt_tmp);
		wnd->ScreenToClient(pt);
		CRect rc;
		pCtl->GetRealRect(rc);

		if (status == 2) // 当按下后抬起,视为一次Click
		{
			pCtl->SendMessage(BM_CLICK);
			if(!pCtl->IsValid()) return;
			SendPare(pCtl, BN_CLICKED);
		}

		status = (DWORD)pCtl->GetState(_T("status"));
		if (status == 3) return;

		if (rc.PtInRect(pt))
			status = 1;
		else
			status = 0;
		pCtl->SetState(_T("status"), (void*)status);
	}

	void MusMov(IGuiCtl* pCtl)
	{
		DWORD status = (DWORD)pCtl->GetState(_T("status"));
		if (status != 2 && status != 3)
			pCtl->SetState(_T("status"), (void*)1);
	}

	void MusLev(IGuiCtl* pCtl)
	{
		if ((DWORD)pCtl->GetState(_T("status")) != 3)
			pCtl->SetState(_T("status"), (void*)0);
	}

	void Paint(IGuiCtl* pCtl, CImage* mem_img)
	{
		if (!mem_img || mem_img->IsNull()) return;

		// 获得属性
		LONG sta_tim = 4;

		DWORD status = (DWORD)pCtl->GetState(_T("status"));

		CImage* image = (CImage*)pCtl->GetState(_T("image"));
		pixel_t* pixel = ((pixel_t**)pCtl->GetState(_T("color")))[status];
		CText* text = ((CText**)pCtl->GetState(_T("font")))[status];
		CString* str = (CString*)pCtl->GetState(_T("text"));

		CRect rect, clt_rct;
		pCtl->GetClipRect(rect);
		pCtl->GetClientRect(clt_rct);

		// 处理
		LONG r_h = clt_rct.Height() * sta_tim;

		// 绘图
		if (pixel)
			CImgDrawer::Fill(mem_img->Get(), rect, *pixel);
		// m-m
		if (image)
			CImgDrawer::Draw(mem_img->Get(), image->Get(), rect, CRect(0, 0, 
				rect.Width() >= clt_rct.Width() ? 
				image->GetWidth() : image->GetWidth() * rect.Width() / clt_rct.Width(), 
				rect.Height() >= r_h ? 
				image->GetHeight() : image->GetHeight() * rect.Height() / r_h).MoveTo(
				CPoint(0, image->GetHeight() * status / sta_tim)));
		
		// 绘文字
		DWORD locate = (DWORD)pCtl->GetState(_T("locate"));
		LONG loc_off = (LONG)pCtl->GetState(_T("loc_off"));
		CRect img_rct;
		if (FmtTxtRect(rect, text, str, locate, loc_off, img_rct))
			CImgDrawer::Draw(mem_img->Get(), m_imgClp, img_rct);
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiButtonEvent, IGuiEvent)
EXP_IMPLEMENT_DYNCREATE_CLS(CGuiPushBtnEvent, CGuiButtonEvent)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiButtonEvent_hpp__*/