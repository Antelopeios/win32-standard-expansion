// Copyright 2011-2012, ľͷ��
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
// Text - ���ֶ�����
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-06
// Version:	1.0.0009.1123
//
// History:
//	- 1.0.0001.1425(2011-05-25)	# ����CText::operator=()�ĸ�ֵ������ֵ����
//								# ����CText::GetImage()�ӿڶ���ɫ����Ĵ���
//	- 1.0.0002.1553(2011-05-30)	= ����CText�Ļ���,��Ϊͬʱ��CString��CFont�̳�,��ͳһ����֮��Ľӿ�
//	- 1.0.0003.1919(2011-06-24)	# ����CText::GetSize()�޷��Զ�ƥ�������ȡ��С������
//	- 1.0.0004.1610(2011-08-10)	^ ʹ�û�������Ż�CText::GetImage()��Ч��
//	- 1.0.0005.1742(2011-08-12)	^ ��CText::GetSize()�ӿ�,��֧��ʡ��tGrp����
//	- 1.0.0006.1331(2012-02-03)	- ��CString��CText����
//	- 1.0.0007.2327(2012-02-29)	+ ����µ�GetImage()�ӿ�,֧������ض�����������ı�
//	- 1.0.0008.2050(2012-03-04)	# �����ú�/������ͼ���ϵķ�ʽ����ʵ�ʵ�����ͼ��,����ĳЩ�������Ч�������������
//	- 1.0.0009.1123(2012-03-06)	^ ʹ��ExtTextOut����TextOut�����������,������Ч��
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
		// ������ʱ����
		CGraph tmp_grp;
		tmp_grp.Create();
		tmp_grp.SetObject(Get());
		// �����������
		CSize sz;
		GetSize(m_MemStr, sz, tmp_grp);
		// ��������ͼ��
		m_MemImg.Create(sz.cx, sz.cy);
		if (m_MemImg.IsNull())
		{
			tmp_grp.Delete();
			return NULL;
		}
		// ����ͼ�㴦��
		CImage w_img; w_img.Create(sz.cx, sz.cy);
		CImage b_img; b_img.Create(sz.cx, sz.cy);
		CRect rc(0, 0, sz.cx, sz.cy);
		pixel_t w_bk = ExRGBA(EXP_CM, EXP_CM, EXP_CM, EXP_CM), b_bk = ExRGBA(0, 0, 0, EXP_CM);
		::SetBkMode(tmp_grp, TRANSPARENT);
		// ��ɫ����
		CImgFilter::Filter(w_img, &CFilterBrush(w_bk));
		tmp_grp.SetObject(w_img.Get());
		::ExtTextOut(tmp_grp, 0, 0, 0, NULL, m_MemStr, (UINT)m_MemStr.GetLength(), NULL);
		// ��ɫ����
		CImgFilter::Filter(b_img, &CFilterBrush(b_bk));
		tmp_grp.SetObject(b_img.Get());
		::ExtTextOut(tmp_grp, 0, 0, 0, NULL, m_MemStr, (UINT)m_MemStr.GetLength(), NULL);
		// ����ǰ��ɫ
		pixel_t* p_w = w_img.GetPixels();
		pixel_t* p_b = b_img.GetPixels();
		pixel_t* p_s = m_MemImg.GetPixels();
		for(int y = 0; y < sz.cy; ++y)
		{
			for(int x = 0; x < sz.cx; ++x)
			{
				int inx = y * sz.cx + x;
				pixel_t w_p = p_w[inx], b_p = p_b[inx];
				// ���λͼ����
				chann_t w_r = ExGetR(w_p), b_r = ExGetR(b_p);
				// ����Alphaͨ��
				int a_s = (EXP_CM - w_r + b_r) * ExGetA(m_Color);
				// ���Դ����
				p_s[inx] = ExRGBA(
					ExGetB(m_Color), 
					ExGetG(m_Color), 
					ExGetR(m_Color), 
					ExDivCM(a_s));
			}
		}
		CImgFilter::Filter(m_MemImg, &CFilterPreMul());
		// �����ڴ沢����
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
			// ��������ı�
			GetSize(clp_str, txt_clp);
			if (txt_clp.cx == 0 || txt_clp.cy == 0) break;
			if (txt_clp.cx > rcImg.Width())
			{
				// ������һ���Ƿ���ʾ�߶Ȳ���
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
			// ���Ǽ����ı�
			CImage img_tmp(GetImage(clp_str));
			CImgDrawer::Cover(img_clp, img_tmp, CRect(0, txt_off, rcImg.Width(), txt_off + txt_clp.cy));
			// ����
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