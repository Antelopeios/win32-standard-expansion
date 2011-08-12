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
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-08-12
// Version:	1.0.0005.1742
//
// History:
//	- 1.0.0001.1425(2011-05-25)	# 修正CText::operator=()的赋值及返回值错误
//								# 修正CText::GetImage()接口对颜色处理的错误
//	- 1.0.0002.1553(2011-05-30)	= 调整CText的基类,改为同时向CString与CFont继承,以统一他们之间的接口
//	- 1.0.0003.1919(2011-06-24)	# 修正CText::GetSize()无法自动匹配字体获取大小的问题
//	- 1.0.0004.1610(2011-08-10)	^ 使用缓存机制优化CText::GetImage()的效率
//	- 1.0.0005.1742(2011-08-12)	^ 简化CText::GetSize()接口,可支持省略tGrp参数
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
#include "ImgPainter/ImgFilter.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CText : public CString, public CFont
{
protected:
	pixel_t m_Color;
	CImage m_MemImg;

	CString m_MemStr;
	CFont m_MemFnt;
	pixel_t m_MemClr;

public:
	CText(LPCTSTR sCont = NULL, font_t tFont = NULL, pixel_t tColor = ExRGBA(0, 0, 0, EXP_CM))
		: CString(sCont)
		, CFont(tFont)
		, m_Color(tColor)
	{}
	virtual ~CText()
	{}

public:
	const CFont& GetFont() const
	{
		return (*this);
	}
	void SetFont(const CFont& Font)
	{
		Set(Font.Get());
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
		SetString((LPCTSTR)txt);
		SetColor(txt.GetColor());
		SetFont(txt.GetFont());
		return (*this);
	}

	friend bool operator==(const CText& txt1, const CText& txt2)
	{
		if ((CString)txt1 != (CString)txt2) return false;
		if ((CFont)txt1 != (CFont)txt2) return false;
		if (txt1.m_Color != txt2.m_Color) return false;
		return true;
	}
	friend bool operator!=(const CText& txt1, const CText& txt2)
	{
		return !(txt1 == txt2);
	}

	void GetSize(CSize& szCont, graph_t tGrp = NULL)
	{
		graph_t grp_tmp = NULL;
		if (!tGrp) tGrp = grp_tmp = ::CreateCompatibleDC(NULL);
		HGDIOBJ old = ::SelectObject(tGrp, Get());
		SIZE size = {0};
		::GetTextExtentPoint32(tGrp, GetCStr(), (int)GetLength(), &size);
		szCont = size;
		::SelectObject(tGrp, old);
		if (grp_tmp) ::DeleteDC(grp_tmp);
	}

	image_t GetImage()
	{
		if (Empty()) return NULL;
		if (m_MemStr == (CString)(*this) && 
			m_MemFnt == (CFont)(*this) && 
			m_MemClr == m_Color) return m_MemImg;
		m_MemStr = (CString)(*this);
		m_MemFnt = (CFont)(*this);
		m_MemClr = m_Color;
		// 创建临时画板
		CGraph tmp_grp;
		tmp_grp.Create();
		tmp_grp.SetObject(Get());
		::SetBkMode(tmp_grp, TRANSPARENT);
		// 获得文字区域
		CSize sz;
		GetSize(sz, tmp_grp);
		// 创建文字图像
		m_MemImg.Create(sz.cx, sz.cy);
		if (m_MemImg.IsNull()) return NULL;
		tmp_grp.SetObject(m_MemImg.Get());
		// 绘制文字
		CRect rc(0, 0, sz.cx, sz.cy);
		CImgFilter::Filter(m_MemImg, rc, &CFilterFill(ExRGBA(0, 0, 0, EXP_CM), 0x1));
		::DrawText(tmp_grp, GetCStr(), (int)GetLength(), 
			&(RECT)rc, DT_LEFT | DT_TOP);
		CImgFilter::Filter(m_MemImg, rc, &CFilterInverse(0x1));
		if (m_Color != ExRGBA(0, 0, 0, EXP_CM))
			CImgFilter::Filter
				(
				m_MemImg, rc, 
				&CFilterFill
					(
					ExRGBA
						(
						ExGetB(m_Color), 
						ExGetG(m_Color), 
						ExGetR(m_Color), 
						ExGetA(m_Color)
						), 
					0xf, true
					)
				);
		// 清理内存并返回
		tmp_grp.Delete();
		return m_MemImg;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Text_h__*/