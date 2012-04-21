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
// GuiEdit - 编辑框控件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-08-26
// Version:	1.0.0004.1107
//
// History:
//	- 1.0.0000.1030(2011-06-21)	@ 开始构建GuiEdit
//	- 1.0.0001.1615(2011-06-27)	+ 添加txt_sel_color与bkg_sel_color属性
//	- 1.0.0002.2018(2011-06-29)	= 将CGuiEdit的基类由CGuiButton改为CGuiPicture
//								^ 简化CGuiEdit的属性定义
//								+ 添加创建输入法上下文的相关代码
//	- 1.0.0003.0455(2011-08-01)	+ 将CGuiEdit的一些属性拓展为数组,以支持更多属性状态
//	- 1.0.0004.1107(2011-08-26)	+ 添加CGuiEdit的empty_text属性,支持无文字时显示默认文字
//////////////////////////////////////////////////////////////////

#ifndef __GuiEdit_hpp__
#define __GuiEdit_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment(lib, "Imm32.lib")

EXP_BEG
	
//////////////////////////////////////////////////////////////////

class _edt_font : public _pic_font
{
	EXP_DECLARE_DYNCREATE_CLS(_edt_font, _pic_font)

public:
	void Msg(UINT nMessage, WPARAM wParam, LPARAM lParam) {}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_edt_font, _pic_font)

//////////////////////////////////////////////////////////////////

class _edt_empty_font : public _pic_font
{
	EXP_DECLARE_DYNCREATE_CLS(_edt_empty_font, _pic_font)

public:
	CString GetKey() const { return _T("empty_font"); }
	void Msg(UINT nMessage, WPARAM wParam, LPARAM lParam) {}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_edt_empty_font, _pic_font)

//////////////////////////////////////////////////////////////////

class _edt_empty_text : public _pic_text
{
	EXP_DECLARE_DYNCREATE_CLS(_edt_empty_text, _pic_text)

public:
	CString GetKey() const { return _T("empty_text"); }
};

EXP_IMPLEMENT_DYNCREATE_CLS(_edt_empty_text, _pic_text)

//////////////////////////////////////////////////////////////////

class _edt_txt_sel_color : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_edt_txt_sel_color, IGuiSet)

protected:
	pixel_t m_Color[2];

public:
	_edt_txt_sel_color()
	{ ZeroMemory(m_Color, sizeof(m_Color)); }

public:
	CString GetKey() const { return _T("txt_sel_color"); }
	BOOL Exc(const CString& val)
	{
		CArrayT<CString> sa;
		ExStringToArray(val, sa);
		for(int i = 0; i < (int)min(_countof(m_Color), sa.GetCount()); ++i)
			m_Color[i] = ExStringToColor(sa[i]);
		Ctl()->UpdateState();
		return TRUE;
	}
	void* Get(void* par = NULL)
	{
		return (void*)m_Color;
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		for(int i = 0; i < _countof(m_Color); ++i)
			m_Color[i] = ((pixel_t*)sta)[i];
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_edt_txt_sel_color, IGuiSet)
	
//////////////////////////////////////////////////////////////////

class _edt_bkg_sel_color : public _edt_txt_sel_color
{
	EXP_DECLARE_DYNCREATE_CLS(_edt_bkg_sel_color, _edt_txt_sel_color)

public:
	CString GetKey() const { return _T("bkg_sel_color"); }
};

EXP_IMPLEMENT_DYNCREATE_CLS(_edt_bkg_sel_color, _edt_txt_sel_color)

//////////////////////////////////////////////////////////////////

// 是否是密码框
class _edt_password : public ICtrlSetT<BOOL>
{
	EXP_DECLARE_DYNCREATE_CLS(_edt_password, IGuiSet)

public:
	CString GetKey() const { return _T("password"); }
};

EXP_IMPLEMENT_DYNCREATE_CLS(_edt_password, IGuiSet)

//////////////////////////////////////////////////////////////////

class CGuiEdit : public CGuiPicture
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiEdit, CGuiPicture)

protected:
	HIMC m_ImmContext;

protected:
	void Init(IGuiComp* pComp)
	{
		EXP_BASE::Init(pComp);
		// 为窗口创建输入法上下文
		IGuiBase* base = ExDynCast<IGuiBase>(pComp);
		ExAssert(base); if (!base) return;
		::ImmAssociateContext(base->GethWnd(), m_ImmContext = ::ImmCreateContext());
	}
	void Fina()
	{
		// 销毁输入法上下文
		::ImmDestroyContext(m_ImmContext);
		EXP_BASE::Fina();
	}

public:
	CGuiEdit()
		: m_ImmContext(NULL)
	{
		// 添加逻辑对象
		AddSet(_T("_edt_font"));
		AddSet(_T("_edt_empty_font"));
		AddSet(_T("_edt_empty_text"));
		AddSet(_T("_edt_txt_sel_color"));
		AddSet(_T("_edt_bkg_sel_color"));
		AddSet(_T("_edt_password"));
		// 添加事件对象
		AddEvent(_T("CGuiEditEvent"));
		// 设置默认属性
		SetState(_T("color"), (void*)ExRGBA(EXP_CM, EXP_CM, EXP_CM, EXP_CM));
		pixel_t pix[2] = {ExRGBA(0, 0, 0, EXP_CM), ExRGBA(0, 0, 0, EXP_CM)};
		SetState(_T("txt_sel_color"), pix);
		pix[0] = ExRGBA(173, 214, 255, EXP_CM);
		pix[1] = ExRGBA(229, 235, 241, EXP_CM);
		SetState(_T("bkg_sel_color"), pix);
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiEdit, CGuiPicture)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiEdit_hpp__*/