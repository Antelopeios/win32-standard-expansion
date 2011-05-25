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
// Text - 文字对象类
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-24
// Version:	1.0.0000.2204
//////////////////////////////////////////////////////////////////

#ifndef __Text_h__
#define __Text_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgTypes/Types/Types.h"
#include "ImgTypes/Graph.h"
#include "ImgTypes/Image.h"
#include "ImgTypes/Font.h"
#include "ImgDrawer/ImgRenderer.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CText : public CString
{
protected:
	CFont m_Font;
	pixel_t m_Color;

public:
	CText(LPCTSTR sCont = NULL, font_t tFont = NULL, pixel_t tColor = ExRGBA(0, 0, 0, EXP_CM))
		: CString(sCont)
		, m_Font(tFont)
		, m_Color(tColor)
	{}
	virtual ~CText()
	{}

public:
	const CFont& GetFont() const
	{
		return m_Font;
	}
	void SetFont(const CFont& Font)
	{
		m_Font.Delete();
		m_Font.Set(Font.Clone());
	}

	pixel_t GetColor() const
	{
		return m_Color;
	}
	void SetColor(pixel_t tColor)
	{
		m_Color = tColor;
	}

	CText& operator=(const CText& txt)
	{
		(CString)(*this) = (CString)txt;
		SetColor(txt.GetColor());
		SetFont(txt.GetFont());
	}

	friend bool operator==(const CText& txt1, const CText& txt2)
	{
		if ((CString)txt1 != (CString)txt2) return false;
		if (txt1.m_Color != txt2.m_Color) return false;
		LOGFONT lf1 = {0}, lf2 = {0};
		txt1.m_Font.GetLogFont(&lf1);
		txt2.m_Font.GetLogFont(&lf2);
		return (memcmp(&lf1, &lf2, sizeof(LOGFONT)) == 0);
	}
	friend bool operator!=(const CText& txt1, const CText& txt2)
	{
		return !(txt1 == txt2);
	}

	void GetSize(graph_t tGrp, CSize& szCont)
	{
		SIZE size = {0};
		::GetTextExtentPoint32(tGrp, GetCStr(), (int)GetLength(), &size);
		szCont = size;
	}

	image_t GetImage()
	{
		CGraph tmp_grp;
		tmp_grp.Create();
		tmp_grp.SetObject(m_Font.Get());
		::SetTextColor(tmp_grp, m_Color);
		::SetBkMode(tmp_grp, TRANSPARENT);

		CSize sz;
		GetSize(tmp_grp, sz);

		CImage img;
		img.Create(sz.cx, sz.cy);
		if (img.IsNull()) return NULL;
		tmp_grp.SetObject(img.Get());

		CImgRenderer::Render(img, img, CRect(), CPoint(), 
			&CFilterFillT<CFilterCopy>(ExRGBA(0, 0, 0, EXP_CM), 0x1));
		::DrawText(tmp_grp, GetCStr(), (int)GetLength(), 
			&(RECT)CRect(0, 0, sz.cx, sz.cy), DT_LEFT | DT_TOP);
		CImgRenderer::Render(img, img, CRect(), CPoint(), 
			&CFilterInverseT<CFilterCopy>(0x1));
		if (ExGetA(m_Color) != EXP_CM)
			CImgRenderer::Render(img, img, CRect(), CPoint(), 
				&CFilterFillT<CFilterCopy>(ExRGBA(0, 0, 0, ExGetA(m_Color)), 0x1, true));

		tmp_grp.Delete();

		return img;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Text_h__*/