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
// GuiCtrlSet - 控件基础逻辑
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-22
// Version:	1.0.0000.1525
//
// History:
//	- 1.0.0000.1525(2011-03-22)	@ 开始构建GuiCtrlSet
//////////////////////////////////////////////////////////////////

#ifndef __GuiCtrlSet_hpp__
#define __GuiCtrlSet_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////
// CGuiPicture
//////////////////////////////////////////////////////////////////

class _pic_style : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_pic_style, IGuiSet)

public:
	BOOL Key(const CString& key) const { return (key == _T("style")); }
	BOOL Exc(const CString& val)
	{
		style_t* sty = ExGet<style_t>(val);
		if(!sty) return TRUE;
		if(!sty->font.Empty())
			Ctl()->SetState(_T("font"), sty->font[0]);
		if(!sty->color.Empty())
			Ctl()->SetState(_T("color"), (void*)(sty->color[0]));
		if(!sty->image.Empty())
			Ctl()->SetState(_T("image"), sty->image[0]);
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_pic_style, IGuiSet)

//////////////////////////////////////////////////////////////////

class _pic_font : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_pic_font, IGuiSet)

protected:
	CText* m_Text;

public:
	_pic_font()
		: m_Text(NULL)
	{}

public:
	BOOL Key(const CString& key) const { return (key == _T("font")); }
	BOOL Exc(const CString& val)
	{
		CArrayT<CString> sa;
		ExStringToArray(val, sa);
		Set(ExGet<CText>(sa[0]));
		return TRUE;
	}
	void* Get(void* par = NULL)
	{
		return (void*)m_Text;
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		m_Text = (CText*)sta;
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_pic_font, IGuiSet)

//////////////////////////////////////////////////////////////////

class _pic_color : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_pic_color, IGuiSet)

protected:
	pixel_t m_Color;

public:
	_pic_color()
		: m_Color(ExRGBA(0, 0, 0, 0))
	{}

public:
	BOOL Key(const CString& key) const { return (key == _T("color")); }
	BOOL Exc(const CString& val)
	{
		CArrayT<CString> sa;
		ExStringToArray(val, sa);
		Set((void*)ExStringToColor(sa[0]));
		return TRUE;
	}
	void* Get(void* par = NULL)
	{
		return (void*)m_Color;
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		m_Color = (pixel_t)sta;
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_pic_color, IGuiSet)

//////////////////////////////////////////////////////////////////
	
class _pic_image : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_pic_image, IGuiSet)

protected:
	CImage* m_Image;

public:
	_pic_image()
		: m_Image(NULL)
	{}

public:
	BOOL Key(const CString& key) const { return (key == _T("image")); }
	BOOL Exc(const CString& val)
	{
		CArrayT<CString> sa;
		ExStringToArray(val, sa);
		Set((void*)ExGet<CImage>(sa[0]));
		return TRUE;
	}
	void* Get(void* par = NULL)
	{
		return (void*)m_Image;
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		m_Image = (CImage*)sta;
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_pic_image, IGuiSet)

//////////////////////////////////////////////////////////////////

class _pic_text : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_pic_text, IGuiSet)

protected:
	CString m_Str;

public:
	BOOL Key(const CString& key) const { return (key == _T("text")); }
	BOOL Exc(const CString& val)
	{
		Set((void*)&val);
		return TRUE;
	}
	void* Get(void* par = NULL)
	{
		return (void*)(&m_Str);
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		m_Str = *(CString*)sta;
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_pic_text, IGuiSet)

//////////////////////////////////////////////////////////////////
// CGuiButton
//////////////////////////////////////////////////////////////////

class _btn_style : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_style, IGuiSet)

public:
	BOOL Key(const CString& key) const { return (key == _T("style")); }
	BOOL Exc(const CString& val)
	{
		style_t* sty = ExGet<style_t>(val);
		if(!sty) return TRUE;
		if(!sty->font.Empty())
			Ctl()->SetState(_T("font"), (void*)(CText**)sty->font);
		if(!sty->color.Empty())
			Ctl()->SetState(_T("color"), (void*)(pixel_t*)sty->color);
		if(!sty->image.Empty())
			Ctl()->SetState(_T("image"), (void*)(CImage**)sty->image);
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_style, IGuiSet)

//////////////////////////////////////////////////////////////////

class _btn_font : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_font, IGuiSet)

protected:
	CText* m_Text[10];

public:
	_btn_font()
	{ ZeroMemory(m_Text, sizeof(m_Text)); }

public:
	BOOL Key(const CString& key) const { return (key == _T("font")); }
	BOOL Exc(const CString& val)
	{
		CArrayT<CString> sa;
		ExStringToArray(val, sa);
		for(int i = 0; i < (int)min(_countof(m_Text), sa.GetCount()); ++i)
			m_Text[i] = ExGet<CText>(sa[i]);
		return TRUE;
	}
	void* Get(void* par = NULL)
	{
		return (void*)m_Text;
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		if (par)
		{
			for(int i = 0; i < _countof(m_Text); ++i)
				m_Text[i] = ((CText**)sta)[i];
		}
		else
		{
			for(int i = 0; i < _countof(m_Text); ++i)
				m_Text[i] = (CText*)sta + i;
		}
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_font, IGuiSet)

//////////////////////////////////////////////////////////////////

class _btn_color : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_color, IGuiSet)

protected:
	pixel_t m_Color[10];

public:
	_btn_color()
	{ ZeroMemory(m_Color, sizeof(m_Color)); }

public:
	BOOL Key(const CString& key) const { return (key == _T("color")); }
	BOOL Exc(const CString& val)
	{
		CArrayT<CString> sa;
		ExStringToArray(val, sa);
		for(int i = 0; i < (int)min(_countof(m_Color), sa.GetCount()); ++i)
			m_Color[i] = ExStringToColor(sa[i]);
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

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_color, IGuiSet)

//////////////////////////////////////////////////////////////////

class _btn_image : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_image, IGuiSet)

protected:
	CImage* m_Image[9];	// 九宫格分割,每个小块保存所有状态

public:
	_btn_image()
	{ ZeroMemory(m_Image, sizeof(m_Image)); }

public:
	BOOL Key(const CString& key) const { return (key == _T("image")); }
	BOOL Exc(const CString& val)
	{
		CArrayT<CString> sa;
		ExStringToArray(val, sa);
		for(int i = 0; i < (int)min(_countof(m_Image), sa.GetCount()); ++i)
			m_Image[i] = ExGet<CImage>(sa[i]);
		return TRUE;
	}
	void* Get(void* par = NULL)
	{
		return (void*)m_Image;
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		if (par)
		{
			for(int i = 0; i < _countof(m_Image); ++i)
				m_Image[i] = ((CImage**)sta)[i];
		}
		else
		{
			for(int i = 0; i < _countof(m_Image); ++i)
				m_Image[i] = (CImage*)sta + i;
		}
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_image, IGuiSet)

//////////////////////////////////////////////////////////////////

class _btn_text : public _pic_text
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_text, _pic_text)
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_text, _pic_text)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiCtrlSet_hpp__*/
