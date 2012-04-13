// Copyright 2012, 木头云
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
// GuiTreeEvent - 树控件事件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-04-10
// Version:	1.0.0000.1430
//
// History:
//	- 1.0.0000.1430(2012-04-10)	@ 准备构建GuiTreeEvent
//////////////////////////////////////////////////////////////////

#ifndef __GuiTreeEvent_hpp__
#define __GuiTreeEvent_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiTreeEvent : public CGuiListEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiTreeEvent, CGuiListEvent)

public:
	typedef IGuiCtl::itree_t items_t;

public:
	void FormatItems(items_t::iterator_t ite, LONG space, LONG offst, CPoint& pt, LONG& w)
	{
		IGuiCtl* itm = *ite;
		if(!itm->IsVisible()) return;

		CRect rc;
		itm->GetRect(rc);
		rc.MoveTo(pt);
		itm->SetRect(rc);

		if (w < rc.Right()) w = rc.Right();

		pt.x += offst;
		pt.y = rc.Bottom() + space;

		// 遍历列表项
		items_t::ite_list_t ite_lst = ite->Children();
		items_t::ite_list_t::iterator_t ite_ite = ite_lst.Head();
		for(; ite_ite != ite_lst.Tail(); ++ite_ite)
			FormatItems(*ite_ite, space, offst, pt, w);

		pt.x -= offst;
	}
	void FormatItems()
	{
		ExAssert(m_Ctrl);

		// 获得属性
		items_t* items = (items_t*)GetItems();
		if (items->Empty()) m_FocItm = NULL;
		LONG space = GetSpace();
		LONG offst = (LONG)(LONG_PTR)m_Ctrl->GetState(_T("offset"));

		CRect rect;
		m_Ctrl->GetClientRect(rect);

		CPoint pt;
		LONG w = 0;

		// 遍历列表项
		items_t::iterator_t ite = items->Head();
		items_t::ite_list_t ite_lst = ite->Children();
		items_t::ite_list_t::iterator_t ite_ite = ite_lst.Head();
		for(; ite_ite != ite_lst.Tail(); ++ite_ite)
			FormatItems(*ite_ite, space, offst, pt, w);

		// 设置滚动区域
		m_Ctrl->SetAllRect(CSize(w, pt.y));
		m_Ctrl->SetFraRect(CSize(rect.Width(), rect.Height()));
	}

	// 消息响应
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		switch( nMessage )
		{
		case WM_SETFOCUS:
			if (m_Ctrl == IGuiCtl::GetFocus())
			{
				items_t* items = (items_t*)GetItems();
				if (items->Empty()) break;
				if(!m_FocItm)
					m_FocItm = items->HeadItem();
				if(!m_FocItm) break;
				IGuiCtl* pic = GetFocPic();
				if(!pic) break;

				CRect foc_rct;
				m_FocItm->GetWindowRect(foc_rct);
				pic->SetWindowRect(foc_rct);
				pic->SetVisible(TRUE);
			}
			break;
		default:
			EXP_BASE::OnMessage(pGui, nMessage, wParam, lParam);
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiTreeEvent, CGuiListEvent)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiTreeEvent_hpp__*/
