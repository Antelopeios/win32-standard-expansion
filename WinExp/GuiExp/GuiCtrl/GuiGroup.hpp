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
// GuiGroup - 组框控件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-03
// Version:	1.0.0004.2237
//
// History:
//	- 1.0.0000.0940(2011-07-07)	@ 开始构建GuiGroup
//	- 1.0.0001.1752(2011-07-11)	^ 优化GuiGroup的外部调用属性
//	- 1.0.0002.1532(2011-07-21)	= 将CGuiGroup内部items_t结构由指针改为对象,减轻调用复杂度
//	- 1.0.0003.1517(2011-09-28)	# 修正当外部销毁控件对象时,GuiGroup因内部对象析构顺序问题导致的内存访问异常
//	- 1.0.0004.2237(2012-03-03)	+ 为集合型控件添加insert,delete与clear属性
//////////////////////////////////////////////////////////////////

#ifndef __GuiGroup_hpp__
#define __GuiGroup_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG
	
//////////////////////////////////////////////////////////////////

class _grp_items : public IItemSetT<IGuiCtl::items_t>
{
	EXP_DECLARE_DYNCREATE_CLS(_grp_items, IGuiSet)

protected:
	DWORD m_StatusCount;
	CImage* m_ImgCac;
	CGC gc;

public:
	_grp_items()
		: m_StatusCount(3)
		, m_ImgCac(NULL)
	{}

public:
	BOOL Key(const CString& key)
	{
		if (IItemSetT<IGuiCtl::items_t>::Key(key))
			return TRUE;
		else
		if (key == _T("image") || 
			key == _T("sta_cnt"))
		{
			m_Key = key;
			return TRUE;
		}
		else
		{
			m_Key = _T("");
			return FALSE;
		}
	}
	BOOL Exc(const CString& val)
	{
		if (m_Key == _T("image"))
			Set(ExGet<CImage>(val));
		else
		if (m_Key == _T("sta_cnt"))
			Set((void*)_ttol(val));
		Ctl()->UpdateState();
		return TRUE;
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		if (m_Key == _T("items") || 
			m_Key == _T("insert") || 
			m_Key == _T("delete") || 
			m_Key == _T("clear"))
		{
			IItemSetT<IGuiCtl::items_t>::Set(sta, par);
			gc.Clear();
			if (m_Key != _T("clear"))
			{
				m_Key = _T("image");
				Set(m_ImgCac);
			}
		}
		else
		if (m_Key == _T("image"))
		{
			CImage* img = (CImage*)sta;
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
			for(items_t::iterator_t ite = m_ItemList.Head(); ite != m_ItemList.Tail(); ++ite)
			{
				IGuiCtl* item = *ite;
				CImage* tmp = gcnew(gc, CImage, 9);
				item->SetWindowRect(rc_itm);
				tmp[4].Set(img->Clone(rc_img));
				item->SetState(_T("image"), tmp);
				rc_img.Offset(pt_off);
				rc_itm.Offset(pt_off);
			}
			m_ImgCac = NULL;
		}
		else
		if (m_Key == _T("sta_cnt"))
			m_StatusCount = (DWORD)(LONG_PTR)sta;
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_grp_items, IGuiSet)

//////////////////////////////////////////////////////////////////

class CGuiGroup : public IGuiCtl
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiGroup, IGuiCtl)

public:
	CGuiGroup()
	{
		// 添加逻辑对象
		AddSet(_T("_grp_items"));
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiGroup, IGuiCtl)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiGroup_hpp__*/