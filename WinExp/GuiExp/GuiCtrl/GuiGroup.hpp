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
// GuiGroup - 组框控件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-07-21
// Version:	1.0.0002.1532
//
// History:
//	- 1.0.0000.0940(2011-07-07)	@ 开始构建GuiGroup
//	- 1.0.0001.1752(2011-07-11)	^ 优化GuiGroup的外部调用属性
//	- 1.0.0002.1532(2011-07-21)	= 将CGuiGroup内部items_t结构由指针改为对象,减轻调用复杂度
//////////////////////////////////////////////////////////////////

#ifndef __GuiGroup_hpp__
#define __GuiGroup_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////

class CGuiGroup : public IGuiCtrlBase
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiGroup, IGuiCtrlBase)

public:
	typedef CArrayT<IGuiCtrl*> items_t;

protected:
	items_t m_ItemList;
	DWORD m_StatusCount;
	bool m_StyleBox;

public:
	CGuiGroup()
		: m_ItemList(NULL)
		, m_StatusCount(3)
		, m_StyleBox(true)
	{}

public:
	// 获得控件状态
	state_t* GetState(const CString& sType, CGC* pGC = NULL)
	{
		state_t* state = EXP_BASE::GetState(sType, pGC);
		if (state)
		{
			if (state->sta_typ == _T("items"))
				state->sta_arr.Add(&m_ItemList);
		}
		return state;
	}
	void SetState(const CString& sType, void* pState)
	{
		if (sType == _T("items"))
		{
			items_t* new_sta = (items_t*)pState;
			if (new_sta == NULL) return;
			for(items_t::iterator_t ite = m_ItemList.Head(); ite != m_ItemList.Tail(); ++ite)
			{
				IGuiCtrl* item = *ite;
				if (!item) continue;
				items_t::iterator_t it = items_t::finder_t::Find(*new_sta, item);
				if (it == new_sta->Tail()) DelComp(item);
			}
			m_ItemList = *(items_t*)pState;
			for(items_t::iterator_t ite = m_ItemList.Head(); ite != m_ItemList.Tail(); ++ite)
			{
				IGuiCtrl* item = *ite;
				if (!item) continue;
				AddComp(item);
			}
			IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("sta_cnt"))
			m_StatusCount = (DWORD)(LONG_PTR)pState;
		else
		if (sType == _T("sty_box"))
			m_StyleBox = (bool)(LONG_PTR)pState;
		else
		if (sType == _T("image"))
		{
			CImage* img = (CImage*)pState;
			if (!img || img->IsNull()) return;
			if (!m_ItemList || m_ItemList.Empty()) return;
			DWORD count = m_ItemList.GetCount();
			LONG offset = img->GetWidth() / count;
			CRect rc_img(0, 0, offset, img->GetHeight());
			CRect rc_itm(0, 0, offset, img->GetHeight() / m_StatusCount);
			CPoint pt_off(offset, 0);
			if (m_StyleBox)
			{
				CImage tmp[9];
				for(DWORD i = 0; i < count; ++i)
				{
					m_ItemList[i]->SetWindowRect(rc_itm);
					tmp[4].Set(img->Clone(rc_img));
					m_ItemList[i]->SetState(_T("image"), tmp);
					rc_img.Offset(pt_off);
					rc_itm.Offset(pt_off);
				}
			}
			else
			{
				CImage tmp;
				for(DWORD i = 0; i < count; ++i)
				{
					m_ItemList[i]->SetWindowRect(rc_itm);
					tmp.Set(img->Clone(rc_img));
					m_ItemList[i]->SetState(_T("image"), &tmp);
					rc_img.Offset(pt_off);
					rc_itm.Offset(pt_off);
				}
			}
			EXP_BASE::SetState(sType, pState);
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiGroup, IGuiCtrlBase)

//////////////////////////////////////////////////////////////////

#endif/*__GuiGroup_hpp__*/