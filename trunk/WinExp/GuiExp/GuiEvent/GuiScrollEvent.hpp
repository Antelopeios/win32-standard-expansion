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
// GuiScrollEvent - 滚动条控件事件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-08-02
// Version:	1.0.0000.1337
//
// History:
//	- 1.0.0000.1337(2011-08-02)	@ 准备构建GuiScrollEvent
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
	bool m_MDown;
	CPoint m_MPoint;

public:
	CGuiSliBlkEvent()
		: m_MDown(false)
	{}

public:
	// 消息响应
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

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
				IGuiCtrl* pare = ExDynCast<IGuiCtrl>(ctrl->GetParent());
				pare->Send(ExDynCast<IGuiObject>(pare), WM_COMMAND, SB_THUMBPOSITION, (LPARAM)&pt);
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			ctrl->SetCapture();
			m_MDown = true;
			::GetCursorPos((LPPOINT)&m_MPoint);
			{
				IGuiCtrl* pare = ExDynCast<IGuiCtrl>(ctrl->GetParent());
				pare->Send(ExDynCast<IGuiObject>(pare), WM_COMMAND, SB_THUMBTRACK);
			}
			break;
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			{
				IGuiCtrl* pare = ExDynCast<IGuiCtrl>(ctrl->GetParent());
				pare->Send(ExDynCast<IGuiObject>(pare), WM_COMMAND, SB_ENDSCROLL);
			}
			m_MDown = false;
			ctrl->ReleaseCapture();
			break;
		case WM_MOUSEWHEEL:
			{
				IGuiCtrl* pare = ExDynCast<IGuiCtrl>(ctrl->GetParent());
				pare->Send(ExDynCast<IGuiObject>(pare), nMessage, wParam, lParam);
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
	IGuiCtrl* m_Ctrl;

public:
	CGuiSliderEvent()
		: m_Ctrl(NULL)
	{}

public:
	// 获得属性
	IGuiCtrl* GetSlider(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("slider"), pGC);
		if (!state) return NULL;
		return (IGuiCtrl*)(state->sta_arr[0]);
	}
	bool GetOri(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("ori"), pGC);
		if (!state) return NULL;
		return (bool)(LONG_PTR)(state->sta_arr[0]);
	}
	LONG GetAll(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("all"), pGC);
		if (!state) return NULL;
		return (LONG)(LONG_PTR)(state->sta_arr[0]);
	}
	LONG GetFra(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("fra"), pGC);
		if (!state) return NULL;
		return (LONG)(LONG_PTR)(state->sta_arr[0]);
	}
	LONG GetPos(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("pos"), pGC);
		if (!state) return NULL;
		return (LONG)(LONG_PTR)(state->sta_arr[0]);
	}

	// 格式化位置
	void Format(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl* sli = GetSlider(pGC);
		if (!sli) return;

		bool ori = GetOri(pGC);
		LONG all = GetAll(pGC);
		LONG fra = GetFra(pGC);
		LONG pos = GetPos(pGC);

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
		m_Ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!m_Ctrl) return;

		static LONG s_pos = 0;

		// 处理消息
		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
		case WM_PAINT:
			{
				CGC gc;
				Format(&gc);
			}
			break;
		case WM_COMMAND:
			if (wParam == SB_THUMBPOSITION)
			{
				CPoint* pt = (CPoint*)lParam;
				if (!pt) break;
				CRect rect;
				m_Ctrl->GetClientRect(rect);
				CGC gc;
				bool ori = GetOri(&gc);
				if (ori)
				{
					if (pt->y == 0) break;
					LONG all = GetAll(&gc);
					LONG off = pt->y * all / rect.Height();
					m_Ctrl->SetState(_T("pos"), (void*)(s_pos + off));
				}
				else
				{
					if (pt->x == 0) break;
					LONG all = GetAll(&gc);
					LONG off = pt->x * all / rect.Width();
					m_Ctrl->SetState(_T("pos"), (void*)(s_pos + off));
				}
				IGuiCtrl* pare = ExDynCast<IGuiCtrl>(m_Ctrl->GetParent());
				pare->Send(ExDynCast<IGuiObject>(pare), WM_COMMAND, wParam, lParam);
			}
			else
			if (wParam == SB_THUMBTRACK)
			{
				s_pos = GetPos(&CGC());
				IGuiCtrl* pare = ExDynCast<IGuiCtrl>(m_Ctrl->GetParent());
				pare->Send(ExDynCast<IGuiObject>(pare), WM_COMMAND, wParam, lParam);
			}
			else
			if (wParam == SB_ENDSCROLL)
			{
				IGuiCtrl* pare = ExDynCast<IGuiCtrl>(m_Ctrl->GetParent());
				pare->Send(ExDynCast<IGuiObject>(pare), WM_COMMAND, wParam, lParam);
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			{
				CGC gc;
				IGuiCtrl* sli = GetSlider(&gc);
				if (!sli) break;
				CRect rect, rc_pt;
				sli->GetWindowRect(rect);
				::GetCursorPos((LPPOINT)&(rc_pt.pt1));
				m_Ctrl->GetBoard()->ScreenToClient(rc_pt.pt1);
				m_Ctrl->B2C(rc_pt);
				rc_pt.pt1 -= CPoint((rect.Left() + rect.Right()) >> 1, (rect.Top() + rect.Bottom()) >> 1);
				m_Ctrl->GetClientRect(rect);
				LONG all = GetAll(&gc);
				bool ori = GetOri(&gc);
				if (ori)
				{
					LONG off = rc_pt.pt1.y * all / rect.Height();
					m_Ctrl->SetState(_T("pos"), (void*)(GetPos(&gc) + off));
				}
				else
				{
					LONG off = rc_pt.pt1.x * all / rect.Width();
					m_Ctrl->SetState(_T("pos"), (void*)(GetPos(&gc) + off));
				}
				IGuiCtrl* pare = ExDynCast<IGuiCtrl>(m_Ctrl->GetParent());
				pare->Send(ExDynCast<IGuiObject>(pare), WM_COMMAND, SB_THUMBPOSITION);
			}
			break;
		case WM_MOUSEWHEEL:
			{
				IGuiCtrl* pare = ExDynCast<IGuiCtrl>(m_Ctrl->GetParent());
				pare->Send(ExDynCast<IGuiObject>(pare), nMessage, wParam, lParam);
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
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		// 处理消息
		switch( nMessage )
		{
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			{
				ctrl->SetCapture();
				ExRandomize();
				m_TimerId = 10001 + ExRandom(89999);
				IGuiBoard* wnd = ExDynCast<IGuiBoard>(ctrl->GetBoard());
				::SetTimer(wnd->GethWnd(), m_TimerId, 40, NULL);
			}
			break;
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			if (m_TimerId)
			{
				IGuiBoard* wnd = ExDynCast<IGuiBoard>(ctrl->GetBoard());
				::KillTimer(wnd->GethWnd(), m_TimerId);
				m_TimerId = 0;
				ctrl->ReleaseCapture();
			}
			break;
		case WM_TIMER:
			if (m_TimerId == wParam)
			{
				CGC gc;
				IGuiCtrl* pare = ExDynCast<IGuiCtrl>(ctrl->GetParent());
				IGuiCtrl::state_t* state = pare->GetState(_T("sli_pos"), &gc);
				if (!state) break;
				LONG pos = (LONG)(LONG_PTR)(state->sta_arr[0]);
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
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		// 处理消息
		switch( nMessage )
		{
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			{
				ctrl->SetCapture();
				ExRandomize();
				m_TimerId = 10001 + ExRandom(89999);
				IGuiBoard* wnd = ExDynCast<IGuiBoard>(ctrl->GetBoard());
				::SetTimer(wnd->GethWnd(), m_TimerId, 40, NULL);
			}
			break;
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			if (m_TimerId)
			{
				IGuiBoard* wnd = ExDynCast<IGuiBoard>(ctrl->GetBoard());
				::KillTimer(wnd->GethWnd(), m_TimerId);
				m_TimerId = 0;
				ctrl->ReleaseCapture();
			}
			break;
		case WM_TIMER:
			if (m_TimerId == wParam)
			{
				CGC gc;
				IGuiCtrl* pare = ExDynCast<IGuiCtrl>(ctrl->GetParent());
				IGuiCtrl::state_t* state = pare->GetState(_T("sli_pos"), &gc);
				if (!state) break;
				LONG pos = (LONG)(LONG_PTR)(state->sta_arr[0]);
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
	IGuiCtrl* m_Ctrl;

public:
	CGuiScrollEvent()
		: m_Ctrl(NULL)
	{}

public:
	// 获得属性
	IGuiCtrl* GetSlider(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("slider"), pGC);
		if (!state) return NULL;
		return (IGuiCtrl*)(state->sta_arr[0]);
	}
	IGuiCtrl* GetUp(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("up"), pGC);
		if (!state) return NULL;
		return (IGuiCtrl*)(state->sta_arr[0]);
	}
	IGuiCtrl* GetDn(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("down"), pGC);
		if (!state) return NULL;
		return (IGuiCtrl*)(state->sta_arr[0]);
	}
	bool GetOri(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("sli_ori"), pGC);
		if (!state) return NULL;
		return (bool)(LONG_PTR)(state->sta_arr[0]);
	}
	LONG GetPos(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("sli_pos"), pGC);
		if (!state) return NULL;
		return (LONG)(LONG_PTR)(state->sta_arr[0]);
	}

	// 格式化位置
	void Format(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl* sli = GetSlider(pGC);
		if (!sli) return;
		IGuiCtrl* up = GetUp(pGC);
		if (!sli) return;
		IGuiCtrl* dn = GetDn(pGC);
		if (!sli) return;
		bool ori = GetOri(pGC);

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
		m_Ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!m_Ctrl) return;

		// 处理消息
		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
		case WM_PAINT:
			{
				CGC gc;
				Format(&gc);
			}
			break;
		case WM_MOUSEWHEEL:
			{
				short scr_cnt = (short)ExHiWord(wParam);
				CGC gc;
				m_Ctrl->SetState(_T("sli_pos"), (void*)(GetPos(&gc) - scr_cnt));
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