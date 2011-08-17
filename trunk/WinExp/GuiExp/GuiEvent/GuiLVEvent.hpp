// Copyright 2011, ľͷ��
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
// GuiLVEvent - �б���ͼ�ؼ��¼�
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-08-17
// Version:	1.0.0006.0940
//
// History:
//	- 1.0.0000.1125(2011-07-01)	@ ��ʼ����GuiLVEvent
//	- 1.0.0001.1532(2011-07-21)	= ����CGuiLVItemEvent�Ļ�ͼ�㷨
//	- 1.0.0002.1712(2011-08-09)	^ ��������GuiLV��GuiScroll֮��ĽӿڶԽ�
//	- 1.0.0003.2142(2011-08-11)	# ����������GuiListViewû���κ�Item�����µ��ڴ�����쳣
//	- 1.0.0004.1720(2011-08-12)	# ����GuiListView����ʾ������¶�û�м��Ͽհ׸߶ȵ�С����
//	- 1.0.0005.2236(2011-08-15)	^ GuiListView��ʵ���������Ϣ����(����ť,�����ֵ�),���ٴ��������ͼ
//								# �������б���ͼ�е�Items��С��һʱ���ֵĻ��ж����λ
//								# ����ʹ�÷���������б����ʱż�������޷��ƶ���������
//	- 1.0.0006.0940(2011-08-17)	# ��GuiListView�����ӿؼ�ʱ,���ҵķ������Ӧ�ý������Ƶ��ӿؼ���ȥ
//////////////////////////////////////////////////////////////////

#ifndef __GuiLVEvent_hpp__
#define __GuiLVEvent_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiLVItemEvent : public IGuiEvent /*CGuiListView�ڲ�ʹ�õ��б���*/
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiLVItemEvent, IGuiEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
#pragma push_macro("add")
#undef add
#pragma push_macro("sub")
#undef sub

#define add(ite) \
	if (ite == items->Last()) \
		ite = items->Head(); \
	else \
		++ite; \
	if (!IGuiCtrl::IsEffect(*ite)) \
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
	if (!IGuiCtrl::IsEffect(*ite)) \
	{ \
		if (ite == items->Head()) \
			ite = items->Last(); \
		else \
			--ite; \
	}
//#define sub()

		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		typedef CListT<IGuiCtrl*> items_t;

		// ������Ϣ
		switch( nMessage )
		{
		case WM_KEYDOWN:
			{
				IGuiBoard* wnd = ctrl->GetBoard();
				switch (wParam)
				{
				case VK_UP:
					{
						CGC gc;

						IGuiCtrl* comp = ExDynCast<IGuiCtrl>(ctrl->GetParent());
						IGuiCtrl::state_t* state = comp->GetState(_T("items"), &gc);
						if (!state) break;
						items_t* items = (items_t*)(state->sta_arr[0]);

						CRect rc_me, rc_it;
						ctrl->GetWindowRect(rc_me);
						LONG of_it = -1;

						items_t::iterator_t ite = items_t::finder_t::Find(*items, ctrl);
						sub(ite);

						while(true)
						{
							IGuiCtrl* it = (*ite);
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
						CGC gc;

						IGuiCtrl* comp = ExDynCast<IGuiCtrl>(ctrl->GetParent());
						IGuiCtrl::state_t* state = comp->GetState(_T("items"), &gc);
						if (!state) break;
						items_t* items = (items_t*)(state->sta_arr[0]);

						CRect rc_me, rc_it;
						ctrl->GetWindowRect(rc_me);
						LONG of_it = -1;

						items_t::iterator_t ite = items_t::finder_t::Find(*items, ctrl);
						add(ite);

						while(true)
						{
							IGuiCtrl* it = (*ite);
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
				case VK_LEFT:
					{
						CGC gc;

						IGuiCtrl* comp = ExDynCast<IGuiCtrl>(ctrl->GetParent());
						IGuiCtrl::state_t* state = comp->GetState(_T("items"), &gc);
						if (!state) break;
						items_t* items = (items_t*)(state->sta_arr[0]);

						items_t::iterator_t ite = items_t::finder_t::Find(*items, ctrl);
						sub(ite);
						(*ite)->SetFocus();
					}
					break;
				case VK_RIGHT:
					{
						CGC gc;

						IGuiCtrl* comp = ExDynCast<IGuiCtrl>(ctrl->GetParent());
						IGuiCtrl::state_t* state = comp->GetState(_T("items"), &gc);
						if (!state) break;
						items_t* items = (items_t*)(state->sta_arr[0]);

						items_t::iterator_t ite = items_t::finder_t::Find(*items, ctrl);
						add(ite);
						(*ite)->SetFocus();
					}
					break;
				}
			}
			break;
		case WM_MOUSEWHEEL:
			{
				IGuiCtrl* pare = ExDynCast<IGuiCtrl>(ctrl->GetParent());
				pare->Send(ExDynCast<IGuiObject>(pare), nMessage, wParam, lParam);
			}
			break;
		}
#pragma pop_macro("sub")
#pragma pop_macro("add")
	}
};

//////////////////////////////////////////////////////////////////

class CGuiLVEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiLVEvent, IGuiEvent)

protected:
	IGuiCtrl* m_Ctrl;
	IGuiCtrl* m_FocItm;

public:
	CGuiLVEvent()
		: m_Ctrl(NULL)
		, m_FocItm(NULL)
	{}

public:
	// �������
	IGuiCtrl* GetFocPic(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("foc"), pGC);
		if (!state) return NULL;
		return (IGuiCtrl*)(state->sta_arr[0]);
	}
	CListT<IGuiCtrl*>* GetItems(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("items"), pGC);
		if (!state) return NULL;
		return (CListT<IGuiCtrl*>*)(state->sta_arr[0]);
	}
	LONG GetSpace(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("space"), pGC);
		if (!state) return NULL;
		return (LONG)(LONG_PTR)(state->sta_arr[0]);
	}
	CImage* GetFocImage(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("foc_image"), pGC);
		if (!state) return NULL;
		return (CImage*)(state->sta_arr[0]);
	}

	// ��ʽ���б����λ��
	void FormatItems(CGC* pGC)
	{
		ExAssert(m_Ctrl);

		typedef CListT<IGuiCtrl*> items_t;

		// �������
		items_t* items = GetItems(pGC);
		if (!items || items->Empty()) return;
		LONG space = GetSpace(pGC);

		CRect rect;
		m_Ctrl->GetClientRect(rect);
		CSize scr_sz;
		m_Ctrl->GetScrollSize(scr_sz);

		// �����б���
		LONG all_line = 0;
		CRect itm_rc, lst_rc/*������ߵ�itm*/, 
			old_rc(scr_sz.cx, space - scr_sz.cy, scr_sz.cx, space - scr_sz.cy);
		for(items_t::iterator_t ite = items->Head(); ite != items->Tail(); ++ite)
		{
			IGuiCtrl* item = *ite;
			if (!item) continue;
			// ��ȡ��ǰ�������
			item->GetWindowRect(itm_rc);
			// ��������
			itm_rc.MoveTo(CPoint(old_rc.Right() + space, old_rc.Top()));
			if (itm_rc.Right() > rect.Right())
			{
				itm_rc.MoveTo(CPoint(rect.Left() + space, lst_rc.Bottom() + space));
				lst_rc = itm_rc;
			}
			// ���õ�ǰ������
			item->SetWindowRect(itm_rc);
			// �洢����
			old_rc = itm_rc;
			if (itm_rc.Height() > lst_rc.Height())
				lst_rc = itm_rc;
		}

		// ���ù�������
		itm_rc.pt2.y += (scr_sz.cy + space);
		m_Ctrl->SetState(_T("fra_line"), (void*)rect.Height());
		m_Ctrl->SetState(_T("all_line"), (void*)itm_rc.Bottom());
	}
	void ShowItem(IGuiCtrl* pItem)
	{
		if (!pItem) return;
		ExAssert(m_Ctrl);

		CSize scr_sz;
		m_Ctrl->GetScrollSize(scr_sz);
		CRect scr_rc;
		m_Ctrl->GetClientRect(scr_rc);
		CRect itm_rc;
		pItem->GetWindowRect(itm_rc);

		LONG off1 = 0, off2 = 0;
		if (!scr_rc.PtInRect(itm_rc.pt1))
			off1 = itm_rc.pt1.y - scr_rc.pt1.y;
		if (!scr_rc.PtInRect(itm_rc.pt2))
			off2 = itm_rc.pt2.y - scr_rc.pt2.y;
		scr_sz.cy += 
			(off1 && off2 ? 
			(abs(off1) < abs(off2) ? off1 : off2) : 
			(off1 ? off1 : off2));
		m_Ctrl->SetScrollSize(scr_sz, true);
	}

	// ��Ϣ��Ӧ
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		m_Ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!m_Ctrl) return;

		// ������Ϣ
		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
			{
				CGC gc;
				FormatItems(&gc);
			}
			break;
		case WM_SETFOCUS:
			if (m_Ctrl == IGuiCtrl::GetFocus())
			{
				CGC gc;
				CListT<IGuiCtrl*>* items = GetItems(&gc);
				if (items->Empty()) break;
				if(!m_FocItm)
					m_FocItm = items->HeadItem();
				if(!m_FocItm) break;
				IGuiCtrl* pic = GetFocPic(&gc);
				if(!pic) break;

				CRect foc_rct;
				m_FocItm->GetWindowRect(foc_rct);
				pic->SetWindowRect(foc_rct);
				pic->SetVisible(true);
				break;
			}
		case WM_KILLFOCUS:
			{
				CGC gc;
				IGuiCtrl* pic = GetFocPic(&gc);
				if(!pic) break;
				pic->SetVisible(false);
			}
			break;
		case WM_KEYDOWN:
			if (!m_FocItm) break;
			if (wParam == VK_SPACE)
				m_FocItm->SetFocus();
			else
				m_FocItm->Send(ExDynCast<IGuiObject>(m_FocItm), nMessage, wParam, lParam);
			break;
		case WM_COMMAND:
			if (BN_SETFOCUS == wParam)
				ShowItem(m_FocItm = (IGuiCtrl*)lParam);
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiLVItemEvent, IGuiEvent)
EXP_IMPLEMENT_DYNCREATE_CLS(CGuiLVEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiLVEvent_hpp__*/