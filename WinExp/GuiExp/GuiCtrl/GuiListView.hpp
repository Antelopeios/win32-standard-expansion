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
// GuiListView - 列表视图控件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-08-26
// Version:	1.0.0007.1623
//
// History:
//	- 1.0.0000.1543(2011-06-30)	@ 开始构建GuiListView
//	- 1.0.0001.1532(2011-07-21)	= 将CGuiListView内部items_t结构由指针改为对象,减轻调用复杂度
//	- 1.0.0002.1744(2011-08-05)	+ 添加GuiListView焦点时默认列表项的背景图
//	- 1.0.0003.1508(2011-08-10)	= 将默认列表项的背景图由图片改为Pic控件
//	- 1.0.0004.1730(2011-08-11)	= 列表项的图标偏移采用单独的属性(ico_off)控制
//	- 1.0.0005.1030(2011-08-12)	= 将默认列表项的背景图由图片改为单态按钮控件,方便支持九宫格式的焦点图片
//	- 1.0.0006.1606(2011-08-15)	^ 将CGuiLVItem的具体属性转移到CGuiButton中实现,仅保留特殊的事件响应
//	- 1.0.0007.1623(2011-08-26)	# 修正当GuiListView在运行过程中修改列表项时,不会自动格式化列表项位置的问题
//////////////////////////////////////////////////////////////////

#ifndef __GuiListView_hpp__
#define __GuiListView_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiLVItem : public CGuiButton /*CGuiListView内部使用的列表项*/
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiLVItem, CGuiButton)

public:
	CGuiLVItem()
	{
		// 添加事件对象
		InsEvent((IGuiEvent*)ExGui(_T("CGuiLVItemEvent"), GetGC())); /*先让基类绘图*/
	}
};

//////////////////////////////////////////////////////////////////

class CGuiListView : public CGuiPicture
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiListView, CGuiPicture)

public:
	typedef CListT<IGuiCtrl*> items_t;

protected:
	items_t m_ItemList;
	LONG m_Space;	// 项间距
	LONG m_AllLine, m_FraLine;
	CGuiButton m_FocPic;

public:
	CGuiListView()
		: m_ItemList(NULL)
		, m_Space(0)
		, m_AllLine(0)
		, m_FraLine(0)
	{
		// 添加事件对象
		InsEvent((IGuiEvent*)ExGui(_T("CGuiLVEvent"), GetGC())); /*先让基类绘图*/
		SetState(_T("color"), (void*)ExRGBA(EXP_CM, EXP_CM, EXP_CM, EXP_CM));
		m_FocPic.SetState(_T("thr_sta"), (void*)-1); /*单态按钮*/
		AddComp(&m_FocPic);
	}

public:
	// 获得控件状态
	void* GetState(const CString& sType)
	{
		if (sType.Left(4) == _T("foc_"))
		{
			CString type(sType);
			type.TrimLeft(_T("foc_"));
			return m_FocPic.GetState(type);
		}
		else
		if (sType == _T("foc"))
			return (void*)(&m_FocPic);
		else
		if (sType == _T("items"))
			return (void*)(&m_ItemList);
		else
		if (sType == _T("space"))
			return (void*)m_Space;
		else
		if (sType == _T("all_line"))
			return (void*)m_AllLine;
		else
		if (sType == _T("fra_line"))
			return (void*)m_FraLine;
		else
			return EXP_BASE::GetState(sType);
	}
	bool SetState(const CString& sType, void* pState)
	{
		CString type(sType);
		if (type.Left(4) == _T("foc_"))
		{
			type.TrimLeft(_T("foc_"));
			return m_FocPic.SetState(type, pState);
		}
		else
		if (sType == _T("items"))
		{
			items_t* new_sta = (items_t*)pState;
			if (new_sta == NULL) return false;
			for(items_t::iterator_t ite = m_ItemList.Head(); ite != m_ItemList.Tail(); ++ite)
			{
				IGuiCtrl* item = *ite;
				if (!item) continue;
				items_t::iterator_t it = new_sta->Find(item);
				if (it == new_sta->Tail()) DelComp(item);
			}
			m_ItemList = *new_sta;
			for(items_t::iterator_t ite = m_ItemList.Head(); ite != m_ItemList.Tail(); ++ite)
			{
				IGuiCtrl* item = *ite;
				if (!item) continue;
				AddComp(item);
			}
			Send(ExDynCast<IGuiObject>(this), WM_SIZE);
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("space"))
		{
			LONG old_sta = m_Space;
			m_Space = (LONG)(LONG_PTR)pState;
			if (old_sta != m_Space)
				return IGuiCtrlBase::SetState(sType, pState);
			else
				return true;
		}
		else
		if (sType == _T("all_line"))
		{
			LONG old_sta = m_AllLine;
			m_AllLine = (LONG)(LONG_PTR)pState;
			if (old_sta != m_AllLine && GetScroll())
				GetScroll()->SetState(_T("sli_all"), (void*)m_AllLine);
			return true;
		}
		else
		if (sType == _T("fra_line"))
		{
			LONG old_sta = m_FraLine;
			m_FraLine = (LONG)(LONG_PTR)pState;
			if (old_sta != m_FraLine && GetScroll())
				GetScroll()->SetState(_T("sli_fra"), (void*)m_FraLine);
			return true;
		}
		else
			return EXP_BASE::SetState(sType, pState);
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiLVItem, CGuiButton)
EXP_IMPLEMENT_DYNCREATE_MULT(CGuiListView, CGuiPicture)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiListView_hpp__*/