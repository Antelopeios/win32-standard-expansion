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
// Text - 文字对象类
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-06
// Version:	1.0.0009.1123
//
// History:
//	- 1.0.0001.1425(2011-05-25)	# 修正CText::operator=()的赋值及返回值错误
//								# 修正CText::GetImage()接口对颜色处理的错误
//	- 1.0.0002.1553(2011-05-30)	= 调整CText的基类,改为同时向CString与CFont继承,以统一他们之间的接口
//	- 1.0.0003.1919(2011-06-24)	# 修正CText::GetSize()无法自动匹配字体获取大小的问题
//	- 1.0.0004.1610(2011-08-10)	^ 使用缓存机制优化CText::GetImage()的效率
//	- 1.0.0005.1742(2011-08-12)	^ 简化CText::GetSize()接口,可支持省略tGrp参数
//	- 1.0.0006.1331(2012-02-03)	- 将CString与CText分离
//	- 1.0.0007.2327(2012-02-29)	+ 添加新的GetImage()接口,支持输出特定区域的折行文本
//	- 1.0.0008.2050(2012-03-04)	# 尝试用黑/白文字图层混合的方式计算实际的文字图层,修正某些字体输出效果不理想的问题
//	- 1.0.0009.1123(2012-03-06)	^ 使用ExtTextOut代替TextOut进行文字输出,提高输出效率
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
#include "ImgPainter/ImgDrawer.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CText : public CFont
{
protected:
	pixel_t m_Color;
	CImage m_MemImg;

	CString m_MemStr;
	CFont m_MemFnt;
	pixel_t m_MemClr;

public:
	CText(font_t tFont = NULL, pixel_t tColor = ExRGBA(0, 0, 0, EXP_CM))
		: CFont(tFont)
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
		SetColor(txt.GetColor());
		SetFont(txt.GetFont());
		return (*this);
	}

	friend BOOL operator==(const CText& txt1, const CText& txt2)
	{
		if ((CFont)txt1 != (CFont)txt2) return FALSE;
		if (txt1.m_Color != txt2.m_Color) return FALSE;
		return TRUE;
	}
	friend BOOL operator!=(const CText& txt1, const CText& txt2)
	{
		return !(txt1 == txt2);
	}

	void GetSize(const CString& s, CSize& szCont, graph_t tGrp = NULL)
	{
		graph_t grp_tmp = NULL;
		if (!tGrp) tGrp = grp_tmp = ::CreateCompatibleDC(NULL);
		HGDIOBJ old = ::SelectObject(tGrp, Get());
		SIZE size = {0};
		::GetTextExtentPoint32(tGrp, s, (int)s.GetLength(), &size);
		szCont = size;
		::SelectObject(tGrp, old);
		if (grp_tmp) ::DeleteDC(grp_tmp);
	}

	image_t GetImage(const CString& sStr)
	{
		if (sStr == _T("")) return NULL;
		if (m_MemStr == sStr && 
			m_MemFnt == (CFont)(*this) && 
			m_MemClr == m_Color) return m_MemImg;
		m_MemStr = sStr;
		m_MemFnt = (CFont)(*this);
		m_MemClr = m_Color;
		// 创建临时画板
		CGraph tmp_grp;
		tmp_grp.Create();
		tmp_grp.SetObject(Get());
		// 获得文字区域
		CSize sz;
		GetSize(m_MemStr, sz, tmp_grp);
		// 创建文字图像
		m_MemImg.Create(sz.cx, sz.cy);
		if (m_MemImg.IsNull())
		{
			tmp_grp.Delete();
			return NULL;
		}
		// 文字图层处理
		CImage w_img; w_img.Create(sz.cx, sz.cy);
		CImage b_img; b_img.Create(sz.cx, sz.cy);
		CRect rc(0, 0, sz.cx, sz.cy);
		pixel_t w_bk = ExRGBA(EXP_CM, EXP_CM, EXP_CM, EXP_CM), b_bk = ExRGBA(0, 0, 0, EXP_CM);
		::SetBkMode(tmp_grp, TRANSPARENT);
		// 白色背景
		CImgFilter::Filter(w_img, &CFilterBrush(w_bk));
		tmp_grp.SetObject(w_img.Get());
		::ExtTextOut(tmp_grp, 0, 0, 0, NULL, m_MemStr, (UINT)m_MemStr.GetLength(), NULL);
		// 黑色背景
		CImgFilter::Filter(b_img, &CFilterBrush(b_bk));
		tmp_grp.SetObject(b_img.Get());
		::ExtTextOut(tmp_grp, 0, 0, 0, NULL, m_MemStr, (UINT)m_MemStr.GetLength(), NULL);
		// 计算前景色
		pixel_t* p_w = w_img.GetPixels();
		pixel_t* p_b = b_img.GetPixels();
		pixel_t* p_s = m_MemImg.GetPixels();
		for(int y = 0; y < sz.cy; ++y)
		{
			for(int x = 0; x < sz.cx; ++x)
			{
				int inx = y * sz.cx + x;
				pixel_t w_p = p_w[inx], b_p = p_b[inx];
				// 获得位图像素
				chann_t w_r = ExGetR(w_p), b_r = ExGetR(b_p);
				// 计算Alpha通道
				int a_s = (EXP_CM - w_r + b_r) * ExGetA(m_Color);
				// 获得源像素
				p_s[inx] = ExRGBA(
					ExGetB(m_Color), 
					ExGetG(m_Color), 
					ExGetR(m_Color), 
					ExDivCM(a_s));
			}
		}
		CImgFilter::Filter(m_MemImg, &CFilterPreMul());
		// 清理内存并返回
		tmp_grp.Delete();
		return m_MemImg;
	}
	image_t GetImage(const CString& sStr, const CRect& rcImg, const int nSpace = 2, const CString& sClp = _T("..."))
	{
		CString clp_str(sStr), clp_lne(sStr);
		CImage img_clp;
		img_clp.SetTrust(FALSE);
		img_clp.Create(rcImg.Width(), rcImg.Height());
		CSize txt_clp;
		LONG txt_off = 0;
		do
		{
			// 计算剪切文本
			GetSize(clp_str, txt_clp);
			if (txt_clp.cx == 0 || txt_clp.cy == 0) break;
			if (txt_clp.cx > rcImg.Width())
			{
				// 计算下一行是否显示高度不够
				if (txt_off + ((txt_clp.cy + nSpace) << 1) <= rcImg.Height())
				{
					do
					{
						clp_str.LastItem() = 0;
						GetSize(clp_str, txt_clp);
					} while (!clp_str.Empty() && txt_clp.cx > rcImg.Width());
				}
				else
				{
					CString tmp(clp_str);
					do
					{
						tmp.LastItem() = 0;
						clp_str = tmp;
						clp_str += sClp;
						GetSize(clp_str, txt_clp);
					} while (!tmp.Empty() && txt_clp.cx > rcImg.Width());
				}
			}
			// 覆盖剪切文本
			CImage img_tmp(GetImage(clp_str));
			CImgDrawer::Cover(img_clp, img_tmp, CRect(0, txt_off, rcImg.Width(), txt_off + txt_clp.cy));
			// 折行
			clp_lne = ((LPCTSTR)clp_lne) + clp_str.GetLength();
			clp_str = clp_lne;
			txt_off += (txt_clp.cy + nSpace);
		} while(txt_off + txt_clp.cy <= rcImg.Height());
		return img_clp;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Text_h__*/