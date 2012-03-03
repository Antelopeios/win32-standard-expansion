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
// GuiLVEvent - �б���ͼ�ؼ��¼�
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-01
// Version:	1.0.0013.1237
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
//	- 1.0.0007.2317(2011-08-30)	+ ���GuiListView�б�����/�²�ͬ���뷽ʽ������ʵ��
//								# ������GuiListView���С��ĳ�б�����ʱ������б������г���
//	- 1.0.0008.2333(2011-08-31)	= ��GuiListViewû�й���������ʱ,����б���������б���ͼ����
//	- 1.0.0009.1035(2011-09-01)	# ���������б����Զ��������ܴ��ڵ�Сbug,ͬʱ�ع���һ�����
//	- 1.0.0010.1709(2011-09-05)	# �������б���Ϊ��ʱ,��ʽ���б���ĺ�������ˢ��all_line��fra_line���Ե�����
//	- 1.0.0011.1154(2011-09-06)	# �������б���Ϊ��ʱ,����û�н�����ָ���ÿ�,�������б��Ͱ�����Ϣʱ����
//	- 1.0.0012.1740(2012-02-29)	= ��ϵײ���������ƻ��Ƶ��޸�,����GuiLVEvent�о�����߼�ʵ��
//	- 1.0.0013.1237(2012-03-01)	^ ��GuiLVEvent�е�ͨ���߼�����GuiListEvent��ʵ��
//////////////////////////////////////////////////////////////////

#ifndef __GuiLVEvent_hpp__
#define __GuiLVEvent_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiLVItemEvent : public CGuiListItemEvent /*CGuiListView�ڲ�ʹ�õ��б���*/
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiLVItemEvent, CGuiListItemEvent)

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		typedef IGuiCtl::items_t items_t;

		// ������Ϣ
		switch( nMessage )
		{
		case WM_KEYDOWN:
			{
				IGuiWnd* wnd = ctl->GetWnd();
				switch (wParam)
				{
				case VK_LEFT:
					{
						IGuiCtl* comp = ExDynCast<IGuiCtl>(ctl->GetParent());
						items_t* items = (items_t*)comp->GetState(_T("items"));
						items_t::iterator_t ite = items->Find(ctl);
						sub(ite);
						(*ite)->SetFocus();
					}
					break;
				case VK_RIGHT:
					{
						IGuiCtl* comp = ExDynCast<IGuiCtl>(ctl->GetParent());
						items_t* items = (items_t*)comp->GetState(_T("items"));
						items_t::iterator_t ite = items->Find(ctl);
						add(ite);
						(*ite)->SetFocus();
					}
					break;
				default:
					EXP_BASE::OnMessage(pGui, nMessage, wParam, lParam);
				}
			}
			break;
		default:
			EXP_BASE::OnMessage(pGui, nMessage, wParam, lParam);
		}
	}
};

#pragma pop_macro("sub")
#pragma pop_macro("add")

//////////////////////////////////////////////////////////////////

class CGuiLVEvent : public CGuiListEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiLVEvent, CGuiListEvent)

public:
	CGuiLVEvent()
	{}

public:
	// ��ʽ���б����λ��
	void FormatItems()
	{
		ExAssert(m_Ctrl);

		typedef IGuiCtl::items_t items_t;

		// �������
		items_t* items = GetItems();
		if (items->Empty()) m_FocItm = NULL;
		LONG space = GetSpace();
		BOOL b_top = (BOOL)(LONG_PTR)m_Ctrl->GetState(_T("align_top"));

		CRect rect;
		m_Ctrl->GetClientRect(rect);

		// �����б���
		LONG all_line = 0;
		CRect itm_rc, him_rc/*������ߵ�itm*/, old_rc(0, space, 0, space);
		if (b_top)
		{	// ���϶���
			for(items_t::iterator_t ite = items->Head(); ite != items->Tail(); ++ite)
			{
				IGuiCtl* item = *ite;
				if (!item) continue;
				// ��ȡ��ǰ�������
				item->GetRect(itm_rc);
				// ��������
				itm_rc.MoveTo(CPoint(old_rc.Right() + space, old_rc.Top()));
				if (itm_rc.Right() > rect.Right() && itm_rc.Left() > space)
				{
					itm_rc.MoveTo(CPoint(rect.Left() + space, him_rc.Bottom() + space));
					him_rc = itm_rc;
				}
				// ���õ�ǰ������
				item->SetRect(itm_rc);
				// �洢����
				old_rc = itm_rc;
				if (itm_rc.Height() > him_rc.Height())
					him_rc = itm_rc;
			}
		}
		else
		{	// ���¶���
			for(items_t::iterator_t ite = items->Head(); ite != items->Tail();)
			{	// ��һ�α���,��ȡ��һ������ߵĸ߶�
				items_t::iterator_t it = ite;
				for(long w_itm = 0; it != items->Tail(); ++it)
				{
					IGuiCtl* item = *it;
					if (!item) continue;
					// ��ȡ��ǰ�������
					w_itm += space;
					item->GetWindowRect(itm_rc);
					w_itm += itm_rc.Width();
					if (w_itm > rect.Right() && it != ite) break;
					// �洢����
					if (itm_rc.Height() > him_rc.Height())
						him_rc = itm_rc;
				}
				old_rc.Height(him_rc.Height());
				// �ڶ��α���,�����߶ȵ��¶˶���һ��
				it = ite;
				for(long w_itm = 0; it != items->Tail(); ++it)
				{
					IGuiCtl* item = *it;
					if (!item) continue;
					// ��ȡ��ǰ�������
					w_itm += space;
					item->GetRect(itm_rc);
					w_itm += itm_rc.Width();
					if (w_itm > rect.Right() && it != ite) break;
					// ��������
					itm_rc.MoveTo(CPoint(old_rc.Right() + space, old_rc.Bottom() - itm_rc.Height()));
					// ���õ�ǰ������
					item->SetRect(itm_rc);
					// �洢����
					old_rc.Left(itm_rc.Left());
					old_rc.Right(itm_rc.Right());
				}
				// ����ƶ�һ��
				old_rc.MoveTo(CPoint(0, old_rc.Bottom() + space));
				old_rc.Width(0);
				// ������ָ����һ��
				ite = it;
			}
		}

		// ���ù�������
		itm_rc.pt2.y += space;
		m_Ctrl->SetFraRect(CSize(0, rect.Height()));
		m_Ctrl->SetAllRect(CSize(0, itm_rc.Bottom()));
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiLVItemEvent, CGuiListItemEvent)
EXP_IMPLEMENT_DYNCREATE_CLS(CGuiLVEvent, CGuiListEvent)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiLVEvent_hpp__*/