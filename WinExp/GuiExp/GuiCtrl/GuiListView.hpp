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
// GuiListView - 列表视图控件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-01
// Version:	1.0.0011.1239
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
//	- 1.0.0008.2317(2011-08-30)	+ GuiListView添加align_top属性,支持列表项上对齐/下对齐调整
//	- 1.0.0009.1517(2011-09-28)	# 修正当外部销毁控件对象时,GuiListView因内部对象析构顺序问题导致的内存访问异常
//	- 1.0.0010.1740(2012-02-29)	- 将有关滚动条控制的相关属性从GuiListView中移除,统一交由底层完成
//	- 1.0.0011.1239(2012-03-01)	= 将基类由CGuiPicture调整为CGuiList,部分属性将在CGuiList中实现
//////////////////////////////////////////////////////////////////

#ifndef __GuiListView_hpp__
#define __GuiListView_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiLVItem : public CGuiListItem /*CGuiListView内部使用的列表项*/
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiLVItem, CGuiListItem)

public:
	CGuiLVItem()
	{
		// 添加事件对象
		PopEvent(FALSE);
		InsEvent(_T("CGuiLVItemEvent")); /*先让基类绘图*/
	}
};

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiLVItem, CGuiListItem)

//////////////////////////////////////////////////////////////////

class CGuiListView : public CGuiList
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiListView, CGuiList)

protected:
	BOOL m_AlignTop;

public:
	CGuiListView()
		: m_AlignTop(TRUE)
	{
		// 添加事件对象
		PopEvent(FALSE);
		InsEvent(_T("CGuiLVEvent")); /*先让基类绘图*/
	}
	~CGuiListView()
	{
	}

public:
	BOOL Execute(const CString& key, const CString& val)
	{
		if (key == _T("align_top"))
		{
			CString temp(val);
			temp.Lower();
			if (temp == _T("false"))
				SetState(_T("align_top"), (void*)FALSE);
			else
			if (temp == _T("true"))
				SetState(_T("align_top"), (void*)TRUE);
		}
		else
			return EXP_BASE::Execute(key, val);
		return IGuiCtl::Execute(key, val);
	}

	// 获得控件状态
	void* GetState(const CString& sType, void* pParam = NULL)
	{
		if (sType == _T("align_top"))
			return (void*)m_AlignTop;
		else
			return EXP_BASE::GetState(sType, pParam);
	}
	BOOL SetState(const CString& sType, void* pState, void* pParam = NULL)
	{
		CString type(sType);
		if (sType == _T("align_top"))
			SET_STATE(BOOL, m_AlignTop, SendMessage(WM_SIZE))
		else
			return EXP_BASE::SetState(sType, pState, pParam);
	}
};

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiListView, CGuiList)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiListView_hpp__*/