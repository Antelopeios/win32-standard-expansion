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
// GuiTree - 树控件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-27
// Version:	1.0.0000.1650
//
// History:
//	- 1.0.0000.1650(2012-03-27)	@ 开始构建GuiTree
//////////////////////////////////////////////////////////////////

#ifndef __GuiTree_h__
#define __GuiTree_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////
// CGuiTreeItem
//////////////////////////////////////////////////////////////////

// 树结点是否展开
class _tre_itm_expand : public ICtrlSetT<BOOL>
{
	EXP_DECLARE_DYNCREATE_CLS(_tre_itm_expand, IGuiSet)

public:
	CString GetKey() const { return _T("expand"); }
};

EXP_IMPLEMENT_DYNCREATE_CLS(_tre_itm_expand, IGuiSet)

//////////////////////////////////////////////////////////////////

class CGuiTreeItem : public IGuiCtrlBase /*CGuiTree内部使用的列表项*/
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiTreeItem, IGuiCtrlBase)

protected:
	CGuiPushBtn m_Ind;
	CGuiSimpBtn m_Itm;

public:
	CGuiTreeItem()
	{
		// 添加逻辑对象
		AddSet(_T("_tre_itm_expand"));
		// 添加控件对象
		InsComp(&m_Ind);
		InsComp(&m_Itm);
	}
	~CGuiTreeItem()
	{
		DelComp(&m_Ind, FALSE);
		DelComp(&m_Itm, FALSE);
	}

public:
	BOOL Execute(const CString& key, const CString& val)
	{
		if (key.Left(4) == _T("ind_"))
		{
			CString type(key);
			type.TrimLeft(_T("ind_"));
			m_Ind.Execute(type, val);
		}
		else
		if (key.Left(4) == _T("itm_"))
		{
			CString type(key);
			type.TrimLeft(_T("itm_"));
			m_Itm.Execute(type, val);
		}
		else
			return EXP_BASE::Execute(key, val);
		return IGuiCtrlBase::Execute(key, val);
	}
	void* GetState(const CString& sType, void* pParam = NULL)
	{
		if (sType.Left(4) == _T("ind_"))
		{
			CString type(sType);
			type.TrimLeft(_T("ind_"));
			return m_Ind.GetState(type);
		}
		else
		if (sType.Left(4) == _T("itm_"))
		{
			CString type(sType);
			type.TrimLeft(_T("itm_"));
			return m_Itm.GetState(type);
		}
		else
			return EXP_BASE::GetState(sType);
	}
	BOOL SetState(const CString& sType, void* pState, void* pParam = NULL)
	{
		if (sType.Left(4) == _T("ind_"))
		{
			CString type(sType);
			type.TrimLeft(_T("ind_"));
			return m_Ind.SetState(type, pState);
		}
		else
		if (sType.Left(4) == _T("itm_"))
		{
			CString type(sType);
			type.TrimLeft(_T("itm_"));
			return m_Itm.SetState(type, pState);
		}
		else
			return EXP_BASE::SetState(sType, pState);
	}
};

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiTreeItem, IGuiCtrlBase)

//////////////////////////////////////////////////////////////////
// CGuiTree
//////////////////////////////////////////////////////////////////

class _tre_items : public IItemSetT<IGuiCtl::itree_t>
{
	EXP_DECLARE_DYNCREATE_CLS(_tre_items, IGuiSet)
};

EXP_IMPLEMENT_DYNCREATE_CLS(_tre_items, IGuiSet)

//////////////////////////////////////////////////////////////////

class CGuiTree : public CGuiList
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiTree, CGuiList)

protected:
	CGuiStatic m_FocPic;

public:
	CGuiTree()
	{
		// 添加逻辑对象
		AddSet(_T("_tre_items"));
		// 添加事件对象
		PopEvent(FALSE);
		InsEvent(_T("CGuiTreeEvent")); /*先让基类绘图*/
		// 设置默认属性
		SetState(_T("color"), (void*)ExRGBA(EXP_CM, EXP_CM, EXP_CM, EXP_CM));
		AddComp(&m_FocPic);
	}
	~CGuiTree()
	{
		DelComp(&m_FocPic, FALSE);
	}

public:
	BOOL Execute(const CString& key, const CString& val)
	{
		if (key.Left(4) == _T("foc_"))
		{
			CString type(key);
			type.TrimLeft(_T("foc_"));
			return m_FocPic.Execute(type, val);
		}
		else
			return EXP_BASE::Execute(key, val);
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
			return EXP_BASE::SetState(sType, pState);
	}
};

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiTree, CGuiList)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiTree_h__*/
