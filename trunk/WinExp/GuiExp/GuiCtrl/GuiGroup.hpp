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
// Date:	2011-07-07
// Version:	1.0.0000.0940
//
// History:
//	- 1.0.0000.0940(2011-07-07)	@ 开始构建GuiGroup
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
	items_t* m_ItemList;

public:
	CGuiGroup()
		: m_ItemList(NULL)
	{}

public:
	// 获得控件状态
	state_t* GetState(const CString& sType, CGC* pGC = NULL)
	{
		state_t* state = EXP_BASE::GetState(sType, pGC);
		if (state)
		{
			if (state->sta_typ == _T("items"))
				state->sta_arr.Add(m_ItemList);
		}
		return state;
	}
	void SetState(const CString& sType, void* pState)
	{
		if (sType == _T("items"))
		{
			items_t* old_sta = m_ItemList;
			m_ItemList = (items_t*)pState;
			if (old_sta != m_ItemList)
				EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("image"))
		{
			CImage* img = (CImage*)pState;
			if (!img || img->IsNull()) return;
			CRect rect;
			GetWindowRect(rect);
			rect.pt2.x = rect.pt1.x + img->GetWidth();
			rect.pt2.y = rect.pt1.y + img->GetHeight();
			SetWindowRect(rect);
			if (!m_ItemList || m_ItemList->Empty()) return;
			DWORD count = m_ItemList->GetCount();
			LONG offset = img->GetWidth() / count;
			CRect rc_img(0, 0, offset, img->GetHeight());
			CImage tmp[9];
			for(DWORD i = 0; i < count; ++i)
			{
				m_ItemList->GetAt(i)->SetWindowRect(rc_img);
				tmp[4].Set(img->Clone(rc_img));
				m_ItemList->GetAt(i)->SetState(_T("image"), tmp);
				rc_img.Offset(CPoint(offset, 0));
			}
			EXP_BASE::SetState(sType, pState);
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiGroup, IGuiCtrlBase)

//////////////////////////////////////////////////////////////////

#endif/*__GuiGroup_hpp__*/