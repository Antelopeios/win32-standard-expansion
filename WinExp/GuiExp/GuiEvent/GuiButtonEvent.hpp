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
// GuiButtonEvent - ��ť�ؼ��¼�
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-02-28
// Version:	1.0.0017.1653
//
// History:
//	- 1.0.0000.2258(2011-05-25)	@ ��ʼ����CGuiButtonEvent
//								@ �������CGuiButtonEvent�Ļ�ͼ����
//	- 1.0.0001.2302(2011-05-26)	# ����CGuiButtonEvent��ͼ�еĴ���
//								+ CGuiButtonEvent���״̬�仯��Ϣ����
//								# ����갴�º��ƶ�,��ť״̬���Ϊδ���µ�״̬
//								+ ��Ӱ�ť������Ϣת����Enter����Ӧ
//	- 1.0.0002.1020(2011-05-27)	# ����CGuiButtonEvent��WM_KEYUP֮���޷���Ӧ�����л���ͼ
//	- 1.0.0003.1426(2011-06-08)	# ����CGuiButtonEvent����Ӧ�ǹ���������¼�������
//	- 1.0.0004.1708(2011-06-21)	# ��������ť�ؼ�û�м���ͼƬʱCGuiButtonEvent��ͼ���ֵ��ڴ��쳣
//	- 1.0.0005.1430(2011-07-01)	= ����GuiButton�ĸ��µ���GuiButtonEvent
//	- 1.0.0006.1616(2011-08-03)	= ������ť�ؼ����¼�����,��������BM_CLICK��Ϣ,���򸸴��ڷ���BN_CLICKED��WM_COMMAND��Ϣ
//	- 1.0.0007.1708(2011-08-05)	+ GuiButtonEvent�򸸴��ڷ���BN_SETFOCUS��BN_KILLFOCUS��WM_COMMAND��Ϣ
//	- 1.0.0008.1755(2011-08-10)	^ GuiButtonEvent���Զ��Գ������������������ʡ�Դ���
//	- 1.0.0009.1808(2011-08-11)	^ �Ľ�GuiButtonEvent�����ֻ����㷨,�ڸ߶��㹻ʱ�Զ�������������ʾ
//	- 1.0.0010.1533(2011-08-12)	^ GuiButtonEvent�����������㷨֧���Զ���Ӧ����߶�(��һ�汾���ܶ���)
//								# ����һ��GuiButtonEvent״̬�жϵ��߼�����
//	- 1.0.0011.1606(2011-08-15)	+ ��GuiButtonEvent��ʵ��icon������ԵĻ�ͼ����
//	- 1.0.0012.1716(2011-08-30)	+ ���˫����ťʱ��BN_DOUBLECLICKED��Ϣ����
//	- 1.0.0013.1048(2011-09-01)	# ��������ť������Ԥ������Сʱ��������ʾ���ֵ�����
//	- 1.0.0014.1540(2011-09-22)	= ��ť״̬���֧��8���仯
//	- 1.0.0015.1234(2011-09-23)	+ ���shake_ico����,֧�ֵ��ʱ�ζ�ͼ�꼰����
//	- 1.0.0016.1710(2011-10-21)	+ ��ӹرջ���ʱ�Ļ�ͼ�߼�
//	- 1.0.0017.1653(2012-02-28)	^ �Ż����򻯰�ť����¼��߼���ʵ��
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

		// ���ƻ���
		CString clp_str(*str), clp_lne(*str);
		m_imgClp.Create(img_rct.Width(), img_rct.Height());
		LONG txt_off = 0;
		do
		{
			// ��������ı�
			text->GetSize(clp_str, txt_clp);
			if (txt_clp.cx == 0 || txt_clp.cy == 0) break;
			if (txt_clp.cx > img_rct.Width() - 4)
			{
				// ������һ���Ƿ���ʾ�߶Ȳ���
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
			// ���Ǽ����ı�
			CImage img_tmp(text->GetImage(clp_str));
			CImgDrawer::Cover(m_imgClp, img_tmp, CRect(0, txt_off, img_rct.Width(), txt_off + txt_clp.cy));
			// ����
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
		if (status == 2) // �����º�̧��,��Ϊһ��Click
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

		// �������
		int thr_sta = (int)pCtl->GetState(_T("thr_sta"));
		LONG sta_tim = (thr_sta == 1 ? 3 : (thr_sta == -1 ? 1 : 8));

		DWORD status = (DWORD)pCtl->GetState(_T("status"));
		if (thr_sta == 0)
		{	// �������
			if(!pCtl->IsEnabled()) status = 3;
			if (pCtl->IsFocus()) status += 4;
		}
		else
		if (thr_sta == 1)
		{	// �޽���״̬
		//	if (!ctl->IsEnabled()) status = 3;
		}
		else
		if (thr_sta == -1)
		{	// ��̬��ť
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

		// ����
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
		// ͼ����Ӱ
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
					// ��ͼƬ����
					CRect rc(0, 0, icon->GetWidth(), icon->GetHeight());
					m_IconTmp = icon->Clone(rc + pt_flt);
					// ��Ӱ��
					CImgFilter::Filter(m_IconTmp, CRect(), &CFilterFill(0, 0xe));
					CImgFilter::Filter(m_IconTmp, CRect(), &filter);
					// ��Ӱ����
					rc.Offset(pt_flt);
					CImgDrawer::Draw(m_IconTmp, icon->Get(), rc);
				}
				// ����ָ��
				m_IconOld = icon->Get();
			}
			icon = &m_IconTmp;
		}

		// ��ͼ
		CRect rc_tmp;
		CImgDrawer::Fill(mem_img->Get(), rect, pixel);
		for(int i = 0; i < _countof(rc_mem); ++i)
		{
			if(!image[i]) continue;
			CImgDrawer::Draw(mem_img->Get(), *(image[i]), rc_mem[i], 
				CPoint(0, image[i]->GetHeight() * status / sta_tim), sz_img[i]);
		}

		// ������
		DWORD locate = (DWORD)pCtl->GetState(_T("locate"));
		LONG loc_off = (LONG)pCtl->GetState(_T("loc_off"));
		CRect img_rct;
		if (FmtTxtRect(rect, text, str, locate, loc_off, img_rct))
		{
			if (shake_ico != 0 && (status == 2 || status == 6)) img_rct.Offset(CPoint(1, 1));
			CImgDrawer::Draw(mem_img->Get(), m_imgClp, img_rct);
		}

		// ��ͼ��
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

		// ������Ϣ
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

		if (status == 2) // �����º�̧��,��Ϊһ��Click
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

		// �������
		LONG sta_tim = 4;

		DWORD status = (DWORD)pCtl->GetState(_T("status"));

		CImage* image = (CImage*)pCtl->GetState(_T("image"));
		pixel_t* pixel = ((pixel_t**)pCtl->GetState(_T("color")))[status];
		CText* text = ((CText**)pCtl->GetState(_T("font")))[status];
		CString* str = (CString*)pCtl->GetState(_T("text"));

		CRect rect, clt_rct;
		pCtl->GetClipRect(rect);
		pCtl->GetClientRect(clt_rct);

		// ����
		LONG r_h = clt_rct.Height() * sta_tim;

		// ��ͼ
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
		
		// ������
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