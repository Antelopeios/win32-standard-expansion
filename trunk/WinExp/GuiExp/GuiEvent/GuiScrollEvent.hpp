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
// GuiScrollEvent - 滚动条控件事件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-12
// Version:	1.0.0002.1447
//
// History:
//	- 1.0.0000.1337(2011-08-02)	@ 准备构建GuiScrollEvent
//	- 1.0.0001.1816(2011-08-24)	+ 当GuiScroll收到重定位消息时,自动通知其关联的控件进行滚动
//	- 1.0.0002.1447(2012-03-12)	# 修正滚动条无限循环绘图导致CPU居高不下的bug
//////////////////////////////////////////////////////////////////

#ifndef __GuiScrollEvent_hpp__
#define __GuiScrollEvent_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiSliBlkEvent : public IGuiEvent /*滚动条滑块*/
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiSliBlkEvent, IGuiEvent)

protected:
	BOOL m_MDown;
	CPoint m_MPoint;

public:
	CGuiSliBlkEvent()
		: m_MDown(FALSE)
	{}

public:
	// 消息响应
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		// 处理消息
		switch( nMessage )
		{
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			if (m_MDown)
			{
				CPoint pt;
				::GetCursorPos((LPPOINT)&pt);
				pt -= m_MPoint;
				IGuiCtl* pare = ExDynCast<IGuiCtl>(ctl->GetParent());
				pare->SendMessage(WM_COMMAND, SB_THUMBPOSITION, (LPARAM)&pt);
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			ctl->SetCapture();
			m_MDown = TRUE;
			::GetCursorPos((LPPOINT)&m_MPoint);
			{
				IGuiCtl* pare = ExDynCast<IGuiCtl>(ctl->GetParent());
				pare->SendMessage(WM_COMMAND, SB_THUMBTRACK);
			}
			break;
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			{
				IGuiCtl* pare = ExDynCast<IGuiCtl>(ctl->GetParent());
				pare->SendMessage(WM_COMMAND, SB_ENDSCROLL);
			}
			m_MDown = FALSE;
			ctl->ReleaseCapture();
			break;
		case WM_MOUSEWHEEL:
			{
				IGuiCtl* pare = ExDynCast<IGuiCtl>(ctl->GetParent());
				pare->SendMessage(nMessage, wParam, lParam);
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CGuiSliderEvent : public IGuiEvent /*滚动条滑槽*/
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiSliderEvent, IGuiEvent)

protected:
	IGuiCtl* m_Ctrl;

public:
	CGuiSliderEvent()
		: m_Ctrl(NULL)
	{}

public:
	// 获得属性
	IGuiCtl* GetSlider()
	{
		return (IGuiCtl*)m_Ctrl->GetState(_T("slider"));
	}
	BOOL GetOri()
	{
		return (BOOL)(LONG_PTR)m_Ctrl->GetState(_T("ori"));
	}
	LONG GetAll()
	{
		return (LONG)(LONG_PTR)m_Ctrl->GetState(_T("all"));
	}
	LONG GetFra()
	{
		return (LONG)(LONG_PTR)m_Ctrl->GetState(_T("fra"));
	}
	LONG GetPos()
	{
		return (LONG)(LONG_PTR)m_Ctrl->GetState(_T("pos"));
	}

	// 格式化位置
	void Format()
	{
		ExAssert(m_Ctrl);
		IGuiCtl* sli = GetSlider();
		if (!sli) return;

		BOOL ori = GetOri();
		LONG all = GetAll();
		LONG fra = GetFra();
		LONG pos = GetPos();

		if (all <= 0 || fra <= 0) return;
		if (fra > all) fra = all;

		CRect rect;
		m_Ctrl->GetClientRect(rect);
		if (ori)
		{
			LONG sli_pos = (LONG)((double)rect.Height() * (double)pos / (double)all + 0.5);
			LONG sli_fra = sli_pos + (LONG)((double)rect.Height() * (double)fra / (double)all + 0.5);
			sli->SetWindowRect(CRect(0, sli_pos, rect.Right(), sli_fra));
		}
		else
		{
			LONG sli_pos = (LONG)((double)rect.Width() * (double)pos / (double)all + 0.5);
			LONG sli_fra = sli_pos + (LONG)((double)rect.Width() * (double)fra / (double)all + 0.5);
			sli->SetWindowRect(CRect(sli_pos, 0, sli_fra, rect.Bottom()));
		}
	}

	// 消息响应
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		m_Ctrl = ExDynCast<IGuiCtl>(pGui);
		if (!m_Ctrl) return;

		static LONG s_pos = 0;

		// 处理消息
		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
			Format();
			break;
		case WM_COMMAND:
			if (wParam == SB_THUMBPOSITION)
			{
				CPoint* pt = (CPoint*)lParam;
				if (!pt) break;
				CRect rect;
				m_Ctrl->GetClientRect(rect);
				BOOL ori = GetOri();
				if (ori)
				{
					if (pt->y == 0) break;
					LONG all = GetAll();
					LONG off = pt->y * all / rect.Height();
					m_Ctrl->SetState(_T("pos"), (void*)(s_pos + off));
				}
				else
				{
					if (pt->x == 0) break;
					LONG all = GetAll();
					LONG off = pt->x * all / rect.Width();
					m_Ctrl->SetState(_T("pos"), (void*)(s_pos + off));
				}
				IGuiCtl* pare = ExDynCast<IGuiCtl>(m_Ctrl->GetParent());
				pare->SendMessage(WM_COMMAND, wParam, lParam);
			}
			else
			if (wParam == SB_THUMBTRACK)
			{
				s_pos = GetPos();
				IGuiCtl* pare = ExDynCast<IGuiCtl>(m_Ctrl->GetParent());
				pare->SendMessage(WM_COMMAND, wParam, lParam);
			}
			else
			if (wParam == SB_ENDSCROLL)
			{
				IGuiCtl* pare = ExDynCast<IGuiCtl>(m_Ctrl->GetParent());
				pare->SendMessage(WM_COMMAND, wParam, lParam);
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			{
				IGuiCtl* sli = GetSlider();
				if (!sli) break;
				CRect rect, rc_pt;
				sli->GetWindowRect(rect);
				::GetCursorPos((LPPOINT)&(rc_pt.pt1));
				m_Ctrl->GetWnd()->ScreenToClient(rc_pt.pt1);
				m_Ctrl->B2C(rc_pt);
				rc_pt.pt1 -= CPoint((rect.Left() + rect.Right()) >> 1, (rect.Top() + rect.Bottom()) >> 1);
				m_Ctrl->GetClientRect(rect);
				LONG all = GetAll();
				BOOL ori = GetOri();
				if (ori)
				{
					LONG off = rc_pt.pt1.y * all / rect.Height();
					m_Ctrl->SetState(_T("pos"), (void*)(GetPos() + off));
				}
				else
				{
					LONG off = rc_pt.pt1.x * all / rect.Width();
					m_Ctrl->SetState(_T("pos"), (void*)(GetPos() + off));
				}
				IGuiCtl* pare = ExDynCast<IGuiCtl>(m_Ctrl->GetParent());
				pare->SendMessage(WM_COMMAND, SB_THUMBPOSITION);
			}
			break;
		case WM_MOUSEWHEEL:
			{
				IGuiCtl* pare = ExDynCast<IGuiCtl>(m_Ctrl->GetParent());
				pare->SendMessage(nMessage, wParam, lParam);
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CGuiScrUpEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiScrUpEvent, IGuiEvent)

protected:
	UINT m_TimerId;

public:
	CGuiScrUpEvent()
		: m_TimerId(0)
	{}

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		// 处理消息
		switch( nMessage )
		{
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			{
				ctl->SetCapture();
				ExRandomize();
				m_TimerId = 10001 + ExRandom(89999);
				IGuiWnd* wnd = ExDynCast<IGuiWnd>(ctl->GetWnd());
				::SetTimer(wnd->GethWnd(), m_TimerId, 40, NULL);
			}
			break;
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			if (m_TimerId)
			{
				IGuiWnd* wnd = ExDynCast<IGuiWnd>(ctl->GetWnd());
				::KillTimer(wnd->GethWnd(), m_TimerId);
				m_TimerId = 0;
				ctl->ReleaseCapture();
			}
			break;
		case WM_TIMER:
			if (m_TimerId == wParam)
			{
				IGuiCtl* pare = ExDynCast<IGuiCtl>(ctl->GetParent());
				LONG pos = (LONG)(LONG_PTR)pare->GetState(_T("sli_pos"));
				pare->SetState(_T("sli_pos"), (void*)(pos - WHEEL_DELTA));
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CGuiScrDnEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiScrDnEvent, IGuiEvent)

protected:
	UINT m_TimerId;

public:
	CGuiScrDnEvent()
		: m_TimerId(0)
	{}

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		// 处理消息
		switch( nMessage )
		{
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			{
				ctl->SetCapture();
				ExRandomize();
				m_TimerId = 10001 + ExRandom(89999);
				IGuiWnd* wnd = ExDynCast<IGuiWnd>(ctl->GetWnd());
				::SetTimer(wnd->GethWnd(), m_TimerId, 40, NULL);
			}
			break;
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			if (m_TimerId)
			{
				IGuiWnd* wnd = ExDynCast<IGuiWnd>(ctl->GetWnd());
				::KillTimer(wnd->GethWnd(), m_TimerId);
				m_TimerId = 0;
				ctl->ReleaseCapture();
			}
			break;
		case WM_TIMER:
			if (m_TimerId == wParam)
			{
				IGuiCtl* pare = ExDynCast<IGuiCtl>(ctl->GetParent());
				LONG pos = (LONG)(LONG_PTR)pare->GetState(_T("sli_pos"));
				pare->SetState(_T("sli_pos"), (void*)(pos + WHEEL_DELTA));
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CGuiScrollEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiScrollEvent, IGuiEvent)

protected:
	IGuiCtl* m_Ctrl;

public:
	CGuiScrollEvent()
		: m_Ctrl(NULL)
	{}

public:
	// 获得属性
	IGuiCtl* GetMain()
	{
		return (IGuiCtl*)m_Ctrl->GetState(_T("main"));
	}
	IGuiCtl* GetSlider()
	{
		return (IGuiCtl*)m_Ctrl->GetState(_T("slider"));
	}
	IGuiCtl* GetUp()
	{
		return (IGuiCtl*)m_Ctrl->GetState(_T("up"));
	}
	IGuiCtl* GetDn()
	{
		return (IGuiCtl*)m_Ctrl->GetState(_T("down"));
	}
	BOOL GetOri()
	{
		return (BOOL)(LONG_PTR)m_Ctrl->GetState(_T("sli_ori"));
	}
	LONG GetPos()
	{
		return (LONG)(LONG_PTR)m_Ctrl->GetState(_T("sli_pos"));
	}

	// 格式化位置
	void Format()
	{
		ExAssert(m_Ctrl);
		IGuiCtl* sli = GetSlider();
		if (!sli) return;
		IGuiCtl* up = GetUp();
		if (!sli) return;
		IGuiCtl* dn = GetDn();
		if (!sli) return;
		BOOL ori = GetOri();

		CRect rc_scr, rc_sli, rc_up, rc_dn;
		m_Ctrl->GetClientRect(rc_scr);
		sli->GetClientRect(rc_sli);
		up->GetClientRect(rc_up);
		dn->GetClientRect(rc_dn);
		if (ori)
		{
			rc_dn.pt2.x = rc_up.pt2.x = rc_sli.pt2.x = rc_scr.Right();
			rc_dn.MoveTo(CPoint(0, rc_scr.Bottom() - rc_dn.Height()));
			rc_sli.pt1.y = rc_up.pt2.y;
			rc_sli.pt2.y = rc_dn.pt1.y;
		}
		else
		{
			rc_dn.pt2.y = rc_up.pt2.y = rc_sli.pt2.y = rc_scr.Height();
			rc_dn.MoveTo(CPoint(rc_scr.Right() - rc_dn.Width(), 0));
			rc_sli.pt1.x = rc_up.pt2.x;
			rc_sli.pt2.x = rc_dn.pt1.x;
		}
		up->SetWindowRect(rc_up);
		sli->SetWindowRect(rc_sli);
		dn->SetWindowRect(rc_dn);
	}

	// 消息响应
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		m_Ctrl = ExDynCast<IGuiCtl>(pGui);
		if (!m_Ctrl) return;

		// 处理消息
		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
			Format();
			break;
		case WM_MOUSEWHEEL:
			{
				short scr_cnt = 0;
				BOOL ori = GetOri();
				if (ori)
					scr_cnt = (short)ExHiWord(wParam);
				else
					scr_cnt = (short)ExLoWord(wParam);
				m_Ctrl->SetState(_T("sli_pos"), (void*)(GetPos() - scr_cnt));
			}
			break;
		case WM_COMMAND:
			if (wParam == SB_THUMBPOSITION)
			{
				LONG pos = GetPos();
				CSize scr_sz;
				BOOL ori = GetOri();
				if (ori)
					scr_sz.cy = pos;
				else
					scr_sz.cx = pos;
				IGuiCtl* main = GetMain();
				if (main)
					main->SetScrollSize(scr_sz);
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiSliBlkEvent, IGuiEvent)
EXP_IMPLEMENT_DYNCREATE_CLS(CGuiSliderEvent, IGuiEvent)
EXP_IMPLEMENT_DYNCREATE_CLS(CGuiScrUpEvent, IGuiEvent)
EXP_IMPLEMENT_DYNCREATE_CLS(CGuiScrDnEvent, IGuiEvent)
EXP_IMPLEMENT_DYNCREATE_CLS(CGuiScrollEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiScrollEvent_hpp__*/