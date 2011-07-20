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
// ImgDrawer - 基本图形图像绘制
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-07-20
// Version:	1.0.0002.1747
//
// History:
//	- 1.0.0000.1627(2011-06-20)	@ 重新构建ImgDrawer,作为ImgPainter的子模块,用于绘制基本图形
//	- 1.0.0001.1942(2011-07-16)	# 修正当CImgDrawer::Fill()传入的CRect参数超出image_t界限时导致的访问异常
//	- 1.0.0002.1747(2011-07-20)	# 修正CImgDrawer::Fill()像素循环遍历中的一个低级错误
//////////////////////////////////////////////////////////////////

#ifndef __ImgDrawer_h__
#define __ImgDrawer_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgPainter/ImgRenderer.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CImgDrawer
{
public:
	// 画点
	EXP_INLINE static bool Point(image_t imgDes, CPoint& ptDes, pixel_t pixSrc)
	{
		CImage exp_des;
		exp_des.SetTrust(false);
		exp_des = imgDes;
		if (exp_des.IsNull()) return false;
		CSize sz_des(exp_des.GetWidth(), exp_des.GetHeight());
		pixel_t* pix_des = exp_des.GetPixels();
		if (ptDes.x >= sz_des.cx || ptDes.y >= sz_des.cy) return true;
		LONG i_d = (sz_des.cy - ptDes.y - 1) * sz_des.cx + ptDes.x;
		pix_des[i_d] = pixSrc;
		return true;
	}

	// 填充
	EXP_INLINE static bool Fill(image_t imgDes, const CRect& rcDes, const pixel_t pixSrc)
	{
		CImage exp_des;
		exp_des.SetTrust(false);
		exp_des = imgDes;
		if (exp_des.IsNull()) return false;

		// 格式化区域
		CSize sz_des(exp_des.GetWidth(), exp_des.GetHeight());
		CRect rc_des(rcDes);
		if (rc_des.IsEmpty())
			rc_des.Set(CPoint(), CPoint(sz_des.cx, sz_des.cy));
		else
		{
			if (rc_des.pt1.x < 0) rc_des.pt1.x = 0;
			if (rc_des.pt1.y < 0) rc_des.pt1.y = 0;
			if (rc_des.pt2.x > sz_des.cx) rc_des.pt2.x = sz_des.cx;
			if (rc_des.pt2.y > sz_des.cy) rc_des.pt2.y = sz_des.cy;
		}

		// 获得合适的宽与高
		LONG w = rc_des.Width(), h = rc_des.Height();
		if (w <= 0 || h <= 0) return true;
		// 计算坐标起点
		LONG inx_des = (sz_des.cy - rc_des.Top() - 1) * sz_des.cx + rc_des.Left();

		// 遍历像素绘图
		pixel_t* pix_des = exp_des.GetPixels();
		for(LONG y = 0; y < h; ++y, inx_des -= sz_des.cx)
			for(LONG x = 0, i_d = inx_des; x < w; ++x, ++i_d)
				pix_des[i_d] = pixSrc;
		return true;
	}

#pragma push_macro("PreDraw")
#undef PreDraw
#define PreDraw() \
	CImage exp_des; \
	exp_des.SetTrust(false); \
	exp_des = imgDes; \
	if (exp_des.IsNull()) return false; \
	CSize sz_des(exp_des.GetWidth(), exp_des.GetHeight()); \
	CImage img; \
	img.Create(sz_des.cx, sz_des.cy); \
	Fill(img, CRect(), ExRGBA(EXP_CM, EXP_CM, EXP_CM, EXP_CM)); \
	CGraph grp; \
	grp.Create(); \
	grp.SetObject(img)
//#define PreDraw
#pragma push_macro("EndDraw")
#undef EndDraw
#define EndDraw() \
	grp.Delete(); \
	pixel_t* pix_des = exp_des.GetPixels(); \
	pixel_t* pix_img = img.GetPixels(); \
	for(LONG y_d = 0; y_d < sz_des.cy; ++y_d) \
	{ \
		for(LONG x_d = 0; x_d < sz_des.cx; ++x_d) \
		{ \
			LONG i_d = (sz_des.cy - y_d - 1) * sz_des.cx + x_d; \
			if (pix_img[i_d] != ExRGBA(EXP_CM, EXP_CM, EXP_CM, EXP_CM)) \
				pix_des[i_d] = pixSrc; \
		} \
	}
//#define EndDraw

	// 画线
	EXP_INLINE static bool Line(image_t imgDes, const CLine& lnDes, const pixel_t pixSrc)
	{
		PreDraw();
		MoveToEx(grp, lnDes.pt1.x, lnDes.pt1.y, &POINT());
		LineTo(grp, lnDes.pt2.x, lnDes.pt2.y);
		EndDraw()
		return true;
	}

#pragma pop_macro("EndDraw")
#pragma pop_macro("PreDraw")
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ImgDrawer_h__*/