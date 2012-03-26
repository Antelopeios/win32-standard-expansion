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
// GuiPicture - 绘图板控件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-08-08
// Version:	1.0.0004.1100
//
// History:
//	- 1.0.0001.2236(2011-05-23)	+ 添加CGuiPicture::IsUpdated()接口
//	- 1.0.0002.1500(2011-05-24)	+ CGuiPicture添加Color属性
//	- 1.0.0003.2305(2011-05-25)	+ CGuiPicture添加Text属性
//								= CGuiPicture调整控件状态接口
//								= CGuiPicture不再托管内部的图片资源
//	- 1.0.0004.1100(2011-08-08)	^ 将Color属性由指针赋值改为内容拷贝,减少调用复杂度
//////////////////////////////////////////////////////////////////

#ifndef __GuiPicture_hpp__
#define __GuiPicture_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class _pic_style : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_pic_style, IGuiSet)

public:
	CString GetKey() const { return _T("style"); }
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

class _pic_font : public ICtrlSetT<CText*>
{
	EXP_DECLARE_DYNCREATE_CLS(_pic_font, IGuiSet)

public:
	CString GetKey() const { return _T("font"); }
	BOOL Exc(const CString& val)
	{
		CArrayT<CString> sa;
		ExStringToArray(val, sa);
		Set(ExGet<CText>(sa[0]));
		Ctl()->UpdateState();
		return TRUE;
	}
	void Msg(UINT nMessage, WPARAM wParam, LPARAM lParam)
	{
		if (nMessage != WM_PAINT) return;

		if (!m_Val) return;
		CString* str = (CString*)Ctl()->GetState(_T("text"));
		if (!str) return;

		CImage txt_img(m_Val->GetImage(*str));
		if (txt_img.IsNull()) return;

		CGraph* mem_img = (CGraph*)lParam;
		if (!mem_img || mem_img->IsNull()) return;

		CRect rect;
		Ctl()->GetClientRect(rect);

		CImgDrawer::Draw(*mem_img, txt_img, CRect(
				(rect.Right() - txt_img.GetWidth()) / 2, 
				(rect.Bottom() - txt_img.GetHeight()) / 2, 
				rect.Right(), rect.Bottom()));
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_pic_font, IGuiSet)

//////////////////////////////////////////////////////////////////

class _pic_color : public ICtrlSetT<pixel_t>
{
	EXP_DECLARE_DYNCREATE_CLS(_pic_color, IGuiSet)

public:
	CString GetKey() const { return _T("color"); }
	BOOL Exc(const CString& val)
	{
		CArrayT<CString> sa;
		ExStringToArray(val, sa);
		Set((void*)ExStringToColor(sa[0]));
		Ctl()->UpdateState();
		return TRUE;
	}
	void Msg(UINT nMessage, WPARAM wParam, LPARAM lParam)
	{
		if (nMessage != WM_PAINT) return;

		CGraph* mem_img = (CGraph*)lParam;
		if (!mem_img || mem_img->IsNull()) return;

		CRect rect;
		Ctl()->GetClientRect(rect);

		CImgDrawer::Fill(*mem_img, rect, m_Val);
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_pic_color, IGuiSet)

//////////////////////////////////////////////////////////////////
	
class _pic_image : public ICtrlSetT<CImage*>
{
	EXP_DECLARE_DYNCREATE_CLS(_pic_image, IGuiSet)

public:
	CString GetKey() const { return _T("image"); }
	BOOL Exc(const CString& val)
	{
		CArrayT<CString> sa;
		ExStringToArray(val, sa);
		Set((void*)ExGet<CImage>(sa[0]));
		Ctl()->UpdateState();
		return TRUE;
	}
	void Msg(UINT nMessage, WPARAM wParam, LPARAM lParam)
	{
		if (nMessage != WM_PAINT) return;

		if (!m_Val || m_Val->IsNull()) return;

		CGraph* mem_img = (CGraph*)lParam;
		if (!mem_img || mem_img->IsNull()) return;

		CRect rect;
		Ctl()->GetClientRect(rect);

		CImgDrawer::Draw(*mem_img, m_Val->Get(), rect, 
			CPoint(), CSize(rect.Width(), rect.Height()));
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_pic_image, IGuiSet)

//////////////////////////////////////////////////////////////////

class _pic_text : public ICtrlSetT<CString>
{
	EXP_DECLARE_DYNCREATE_CLS(_pic_text, IGuiSet)

public:
	CString GetKey() const { return _T("text"); }
};

EXP_IMPLEMENT_DYNCREATE_CLS(_pic_text, IGuiSet)

//////////////////////////////////////////////////////////////////

class CGuiPicture : public IGuiCtrlBase
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiPicture, IGuiCtrlBase)

public:
	CGuiPicture()
	{
		// 添加逻辑对象
		AddSet(_T("_pic_style"));
		AddSet(_T("_pic_image"));
		AddSet(_T("_pic_color"));
		AddSet(_T("_pic_text"));
		AddSet(_T("_pic_font"));
	}
};

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiPicture, IGuiCtrlBase)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiPicture_hpp__*/