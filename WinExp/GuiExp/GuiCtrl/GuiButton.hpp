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
// GuiButton - 按钮控件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-11
// Version:	1.0.0011.1931
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
//	- 1.0.0009.2025(2012-03-01) + GuiButton支持通过字符串匹配对应的locate属性
//	- 1.0.0010.1443(2012-03-08) ^ 将CGuiPushBtn继承于GuiButton,其中的特殊处理一并放入GuiButton中
//	- 1.0.0011.1931(2012-03-11) ^ 优化GuiButton的内部设定,Execute的某些设置项支持直接用中文定义整型属性
//								+ GuiButton支持勾选按钮
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
		正常; 浮动; 按下; push
	*/
	enum status_t {nor, ovr, prs, psh};
	/*
		按钮类型
	*/
	enum styles_t
	{
		nor_btn,	// 普通按钮
		psh_btn,	// 下压按钮
		check_box	// 勾选按钮
	};
	/*
		文字位置: 中; 上; 下; 左; 右
	*/
	enum locate_t {center, top, bottom, left, right};

protected:
	status_t m_Status;	// 按钮状态
	locate_t m_Locate;	// 文字位置
	LONG m_LocOff;		// 文字位置偏移

	CImage* m_Icon;
	BOOL m_bGlow;		// 是否绘制图标外发光
	LONG m_IcoOff;		// 图标位置偏移
	int m_ShakeIco;		// 图标点击摇晃

	UINT m_ThreeSta;	// 按钮状态选项
	styles_t m_BtnSty;	// 按钮状态
	pixel_t m_Color[10];
	CImage* m_Image[9];	// 九宫格分割,每个小块保存所有状态
	CText* m_Text[10];
	CString m_Str;

	int m_CheckSta;		// 勾选状态

public:
	CGuiButton()
		: m_Status(nor)
		, m_Locate(center)
		, m_LocOff(0)
		, m_bGlow(FALSE)
		, m_IcoOff(0)
		, m_ShakeIco(0)
		, m_ThreeSta(0)
		, m_BtnSty(nor_btn)
		, m_Icon(NULL)
		, m_CheckSta(0)
	{
		ZeroMemory(m_Color, sizeof(m_Color));
		ZeroMemory(m_Image, sizeof(m_Image));
		ZeroMemory(m_Text, sizeof(m_Text));
		// 添加事件对象
		AddEvent(ExGui(_T("CGuiButtonEvent"), GetGC()));
	}
	~CGuiButton()
	{}

public:
	BOOL Execute(const CString& key, const CString& val)
	{
		CArrayT<CString> sa;
		if (key == _T("style"))
		{
			style_t* sty = ExGet<style_t>(val);
			if (sty)
			{
				if(!sty->font.Empty())
					for(int i = 0; i < (int)min(_countof(m_Text), sty->font.GetCount()); ++i)
						m_Text[i] = sty->font[i];
				if(!sty->color.Empty())
					SetState(_T("color"), (void*)(pixel_t*)sty->color);
				if(!sty->image.Empty())
					for(int i = 0; i < (int)min(_countof(m_Image), sty->image.GetCount()); ++i)
						m_Image[i] = sty->image[i];
			}
		}
		else
		if (key == _T("font"))
		{
			ExStringToArray(val, sa);
			for(int i = 0; i < (int)min(_countof(m_Text), sa.GetCount()); ++i)
				m_Text[i] = ExGet<CText>(sa[i]);
		}
		else
		if (key == _T("color"))
		{
			ExStringToArray(val, sa);
			for(int i = 0; i < (int)min(_countof(m_Color), sa.GetCount()); ++i)
				m_Color[i] = ExStringToColor(sa[i]);
		}
		else
		if (key == _T("image"))
		{
			ExStringToArray(val, sa);
			for(int i = 0; i < (int)min(_countof(m_Image), sa.GetCount()); ++i)
				m_Image[i] = ExGet<CImage>(sa[i]);
		}
		else
		if (key == _T("text"))
			SetState(_T("text"), (void*)&val);
		else
		if (key == _T("status"))
		{
			int sta_i = _ttol(val);
			if (sta_i == 0 && val != _T("0"))
			{
				if (val == _T("nor") || val == _T("normal"))
					sta_i = (int)nor;
				else
				if (val == _T("ovr") || val == _T("over"))
					sta_i = (int)ovr;
				else
				if (val == _T("prs") || val == _T("press"))
					sta_i = (int)prs;
				else
				if (val == _T("psh") || val == _T("push"))
					sta_i = (int)psh;
			}
			SetState(_T("status"), (void*)sta_i);
		}
		else
		if (key == _T("locate"))
		{
			int loc_i = _ttol(val);
			if (loc_i == 0 && val != _T("0"))
			{
				if (val == _T("center"))
					loc_i = (int)center;
				else
				if (val == _T("top"))
					loc_i = (int)top;
				else
				if (val == _T("bottom"))
					loc_i = (int)bottom;
				else
				if (val == _T("left"))
					loc_i = (int)left;
				else
				if (val == _T("right"))
					loc_i = (int)right;
			}
			SetState(_T("locate"), (void*)loc_i);
		}
		else
		if (key == _T("loc_off"))
			SetState(_T("loc_off"), (void*)_ttol(val));
		else
		if (key == _T("shake_ico"))
			SetState(_T("shake_ico"), (void*)_ttoi(val));
		else
		if (key == _T("thr_sta"))
		{
			int sta_i = _ttol(val);
			if (sta_i == 0 && val != _T("0"))
			{
				ExStringToArray(val, sa);
				for(int i = 0; i < (int)sa.GetCount(); ++i)
				{
					if (sa[i] == _T("0x01") || sa[i] == _T("NO_OVER"))
						sta_i |= 0x01;
					else
					if (sa[i] == _T("0x02") || sa[i] == _T("NO_PRESS"))
						sta_i |= 0x02;
					else
					if (sa[i] == _T("0x04") || sa[i] == _T("NO_DISABLE"))
						sta_i |= 0x04;
					else
					if (sa[i] == _T("0x08") || sa[i] == _T("NO_FOCUS"))
						sta_i |= 0x08;
					else
					if (sa[i] == _T("0x10") || sa[i] == _T("NO_MID_CHECK"))
						sta_i |= 0x10;
				}
			}
			SetState(key, (void*)sta_i);
		}
		else
		if (key == _T("btn_sty"))
		{
			int sty_i = _ttol(val);
			if (sty_i == 0 && val != _T("0"))
			{
				if (val == _T("nor_btn"))
					sty_i = (int)nor_btn;
				else
				if (val == _T("psh_btn"))
					sty_i = (int)psh_btn;
				else
				if (val == _T("check_box"))
					sty_i = (int)check_box;
			}
			SetState(key, (void*)sty_i);
		}
		else
		if (key == _T("chk_sta"))
			SetState(key, (void*)_ttoi(val));
		else
		if (key == _T("icon"))
			SetState(_T("icon"), ExGet<CImage>(val));
		else
		if (key == _T("ico_off"))
			SetState(_T("ico_off"), (void*)_ttol(val));
		else
		if (key == _T("glow"))
		{
			CString temp(val);
			temp.Lower();
			if (temp == _T("false"))
				SetState(_T("glow"), (void*)FALSE);
			else
			if (temp == _T("true"))
				SetState(_T("glow"), (void*)TRUE);
		}
		return TRUE;
	}

	// 获得控件状态
	void* GetState(const CString& sType, void* pParam = NULL)
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
		if (sType == _T("btn_sty"))
			return (void*)m_BtnSty;
		else
		if (sType == _T("chk_sta"))
			return (void*)m_CheckSta;
		else
		if (sType == _T("color"))
			return (void*)m_Color;
		else
		if (sType == _T("image"))
			return (void*)m_Image;
		else
		if (sType == _T("font"))
			return (void*)m_Text;
		else
		if (sType == _T("text"))
			return (void*)(&m_Str);
		else
		if (sType == _T("icon"))
			return (void*)m_Icon;
		else
		if (sType == _T("ico_off"))
			return (void*)m_IcoOff;
		else
		if (sType == _T("glow"))
			return (void*)m_bGlow;
		else
			return EXP_BASE::GetState(sType);
	}
	BOOL SetState(const CString& sType, void* pState, void* pParam = NULL)
	{
		if (sType == _T("status"))
			SET_STATE(status_t, m_Status)
		else
		if (sType == _T("locate"))
			SET_STATE(locate_t, m_Locate)
		else
		if (sType == _T("loc_off"))
			SET_STATE(LONG, m_LocOff)
		else
		if (sType == _T("shake_ico"))
			SET_STATE(int, m_ShakeIco)
		else
		if (sType == _T("thr_sta"))
			SET_STATE(int, m_ThreeSta)
		else
		if (sType == _T("btn_sty"))
			SET_STATE(styles_t, m_BtnSty)
		else
		if (sType == _T("chk_sta"))
			SET_STATE(int, m_CheckSta)
		else
		if (sType == _T("color"))
		{
			for(int i = 0; i < _countof(m_Color); ++i)
				m_Color[i] = ((pixel_t*)pState)[i];
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("image"))
		{
			for(int i = 0; i < _countof(m_Image); ++i)
				m_Image[i] = (CImage*)pState + i;
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("font"))
		{
			for(int i = 0; i < _countof(m_Text); ++i)
				m_Text[i] = (CText*)pState + i;
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("text"))
		{
			m_Str = *(CString*)pState;
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("icon"))
		{
			m_Icon = (CImage*)pState;
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("ico_off"))
			SET_STATE(LONG, m_IcoOff)
		else
		if (sType == _T("glow"))
			SET_STATE(BOOL, m_bGlow)
		return FALSE;
	}
};

//////////////////////////////////////////////////////////////////

class CGuiStatic : public CGuiButton
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiStatic, CGuiButton)

public:
	CGuiStatic()
	{
		SetState(_T("thr_sta"), (void*)(0x01 | 0x02 | 0x04 | 0x08));
	}
};

//////////////////////////////////////////////////////////////////

class CGuiSimpBtn : public CGuiButton
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiSimpBtn, CGuiButton)

public:
	CGuiSimpBtn()
	{
		SetState(_T("thr_sta"), (void*)(0x04 | 0x08));
	}
};

//////////////////////////////////////////////////////////////////

class CGuiPushBtn : public CGuiSimpBtn
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiPushBtn, CGuiSimpBtn)

public:
	CGuiPushBtn()
	{
		SetState(_T("btn_sty"), (void*)psh_btn);
	}
};

//////////////////////////////////////////////////////////////////

class CGuiCheckBox : public CGuiButton
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiCheckBox, CGuiButton)

public:
	CGuiCheckBox()
	{
		SetState(_T("btn_sty"), (void*)check_box);
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiButton, IGuiCtrlBase)
EXP_IMPLEMENT_DYNCREATE_MULT(CGuiStatic, CGuiButton)
EXP_IMPLEMENT_DYNCREATE_MULT(CGuiSimpBtn, CGuiButton)
EXP_IMPLEMENT_DYNCREATE_MULT(CGuiPushBtn, CGuiSimpBtn)
EXP_IMPLEMENT_DYNCREATE_MULT(CGuiCheckBox, CGuiButton)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiButton_hpp__*/