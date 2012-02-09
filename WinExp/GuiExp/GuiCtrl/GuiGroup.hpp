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
// Date:	2011-09-28
// Version:	1.0.0003.1517
//
// History:
//	- 1.0.0000.0940(2011-07-07)	@ 开始构建GuiGroup
//	- 1.0.0001.1752(2011-07-11)	^ 优化GuiGroup的外部调用属性
//	- 1.0.0002.1532(2011-07-21)	= 将CGuiGroup内部items_t结构由指针改为对象,减轻调用复杂度
//	- 1.0.0003.1517(2011-09-28)	# 修正当外部销毁控件对象时,GuiGroup因内部对象析构顺序问题导致的内存访问异常
//////////////////////////////////////////////////////////////////

#ifndef __GuiGroup_hpp__
#define __GuiGroup_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiGroup : public IGuiCtrlBase
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiGroup, IGuiCtrlBase)

protected:
	items_t m_ItemList;
	DWORD m_StatusCount;
	BOOL m_StyleBox;
	CImage* m_ImgCac;

	CGC gc;

public:
	CGuiGroup()
		: m_ItemList(NULL)
		, m_StatusCount(3)
		, m_StyleBox(TRUE)
		, m_ImgCac(NULL)
	{}
	~CGuiGroup()
	{
	}

public:
	BOOL Execute(const CString& key, const CString& val)
	{
		if (key == _T("image"))
			SetState(_T("image"), CGuiManagerT<CImage>::Get(val));
		else
		if (key == _T("sta_cnt"))
			SetState(_T("sta_cnt"), (void*)_ttol(val));
		else
		if (key == _T("sty_box"))
		{
			CString temp(val);
			temp.Lower();
			if (temp == _T("false"))
				SetState(_T("sty_box"), (void*)FALSE);
			else
			if (temp == _T("true"))
				SetState(_T("sty_box"), (void*)TRUE);
		}
		return TRUE;
	}

	// 获得控件状态
	void* GetState(const CString& sType)
	{
		if (sType == _T("items"))
			return (void*)(&m_ItemList);
		else
			return EXP_BASE::GetState(sType);
	}
	BOOL SetState(const CString& sType, void* pState)
	{
		if (sType == _T("items"))
		{
			items_t* new_sta = (items_t*)pState;
			if (new_sta == NULL) return FALSE;
			for(items_t::iterator_t ite = m_ItemList.Head(); ite != m_ItemList.Tail(); ++ite)
			{
				IGuiCtrl* item = *ite;
				if (!item) continue;
				items_t::iterator_t it = new_sta->Find(item);
				if (it == new_sta->Tail()) DelComp(item);
			}
			m_ItemList = *(items_t*)pState;
			for(items_t::iterator_t ite = m_ItemList.Head(); ite != m_ItemList.Tail(); ++ite)
			{
				IGuiCtrl* item = *ite;
				if (!item) continue;
				AddComp(item);
			}
			gc.Clear();
			SetState(_T("image"), m_ImgCac);
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("sta_cnt"))
		{
			m_StatusCount = (DWORD)(LONG_PTR)pState;
			return TRUE;
		}
		else
		if (sType == _T("sty_box"))
		{
			m_StyleBox = (BOOL)(LONG_PTR)pState;
			return TRUE;
		}
		else
		if (sType == _T("image"))
		{
			CImage* img = (CImage*)pState;
			if (!img || img->IsNull()) return FALSE;
			if (m_ItemList.Empty())
			{
				m_ImgCac = img;
				return TRUE;
			}
			DWORD count = m_ItemList.GetCount();
			LONG offset = img->GetWidth() / count;
			CRect rc_img(0, 0, offset, img->GetHeight());
			CRect rc_itm(0, 0, offset, img->GetHeight() / m_StatusCount);
			CPoint pt_off(offset, 0);
			int c = m_StyleBox ? 9 : 1, 
				n = m_StyleBox ? 4 : 0;
			for(DWORD i = 0; i < count; ++i)
			{
				CImage* tmp = gcnew(gc, CImage, c);
				m_ItemList[i]->SetWindowRect(rc_itm);
				tmp[n].Set(img->Clone(rc_img));
				m_ItemList[i]->SetState(_T("image"), tmp);
				rc_img.Offset(pt_off);
				rc_itm.Offset(pt_off);
			}
			m_ImgCac = NULL;
			return EXP_BASE::SetState(sType, pState);
		}
		return FALSE;
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiGroup, IGuiCtrlBase)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiGroup_hpp__*/