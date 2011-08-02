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
			LONG sli_pos = rect.Height() * pos / all;
			LONG sli_fra = sli_pos + rect.Height() * fra / all;
			sli->SetWindowRect(CRect(0, sli_pos, rect.Right(), sli_fra));
		}
		else
		{
			LONG sli_pos = rect.Width() * pos / all;
			LONG sli_fra = sli_pos + rect.Width() * fra / all;
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
				CGC gc;
				bool ori = GetOri(&gc);
				if (ori)
				{
					if (pt->y == 0) break;
					LONG all = GetAll(&gc);
					CRect rect;
					m_Ctrl->GetClientRect(rect);
					LONG off = pt->y * all / rect.Height();
					LONG pos = s_pos + off;
					m_Ctrl->SetState(_T("pos"), (void*)pos);
				}
				else
				{
					if (pt->x == 0) break;
					LONG all = GetAll(&gc);
					CRect rect;
					m_Ctrl->GetClientRect(rect);
					LONG off = pt->x * all / rect.Width();
					LONG pos = s_pos + off;
					m_Ctrl->SetState(_T("pos"), (void*)pos);
				}
			}
			else
			if (wParam == SB_THUMBTRACK)
				s_pos = GetPos(&CGC());
			//else
			//if (wParam == SB_ENDSCROLL)
			//{
			//}
			break;
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			ctrl->SetCapture();
			break;
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			ctrl->ReleaseCapture();
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiSliBlkEvent, IGuiEvent)
EXP_IMPLEMENT_DYNCREATE_CLS(CGuiSliderEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiScrollEvent_hpp__*/