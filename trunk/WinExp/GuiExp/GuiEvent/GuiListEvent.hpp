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
// GuiListEvent - 列表控件事件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-01
// Version:	1.0.0000.1133
//
// History:
//	- 1.0.0000.1133(2012-03-01)	@ 开始构建GuiListEvent
//////////////////////////////////////////////////////////////////

#ifndef __GuiListEvent_hpp__
#define __GuiListEvent_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

#pragma push_macro("add")
#undef add
#pragma push_macro("sub")
#undef sub

#define add(ite) \
	if (ite == items->Last()) \
		ite = items->Head(); \
	else \
		++ite; \
	if (!IGuiCtl::IsEffect(*ite)) \
	{ \
		if (ite == items->Last()) \
			ite = items->Head(); \
		else \
			++ite; \
	}
//#define add()
#define sub(ite) \
	if (ite == items->Head()) \
		ite = items->Last(); \
	else \
		--ite; \
	if (!IGuiCtl::IsEffect(*ite)) \
	{ \
		if (ite == items->Head()) \
			ite = items->Last(); \
		else \
			--ite; \
	}
//#define sub()

class CGuiListItemEvent : public IGuiEvent /*CGuiList内部使用的列表项*/
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiListItemEvent, IGuiEvent)
		
public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		typedef IGuiCtl::items_t items_t;

		// 处理消息
		switch( nMessage )
		{
		case WM_KEYDOWN:
			{
				IGuiWnd* wnd = ctl->GetWnd();
				switch (wParam)
				{
				case VK_UP:
					{
						IGuiCtl* comp = ExDynCast<IGuiCtl>(ctl->GetParent());
						items_t* items = (items_t*)comp->GetState(_T("items"));

						CRect rc_me, rc_it;
						ctl->GetWindowRect(rc_me);
						LONG of_it = -1;

						items_t::iterator_t ite = items->Find(ctl);
						sub(ite);

						while(TRUE)
						{
							IGuiCtl* it = (*ite);
							it->GetWindowRect(rc_it);
							if (rc_me.Top() != rc_it.Top())
							{
								LONG temp = abs(rc_me.Left() - rc_it.Left());
								if (of_it >= 0 && of_it < temp)
								{
									add(ite);
									it = (*ite);
									it->SetFocus();
									break;
								}
								else
								if (temp == 0)
								{
									it = (*ite);
									it->SetFocus();
									break;
								}
								of_it = temp;
							}
							sub(ite);
						}
					}
					break;
				case VK_DOWN:
					{
						IGuiCtl* comp = ExDynCast<IGuiCtl>(ctl->GetParent());
						items_t* items = (items_t*)comp->GetState(_T("items"));

						CRect rc_me, rc_it;
						ctl->GetWindowRect(rc_me);
						LONG of_it = -1;

						items_t::iterator_t ite = items->Find(ctl);
						add(ite);

						while(TRUE)
						{
							IGuiCtl* it = (*ite);
							it->GetWindowRect(rc_it);
							if (rc_me.Top() != rc_it.Top())
							{
								LONG temp = abs(rc_me.Left() - rc_it.Left());
								if (of_it >= 0 && of_it < temp)
								{
									sub(ite);
									it = (*ite);
									it->SetFocus();
									break;
								}
								else
								if (temp == 0)
								{
									it = (*ite);
									it->SetFocus();
									break;
								}
								of_it = temp;
							}
							add(ite);
						}
					}
					break;
				}
			}
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

class CGuiListEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiListEvent, IGuiEvent)

protected:
	IGuiCtl* m_Ctrl;
	IGuiCtl* m_FocItm;

public:
	CGuiListEvent()
		: m_Ctrl(NULL)
		, m_FocItm(NULL)
	{}

public:
	// 获得属性
	IGuiCtl* GetFocPic()
	{
		return (IGuiCtl*)m_Ctrl->GetState(_T("foc"));
	}
	void* GetItems()
	{
		return m_Ctrl->GetState(_T("items"));
	}
	LONG GetSpace()
	{
		return (LONG)(LONG_PTR)m_Ctrl->GetState(_T("space"));
	}
	CImage* GetFocImage()
	{
		return (CImage*)m_Ctrl->GetState(_T("foc_image"));
	}

	// 格式化列表项的位置
	virtual void FormatItems()
	{
		ExAssert(m_Ctrl);

		typedef IGuiCtl::items_t items_t;
		static BOOL first_time = TRUE;

		// 获得属性
		items_t* items = (items_t*)GetItems();
		if (items->Empty()) m_FocItm = NULL;
		LONG space = GetSpace();

		CRect rect;
		m_Ctrl->GetClientRect(rect);

		// 遍历列表项
		LONG all_line = 0;
		CRect itm_rc;
		CSize old_sz;
		if (first_time)
		{
			first_time = FALSE;

			for(items_t::iterator_t ite = items->Head(); ite != items->Tail(); ++ite)
			{
				IGuiCtl* item = *ite;
				if (!item || !item->IsVisible()) continue;
				// 获取当前项的区域
				item->GetRect(itm_rc);
				// 调整区域
				itm_rc.MoveTo(CPoint(old_sz.cx, old_sz.cy + space));
				itm_rc.Width(rect.Width());
				// 设置当前项区域
				item->SetRect(itm_rc);
				// 存储区域
				old_sz.cy = itm_rc.Bottom();
			}

			// 设置滚动区域
			old_sz.cy += space;
			m_Ctrl->SetAllRect(CSize(0, old_sz.cy));
		}
		else
		{
			for(items_t::iterator_t ite = items->Head(); ite != items->Tail(); ++ite)
			{
				IGuiCtl* item = *ite;
				if (!item || !item->IsVisible()) continue;
				// 获取当前项的区域
				item->GetRect(itm_rc);
				// 调整区域
				itm_rc.Width(rect.Width());
				// 设置当前项区域
				item->SetRect(itm_rc);
			}
		}

		// 设置滚动区域
		m_Ctrl->SetFraRect(CSize(0, rect.Height()));
	}
	virtual void ShowItem(IGuiCtl* pItem)
	{
		if (!pItem) return;
		ExAssert(m_Ctrl);
		CSize scr_sz;
		pItem->GetDisplaySize(scr_sz);
		m_Ctrl->SetScrollSize(scr_sz, TRUE);
	}

	// 消息响应
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		m_Ctrl = ExDynCast<IGuiCtl>(pGui);
		if (!m_Ctrl) return;

		// 处理消息
		switch( nMessage )
		{
		case WM_SIZE:
			FormatItems();
			break;
		case WM_SETFOCUS:
			if (m_Ctrl == IGuiCtl::GetFocus())
			{
				IGuiCtl::items_t* items = (IGuiCtl::items_t*)GetItems();
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
		case WM_KILLFOCUS:
			{
				IGuiCtl* pic = GetFocPic();
				if(!pic) break;
				pic->SetVisible(FALSE);
			}
			break;
		case WM_KEYDOWN:
			if (!m_FocItm) break;
			if (wParam == VK_SPACE)
				m_FocItm->SetFocus();
			else
				m_FocItm->SendMessage(nMessage, wParam, lParam);
			break;
		case WM_COMMAND:
			if (BN_SETFOCUS == wParam)
				ShowItem(m_FocItm = (IGuiCtl*)lParam);
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiListItemEvent, IGuiEvent)
EXP_IMPLEMENT_DYNCREATE_CLS(CGuiListEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiListEvent_hpp__*/
