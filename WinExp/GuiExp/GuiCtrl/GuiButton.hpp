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
// GuiButton - 按钮控件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-09-23
// Version:	1.0.0008.1216
//
// History:
//	- 1.0.0001.2305(2011-05-25)	+ CGuiButton添加状态属性
//								= CGuiButton::m_Status初始化为nor
//								= CGuiButton不再托管内部的图片资源
//	- 1.0.0002.1310(2011-07-01)	+ CGuiButton添加locate属性,支持自定义文字显示位置
//	- 1.0.0003.1648(2011-07-06)	+ CGuiButton添加thr_sta属性,支持三态按钮
//	- 1.0.0004.1752(2011-07-11)	+ 添加CGuiPushBtn控件及其对应的事件响应
//	- 1.0.0005.1030(2011-08-12)	+ CGuiButton支持单态按钮
//	- 1.0.0006.1640(2011-08-15)	+ 在CGuiButton中实现icon相关属性
//								# 加上icon相关属性的初始化操作
//	- 1.0.0007.1513(2011-09-22)	= CGuiButton按钮状态改为8态
//	- 1.0.0008.1216(2011-09-23)	+ 添加shake_ico属性,支持点击时晃动图标及文字
//////////////////////////////////////////////////////////////////

#ifndef __GuiButton_hpp__
#define __GuiButton_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiButton : public IGuiCtrlBase
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiButton, IGuiCtrlBase)

public:
	/*
		正常; 浮动; 按下
	*/
	enum status_t {nor, ovr, hit};
	/*
		文字位置: 中; 上; 下; 左; 右
	*/
	enum locate_t {center, top, bottom, left, right};

protected:
	status_t m_Status;	// 按钮状态
	locate_t m_Locate;	// 文字位置
	LONG m_LocOff;		// 文字位置偏移(m_Locate == center 时无效)

	CImage m_Icon;
	BOOL m_bGlow;		// 是否绘制图标外发光
	LONG m_IcoOff;		// 图标位置偏移(m_Locate == center 时无效)
	int m_ShakeIco;		// 图标点击摇晃

	int m_ThreeSta;		// 是否是三态按钮
	pixel_t m_Color[8];
	CImage m_Image[9];	// 九宫格分割,每个小块保存8个状态
	CText m_Text[8];

public:
	CGuiButton()
		: m_Status(nor)
		, m_Locate(center)
		, m_LocOff(5)
		, m_bGlow(FALSE)
		, m_IcoOff(5)
		, m_ShakeIco(0)
		, m_ThreeSta(0)
	{
		ZeroMemory(m_Color, sizeof(m_Color));
		// 添加事件对象
		AddEvent((IGuiEvent*)ExGui(_T("CGuiButtonEvent"), GetGC()));
	}
	~CGuiButton()
	{}

public:
	void* Execute(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent)
	{
		style_t* sty = CGuiManagerT<style_t>::Get(xml.GetAttr(_T("style"), ite));
		if (sty)
		{
			if(!sty->font.Empty())
			{
				for(int i = 0; i < (int)min(_countof(m_Text), sty->font.GetCount()); ++i)
					m_Text[i] = *(sty->font[i]);
			}
			if(!sty->color.Empty())
			{
				for(int i = 0; i < (int)min(_countof(m_Color), sty->color.GetCount()); ++i)
					m_Color[i] = sty->color[i];
			}
			if(!sty->image.Empty())
			{
				for(int i = 0; i < (int)min(_countof(m_Image), sty->image.GetCount()); ++i)
					m_Image[i] = *(sty->image[i]);
			}
		}
		m_Text[0].SetString(xml.GetAttr(_T("text"), ite));
		for(int i = 1; i < _countof(m_Text); ++i)
			m_Text[i].SetString(m_Text[0]);
		SetWindowRect(StringToRect(xml.GetAttr(_T("rect"), ite)));
		return this;
	}

	// 获得控件状态
	void* GetState(const CString& sType)
	{
		if (sType == _T("status"))
			return (void*)m_Status;
		else
		if (sType == _T("locate"))
			return (void*)m_Locate;
		else
		if (sType == _T("loc_off"))
			return (void*)m_LocOff;
		else
		if (sType == _T("shake_ico"))
			return (void*)m_ShakeIco;
		else
		if (sType == _T("thr_sta"))
			return (void*)m_ThreeSta;
		else
		if (sType == _T("color"))
			return (void*)m_Color;
		else
		if (sType == _T("image"))
			return (void*)m_Image;
		else
		if (sType == _T("text"))
			return (void*)m_Text;
		else
		if (sType == _T("icon"))
			return (void*)(&m_Icon);
		else
		if (sType == _T("ico_off"))
			return (void*)m_IcoOff;
		else
		if (sType == _T("glow"))
			return (void*)m_bGlow;
		else
			return EXP_BASE::GetState(sType);
	}
	BOOL SetState(const CString& sType, void* pState)
	{
		if (sType == _T("status"))
		{
			status_t old_sta = m_Status;
			m_Status = (status_t)(LONG_PTR)pState;
			if (old_sta != m_Status)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("locate"))
		{
			locate_t old_sta = m_Locate;
			m_Locate = (locate_t)(LONG_PTR)pState;
			if (old_sta != m_Locate)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("loc_off"))
		{
			LONG old_sta = m_LocOff;
			m_LocOff = (LONG)(LONG_PTR)pState;
			if (old_sta != m_LocOff)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("shake_ico"))
		{
			int old_sta = m_ShakeIco;
			m_ShakeIco = (int)(LONG_PTR)pState;
			if (old_sta != m_ShakeIco)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("thr_sta"))
		{
			int old_sta = m_ThreeSta;
			m_ThreeSta = (int)(LONG_PTR)pState;
			if (old_sta != m_ThreeSta)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("color"))
		{
			for(int i = 0; i < _countof(m_Color); ++i)
				m_Color[i] = *((pixel_t*)pState + i);
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("image"))
		{
			for(int i = 0; i < _countof(m_Image); ++i)
				m_Image[i] = *((CImage*)pState + i);
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("text"))
		{
			for(int i = 0; i < _countof(m_Text); ++i)
				m_Text[i] = *((CText*)pState + i);
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("icon"))
		{
			m_Icon = *((CImage*)pState);
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("ico_off"))
		{
			LONG old_sta = m_IcoOff;
			m_IcoOff = (LONG)(LONG_PTR)pState;
			if (old_sta != m_IcoOff)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("glow"))
		{
			BOOL old_sta = m_bGlow;
			m_bGlow = (BOOL)(LONG_PTR)pState;
			if (old_sta != m_bGlow)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		return FALSE;
	}
};

//////////////////////////////////////////////////////////////////

class CGuiPushBtn : public IGuiCtrlBase
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiPushBtn, IGuiCtrlBase)

public:
	/*
		正常; 浮动; 按下; Push
	*/
	enum status_t {nor, ovr, hit, psh};
	/*
		文字位置: 中; 上; 下; 左; 右
	*/
	enum locate_t {center, top, bottom, left, right};

protected:
	status_t m_Status;	// 按钮状态
	locate_t m_Locate;	// 文字位置
	LONG m_LocOff;		// 文字位置偏移(m_Locate == center 时无效)

	pixel_t m_Color[4];
	CImage m_Image;		// 保存4个状态
	CText m_Text[4];

public:
	CGuiPushBtn()
		: m_Status(nor)
		, m_Locate(center)
		, m_LocOff(5)
	{
		ZeroMemory(m_Color, sizeof(m_Color));
		// 添加事件对象
		AddEvent((IGuiEvent*)ExGui(_T("CGuiPushBtnEvent"), GetGC()));
	}
	~CGuiPushBtn()
	{}

public:
	// 获得控件状态
	void* GetState(const CString& sType)
	{
		if (sType == _T("status"))
			return (void*)m_Status;
		else
		if (sType == _T("locate"))
			return (void*)m_Locate;
		else
		if (sType == _T("loc_off"))
			return (void*)m_LocOff;
		else
		if (sType == _T("color"))
			return (void*)m_Color;
		else
		if (sType == _T("image"))
			return (void*)(&m_Image);
		else
		if (sType == _T("text"))
			return (void*)m_Text;
		else
			return EXP_BASE::GetState(sType);
	}
	BOOL SetState(const CString& sType, void* pState)
	{
		if (sType == _T("status"))
		{
			status_t old_sta = m_Status;
			m_Status = (status_t)(LONG_PTR)pState;
			if (old_sta != m_Status)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("locate"))
		{
			locate_t old_sta = m_Locate;
			m_Locate = (locate_t)(LONG_PTR)pState;
			if (old_sta != m_Locate)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("loc_off"))
		{
			LONG old_sta = m_LocOff;
			m_LocOff = (LONG)(LONG_PTR)pState;
			if (old_sta != m_LocOff)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("color"))
		{
			for(int i = 0; i < _countof(m_Color); ++i)
				m_Color[i] = *((pixel_t*)pState + i);
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("image"))
		{
			m_Image = *((CImage*)pState);
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("text"))
		{
			for(int i = 0; i < _countof(m_Text); ++i)
				m_Text[i] = *((CText*)pState + i);
			return EXP_BASE::SetState(sType, pState);
		}
		return FALSE;
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiButton, IGuiCtrlBase)
EXP_IMPLEMENT_DYNCREATE_MULT(CGuiPushBtn, IGuiCtrlBase)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiButton_hpp__*/