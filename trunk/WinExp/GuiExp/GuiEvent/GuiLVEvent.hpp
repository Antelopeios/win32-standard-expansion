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
// Date:	2011-07-01
// Version:	1.0.0000.1125
//
// History:
//	- 1.0.0000.1125(2011-07-01)	@ ��ʼ����GuiLVEvent
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

protected:
	CImage* m_IconOld;
	CImage m_IconTmp;

public:
	CGuiLVItemEvent()
		: m_IconOld(NULL)
	{}

public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		// ������Ϣ
		switch( nMessage )
		{
		case WM_PAINT:
			if (lParam)
			{
				CGC gc;

				// �������
				IGuiCtrl::state_t* state = ctrl->GetState(_T("icon"), &gc);
				if (!state) break;
				CImage* icon = (CImage*)(((void**)state->sta_arr)[0]);
				if (!icon || icon->IsNull()) break;

				state = ctrl->GetState(_T("glow"), &gc);
				if (!state) break;
				bool glow = (bool)(LONG_PTR)(((void**)state->sta_arr)[0]);

				CImage* mem_img = (CImage*)lParam;
				if (!mem_img || mem_img->IsNull()) break;
				CRect rect;
				ctrl->GetClientRect(rect);

				// ����
				if (glow)
				{
					if (m_IconOld != icon)
					{
						CFilterGauss filter;
						CPoint pt_flt(filter.m_Radius << 1, filter.m_Radius << 1);
						// ��ͼƬ����
						CRect rc(0, 0, icon->GetWidth(), icon->GetHeight());
						m_IconTmp = icon->Clone(rc + pt_flt);
						// ��Ӱ��
						CImgFilter::Filter(m_IconTmp, CRect(), &CFilterFill(0, 0xe));
						CImgFilter::Filter(m_IconTmp, CRect(), &filter);
						// ��Ӱ����
						rc.Offset(pt_flt);
						CImgRenderer::Render(m_IconTmp, icon->Get(), rc, CPoint(), &CRenderOverlay());
						// ����ָ��
						m_IconOld = icon;
					}
					icon = &m_IconTmp;
				}

				// ��ͼ
				state = ctrl->GetState(_T("locate"), &gc);
				if (!state) break;
				DWORD locate = (DWORD)(((void**)state->sta_arr)[0]);
				state = ctrl->GetState(_T("loc_off"), &gc);
				if (!state) break;
				LONG loc_off = (LONG)(((void**)state->sta_arr)[0]);

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
				CImgRenderer::Render(mem_img->Get(), icon->Get(), icon_rct, CPoint());
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CGuiLVEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiLVEvent, IGuiEvent)

protected:
	IGuiCtrl* m_Ctrl;

public:
	CGuiLVEvent()
		: m_Ctrl(NULL)
	{}

public:
	// �������
	CListT<IGuiCtrl*>* GetItems(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("items"), pGC);
		if (!state) return NULL;
		return (CListT<IGuiCtrl*>*)(((void**)state->sta_arr)[0]);
	}
	LONG GetSpace(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("space"), pGC);
		if (!state) return NULL;
		return (LONG)(LONG_PTR)(((void**)state->sta_arr)[0]);
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

		// �����б���
		CRect itm_rc, old_rc(0, space, 0, space);
		for(items_t::iterator_t ite = items->Head(); ite != items->Tail(); ++ite)
		{
			IGuiCtrl* item = *ite;
			if (!item) continue;
			// ��ȡ��ǰ�������
			item->GetWindowRect(itm_rc);
			// ��������
			itm_rc.MoveTo(CPoint(old_rc.Right() + space, old_rc.Top()));
			if (itm_rc.Right() + space > rect.Right())
				itm_rc.MoveTo(CPoint(rect.Left() + space, old_rc.Bottom() + space));
			// ���õ�ǰ������
			item->SetWindowRect(itm_rc);
			// �洢����
			old_rc = itm_rc;
		}
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
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiLVItemEvent, IGuiEvent)
EXP_IMPLEMENT_DYNCREATE_CLS(CGuiLVEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiLVEvent_hpp__*/