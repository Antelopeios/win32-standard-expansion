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
// GuiList - 列表控件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-03
// Version:	1.0.0001.2237
//
// History:
//	- 1.0.0000.1537(2012-02-29)	@ 开始构建GuiList
//	- 1.0.0001.2237(2012-03-03)	+ 为集合型控件添加insert,delete与clear属性
//////////////////////////////////////////////////////////////////

#ifndef __GuiList_hpp__
#define __GuiList_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG
	
//////////////////////////////////////////////////////////////////

class CGuiListItem : public CGuiButton /*CGuiList内部使用的列表项*/
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiListItem, CGuiButton)

public:
	CGuiListItem()
	{
		// 添加事件对象
		InsEvent(ExGui(_T("CGuiListItemEvent"), GetGC())); /*先让基类绘图*/
	}
};

//////////////////////////////////////////////////////////////////

class CGuiList : public CGuiPicture
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiList, CGuiPicture)

protected:
	items_t m_ItemList;
	LONG m_Space;	// 项间距
	CGuiStatic m_FocPic;

public:
	CGuiList()
		: m_Space(0)
	{
		// 添加事件对象
		InsEvent(ExGui(_T("CGuiListEvent"), GetGC())); /*先让基类绘图*/
		SetState(_T("color"), (void*)ExRGBA(EXP_CM, EXP_CM, EXP_CM, EXP_CM));
		AddComp(&m_FocPic);
	}
	~CGuiList()
	{
		DelComp(&m_FocPic, FALSE);
	}

public:
	BOOL Execute(const CString& key, const CString& val)
	{
		CArrayT<CString> sa;
		if (key.Left(4) == _T("foc_"))
		{
			CString type(key);
			type.TrimLeft(_T("foc_"));
			return m_FocPic.Execute(type, val);
		}
		else
		if (key == _T("space"))
			SetState(_T("space"), (void*)_ttol(val));
		else
			return EXP_BASE::Execute(key, val);
		return TRUE;
	}

	// 获得控件状态
	void* GetState(const CString& sType, void* pParam = NULL)
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
			return EXP_BASE::GetState(sType);
	}
	BOOL SetState(const CString& sType, void* pState, void* pParam = NULL)
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
			if (new_sta == NULL) return FALSE;
			for(items_t::iterator_t ite = m_ItemList.Head(); ite != m_ItemList.Tail(); ++ite)
			{
				IGuiCtl* item = *ite;
				if (!item) continue;
				items_t::iterator_t it = new_sta->Find(item);
				if (it == new_sta->Tail()) DelComp(item);
			}
			m_ItemList = *new_sta;
			for(items_t::iterator_t ite = m_ItemList.Head(); ite != m_ItemList.Tail(); ++ite)
			{
				IGuiCtl* item = *ite;
				if (!item) continue;
				AddComp(item);
			}
			SendMessage(WM_SIZE);
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("insert"))
		{
			if (!pState || !pParam) return FALSE;
			items_t::iterator_t ite = *(items_t::iterator_t*)pState;
			IGuiCtl* item = (IGuiCtl*)pParam;
			AddComp(item);
			m_ItemList.Add(item, ite);
			SendMessage(WM_SIZE);
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("delete"))
		{
			items_t::iterator_t ite = *(items_t::iterator_t*)pState;
			IGuiCtl* item = *ite;
			if (!item) return FALSE;
			DelComp(item);
			m_ItemList.Del(ite);
			SendMessage(WM_SIZE);
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("clear"))
		{
			for(items_t::iterator_t ite = m_ItemList.Head(); ite != m_ItemList.Tail(); ++ite)
			{
				IGuiCtl* item = *ite;
				if (!item) continue;
				DelComp(item);
			}
			m_ItemList.Clear();
			SendMessage(WM_SIZE);
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("space"))
			SET_STATE(LONG, m_Space)
		else
			return EXP_BASE::SetState(sType, pState);
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiListItem, CGuiButton)
EXP_IMPLEMENT_DYNCREATE_MULT(CGuiList, CGuiPicture)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiList_hpp__*/
