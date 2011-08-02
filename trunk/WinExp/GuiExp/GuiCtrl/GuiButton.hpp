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
// Date:	2011-07-11
// Version:	1.0.0004.1752
//
// History:
//	- 1.0.0001.2305(2011-05-25)	+ CGuiButton添加状态属性
//								= CGuiButton::m_Status初始化为nor
//								= CGuiButton不再托管内部的图片资源
//	- 1.0.0002.1310(2011-07-01)	+ CGuiButton添加locate属性,支持自定义文字显示位置
//	- 1.0.0003.1648(2011-07-06)	+ CGuiButton添加thr_sta属性,支持三态按钮
//	- 1.0.0004.1752(2011-07-11)	+ 添加CGuiPushBtn控件及其对应的事件响应
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
		正常; 浮动; 按下; 焦点; 禁止
	*/
	enum status_t {nor, ovr, hit/*, foc, dis*/};
	/*
		文字位置: 中; 上; 下; 左; 右
	*/
	enum locate_t {center, top, bottom, left, right};

protected:
	status_t m_Status;	// 按钮状态
	locate_t m_Locate;	// 文字位置
	LONG m_LocOff;		// 文字位置偏移(m_Locate == center 时无效)

	bool m_ThreeSta;	// 是否是三态按钮
	pixel_t m_Color[5];
	CImage m_Image[9];	// 九宫格分割,每个小块保存5个状态
	CText m_Text[5];

public:
	CGuiButton()
		: m_Status(nor)
		, m_Locate(center)
		, m_LocOff(5)
		, m_ThreeSta(false)
	{
		ZeroMemory(m_Color, sizeof(m_Color));
		// 添加事件对象
		AddEvent((IGuiEvent*)ExGui(_T("CGuiButtonEvent"), GetGC()));
	}
	~CGuiButton()
	{}

public:
	// 获得控件状态
	state_t* GetState(const CString& sType, CGC* pGC = NULL)
	{
		state_t* state = EXP_BASE::GetState(sType, pGC);
		if (state)
		{
			if (state->sta_typ == _T("status"))
				state->sta_arr.Add((void*)m_Status);
			else
			if (state->sta_typ == _T("locate"))
				state->sta_arr.Add((void*)m_Locate);
			else
			if (state->sta_typ == _T("loc_off"))
				state->sta_arr.Add((void*)m_LocOff);
			else
			if (state->sta_typ == _T("thr_sta"))
				state->sta_arr.Add((void*)m_ThreeSta);
			else
			if (state->sta_typ == _T("color"))
				for(int i = 0; i < _countof(m_Color); ++i)
					state->sta_arr.Add(m_Color + i);
			else
			if (state->sta_typ == _T("image"))
				for(int i = 0; i < _countof(m_Image); ++i)
					state->sta_arr.Add(m_Image + i);
			else
			if (state->sta_typ == _T("text"))
				for(int i = 0; i < _countof(m_Text); ++i)
					state->sta_arr.Add(m_Text + i);
		}
		return state;
	}
	bool SetState(const CString& sType, void* pState)
	{
		if (sType == _T("status"))
		{
			status_t old_sta = m_Status;
			m_Status = (status_t)(LONG_PTR)pState;
			if (old_sta != m_Status)
				return EXP_BASE::SetState(sType, pState);
			else
				return true;
		}
		else
		if (sType == _T("locate"))
		{
			locate_t old_sta = m_Locate;
			m_Locate = (locate_t)(LONG_PTR)pState;
			if (old_sta != m_Locate)
				return EXP_BASE::SetState(sType, pState);
			else
				return true;
		}
		else
		if (sType == _T("loc_off"))
		{
			LONG old_sta = m_LocOff;
			m_LocOff = (LONG)(LONG_PTR)pState;
			if (old_sta != m_LocOff)
				return EXP_BASE::SetState(sType, pState);
			else
				return true;
		}
		else
		if (sType == _T("thr_sta"))
		{
			bool old_sta = m_ThreeSta;
			m_ThreeSta = (bool)(LONG_PTR)pState;
			if (old_sta != m_ThreeSta)
				return EXP_BASE::SetState(sType, pState);
			else
				return true;
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
		return false;
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
	state_t* GetState(const CString& sType, CGC* pGC = NULL)
	{
		state_t* state = EXP_BASE::GetState(sType, pGC);
		if (state)
		{
			if (state->sta_typ == _T("status"))
				state->sta_arr.Add((void*)m_Status);
			else
			if (state->sta_typ == _T("locate"))
				state->sta_arr.Add((void*)m_Locate);
			else
			if (state->sta_typ == _T("loc_off"))
				state->sta_arr.Add((void*)m_LocOff);
			else
			if (state->sta_typ == _T("color"))
				for(int i = 0; i < _countof(m_Color); ++i)
					state->sta_arr.Add(m_Color + i);
			else
			if (state->sta_typ == _T("image"))
				state->sta_arr.Add(&m_Image);
			else
			if (state->sta_typ == _T("text"))
				for(int i = 0; i < _countof(m_Text); ++i)
					state->sta_arr.Add(m_Text + i);
		}
		return state;
	}
	bool SetState(const CString& sType, void* pState)
	{
		if (sType == _T("status"))
		{
			status_t old_sta = m_Status;
			m_Status = (status_t)(LONG_PTR)pState;
			if (old_sta != m_Status)
				return EXP_BASE::SetState(sType, pState);
			else
				return true;
		}
		else
		if (sType == _T("locate"))
		{
			locate_t old_sta = m_Locate;
			m_Locate = (locate_t)(LONG_PTR)pState;
			if (old_sta != m_Locate)
				return EXP_BASE::SetState(sType, pState);
			else
				return true;
		}
		else
		if (sType == _T("loc_off"))
		{
			LONG old_sta = m_LocOff;
			m_LocOff = (LONG)(LONG_PTR)pState;
			if (old_sta != m_LocOff)
				return EXP_BASE::SetState(sType, pState);
			else
				return true;
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
		return false;
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiButton, IGuiCtrlBase)
EXP_IMPLEMENT_DYNCREATE_MULT(CGuiPushBtn, IGuiCtrlBase)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiButton_hpp__*/