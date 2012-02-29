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
// GuiLVEvent - 列表视图控件事件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-02-29
// Version:	1.0.0012.1740
//
// History:
//	- 1.0.0000.1125(2011-07-01)	@ 开始构建GuiLVEvent
//	- 1.0.0001.1532(2011-07-21)	= 调整CGuiLVItemEvent的绘图算法
//	- 1.0.0002.1712(2011-08-09)	^ 基本完善GuiLV与GuiScroll之间的接口对接
//	- 1.0.0003.2142(2011-08-11)	# 修正几个因GuiListView没有任何Item而导致的内存访问异常
//	- 1.0.0004.1720(2011-08-12)	# 修正GuiListView总显示区域的下端没有加上空白高度的小问题
//	- 1.0.0005.2236(2011-08-15)	^ GuiListView仅实现特殊的消息处理(方向按钮,鼠标滚轮等),不再处理基本绘图
//								# 修正当列表视图中的Items大小不一时出现的换行对齐错位
//								# 修正使用方向键控制列表项焦点时偶尔出现无法移动焦点的情况
//	- 1.0.0006.0940(2011-08-17)	# 当GuiListView中有子控件时,左右的方向键不应该将焦点移到子控件上去
//	- 1.0.0007.2317(2011-08-30)	+ 添加GuiListView列表项上/下不同对齐方式的排列实现
//								# 修正当GuiListView宽度小于某列表项宽度时引起的列表项排列出错
//	- 1.0.0008.2333(2011-08-31)	= 当GuiListView没有关联滚动条时,点击列表项不会引起列表视图滚动
//	- 1.0.0009.1035(2011-09-01)	# 修正焦点列表项自动滚动功能存在的小bug,同时回滚上一项更新
//	- 1.0.0010.1709(2011-09-05)	# 修正当列表项为空时,格式化列表项的函数不会刷新all_line与fra_line属性的问题
//	- 1.0.0011.1154(2011-09-06)	# 修正当列表项为空时,由于没有将焦点指针置空,导致向列表发送按键消息时崩溃
//	- 1.0.0012.1740(2012-02-29)	= 配合底层滚动条控制机制的修改,调整GuiLVEvent中具体的逻辑实现
//////////////////////////////////////////////////////////////////

#ifndef __GuiLVEvent_hpp__
#define __GuiLVEvent_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiLVItemEvent : public IGuiEvent /*CGuiListView内部使用的列表项*/
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
#pragma pop_macro("sub")
#pragma pop_macro("add")
	}
};

//////////////////////////////////////////////////////////////////

class CGuiLVEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiLVEvent, IGuiEvent)

protected:
	IGuiCtrlBase* m_Ctrl;
	IGuiCtl* m_FocItm;

public:
	CGuiLVEvent()
		: m_Ctrl(NULL)
		, m_FocItm(NULL)
	{}

public:
	// 获得属性
	IGuiCtl* GetFocPic()
	{
		return (IGuiCtl*)m_Ctrl->GetState(_T("foc"));
	}
	IGuiCtl::items_t* GetItems()
	{
		return (IGuiCtl::items_t*)(m_Ctrl->GetState(_T("items")));
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
	void FormatItems()
	{
		ExAssert(m_Ctrl);

		typedef IGuiCtl::items_t items_t;

		// 获得属性
		items_t* items = GetItems();
		if (items->Empty()) m_FocItm = NULL;
		LONG space = GetSpace();
		BOOL b_top = (BOOL)(LONG_PTR)m_Ctrl->GetState(_T("align_top"));

		CRect rect;
		m_Ctrl->GetClientRect(rect);
		CSize scr_sz;
		m_Ctrl->GetScrollSize(scr_sz);

		// 遍历列表项
		LONG all_line = 0;
		CRect itm_rc, him_rc/*保存最高的itm*/, 
			old_rc(scr_sz.cx, space - scr_sz.cy, scr_sz.cx, space - scr_sz.cy);
		if (b_top)
		{	// 向上对齐
			for(items_t::iterator_t ite = items->Head(); ite != items->Tail(); ++ite)
			{
				IGuiCtl* item = *ite;
				if (!item) continue;
				// 获取当前项的区域
				item->GetWindowRect(itm_rc);
				// 调整区域
				itm_rc.MoveTo(CPoint(old_rc.Right() + space, old_rc.Top()));
				if (itm_rc.Right() > rect.Right() && itm_rc.Left() > scr_sz.cx + space)
				{
					itm_rc.MoveTo(CPoint(rect.Left() + space, him_rc.Bottom() + space));
					him_rc = itm_rc;
				}
				// 设置当前项区域
				item->SetWindowRect(itm_rc);
				// 存储区域
				old_rc = itm_rc;
				if (itm_rc.Height() > him_rc.Height())
					him_rc = itm_rc;
			}
		}
		else
		{	// 向下对齐
			for(items_t::iterator_t ite = items->Head(); ite != items->Tail();)
			{	// 第一次遍历,先取得一行中最高的高度
				items_t::iterator_t it = ite;
				for(long w_itm = 0; it != items->Tail(); ++it)
				{
					IGuiCtl* item = *it;
					if (!item) continue;
					// 获取当前项的区域
					w_itm += space;
					item->GetWindowRect(itm_rc);
					w_itm += itm_rc.Width();
					if (w_itm > rect.Right() && it != ite) break;
					// 存储区域
					if (itm_rc.Height() > him_rc.Height())
						him_rc = itm_rc;
				}
				old_rc.Height(him_rc.Height());
				// 第二次遍历,以最大高度的下端对齐一行
				it = ite;
				for(long w_itm = 0; it != items->Tail(); ++it)
				{
					IGuiCtl* item = *it;
					if (!item) continue;
					// 获取当前项的区域
					w_itm += space;
					item->GetWindowRect(itm_rc);
					w_itm += itm_rc.Width();
					if (w_itm > rect.Right() && it != ite) break;
					// 调整区域
					itm_rc.MoveTo(CPoint(old_rc.Right() + space, old_rc.Bottom() - itm_rc.Height()));
					// 设置当前项区域
					item->SetWindowRect(itm_rc);
					// 存储区域
					old_rc.Left(itm_rc.Left());
					old_rc.Right(itm_rc.Right());
				}
				// 起点移动一行
				old_rc.MoveTo(CPoint(scr_sz.cx, old_rc.Bottom() + space));
				old_rc.Width(0);
				// 迭代器指向下一行
				ite = it;
			}
		}

		// 设置滚动区域
		itm_rc.pt2.y += (scr_sz.cy + space);
		m_Ctrl->SetFraRect(CSize(0, rect.Height()));
		m_Ctrl->SetAllRect(CSize(0, itm_rc.Bottom()));
	}
	void ShowItem(IGuiCtl* pItem)
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
		if (scr_rc.Top() > itm_rc.Top())
			off1 = itm_rc.Top() - scr_rc.Top();
		if (scr_rc.Bottom() < itm_rc.Bottom())
			off2 = itm_rc.Bottom() - scr_rc.Bottom();
		scr_sz.cy += 
			(off1 && off2 ? 
			(abs(off1) < abs(off2) ? off1 : off2) : 
			(off1 ? off1 : off2));
		m_Ctrl->SetScrollSize(scr_sz, TRUE);
	}

	// 消息响应
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		m_Ctrl = ExDynCast<IGuiCtrlBase>(pGui);
		if (!m_Ctrl) return;

		// 处理消息
		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
			FormatItems();
			break;
		case WM_SETFOCUS:
			if (m_Ctrl == IGuiCtl::GetFocus())
			{
				IGuiCtl::items_t* items = GetItems();
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
				break;
			}
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

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiLVItemEvent, IGuiEvent)
EXP_IMPLEMENT_DYNCREATE_CLS(CGuiLVEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiLVEvent_hpp__*/