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
// ImgDrawer - 基本图形图像绘制
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-04-22
// Version:	1.0.0007.1042
//
// History:
//	- 1.0.0000.1627(2011-06-20)	@ 重新构建ImgDrawer,作为ImgPainter的子模块,用于绘制基本图形
//	- 1.0.0001.1942(2011-07-16)	# 修正当CImgDrawer::Fill()传入的CRect参数超出image_t界限时导致的访问异常
//	- 1.0.0002.1747(2011-07-20)	# 修正CImgDrawer::Fill()像素循环遍历中的一个低级错误
//	- 1.0.0003.1540(2011-08-10)	+ 添加CImgDrawer::Fill()的精简参数重载
//	- 1.0.0004.1646(2012-01-04)	+ CImgDrawer添加Cover()与Draw()系列接口,用于方便绘图调用
//	- 1.0.0005.2325(2012-03-16)	+ CImgDrawer支持通过CGraph进行带独立剪切区与坐标的图像绘制
//	- 1.0.0006.1142(2012-03-28)	+ CImgDrawer::Line()支持通过CGraph进行带独立坐标的图像绘制
//	- 1.0.0007.1042(2012-04-22)	# 修正CImgDrawer::Fill()可能超出CGraph的限定范围进行颜色填充的bug
//////////////////////////////////////////////////////////////////

#ifndef __ImgDrawer_h__
#define __ImgDrawer_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgPainter/ImgFilter.h"
#include "ImgPainter/ImgRenderer.h"
#include "ImgTypes/ImgTypes.h"

#pragma comment(lib, "msimg32.lib")

EXP_BEG

//////////////////////////////////////////////////////////////////

#pragma push_macro("PreDraw")
#undef PreDraw
#define PreDraw() \
	CImage exp_des; \
	exp_des.SetTrust(FALSE); \
	exp_des = imgDes; \
	if (exp_des.IsNull()) return FALSE; \
	CImgASM::PixPreMul(&pixSrc, 1); \
	CSize sz_des(exp_des.GetWidth(), exp_des.GetHeight()); \
	CImage img; \
	img.Create(sz_des.cx, sz_des.cy); \
	Fill(img, CRect(), ExRGBA(EXP_CM, EXP_CM, EXP_CM, EXP_CM)); \
	CDC dc; \
	dc.Create(); \
	dc.SetObject(img)
//#define PreDraw
#pragma push_macro("EndDraw")
#undef EndDraw
#define EndDraw() \
	dc.Delete(); \
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

//////////////////////////////////////////////////////////////////

class CImgDrawer
{
public:
	// 画点
	EXP_INLINE static BOOL Point(const CGraph& imgDes, const CPoint& ptDes, pixel_t pixSrc)
	{
		CPoint pt_des(ptDes);
		imgDes.Transform(pt_des);
		CImage exp_des;
		exp_des.SetTrust(FALSE);
		exp_des = imgDes;
		if (exp_des.IsNull()) return FALSE;
		CImgASM::PixPreMul(&pixSrc, 1);
		CSize sz_des(exp_des.GetWidth(), exp_des.GetHeight());
		pixel_t* pix_des = exp_des.GetPixels();
		if (pt_des.x >= sz_des.cx || pt_des.y >= sz_des.cy) return TRUE;
		LONG i_d = (sz_des.cy - pt_des.y - 1) * sz_des.cx + pt_des.x;
		pix_des[i_d] = pixSrc;
		return TRUE;
	}

	// 画线
	EXP_INLINE static BOOL Line(const CGraph& imgDes, const CLine& lnDes, pixel_t pixSrc)
	{
		CLine ln_des(lnDes);
		imgDes.Transform(ln_des);
		PreDraw();
		MoveToEx(dc, ln_des.pt1.x, ln_des.pt1.y, &POINT());
		LineTo(dc, ln_des.pt2.x, ln_des.pt2.y);
		EndDraw()
		return TRUE;
	}

	// 填充
	EXP_INLINE static BOOL Fill(const CGraph& imgDes, const CRect& rcDes, pixel_t pixSrc)
	{
		CRect rc_des(rcDes);
		if (rc_des.IsNull())
			rc_des = imgDes.GetRect();
		imgDes.Transform(rc_des);
		if (rc_des.Left() < imgDes.GetRect().Left())
			rc_des.Left(imgDes.GetRect().Left());
		if (rc_des.Top() < imgDes.GetRect().Top())
			rc_des.Top(imgDes.GetRect().Top());
		if (rc_des.Right() > imgDes.GetRect().Right())
			rc_des.Right(imgDes.GetRect().Right());
		if (rc_des.Bottom() > imgDes.GetRect().Bottom())
			rc_des.Bottom(imgDes.GetRect().Bottom());
		return CImgFilter::Filter(imgDes, rc_des, &CFilterFill(pixSrc));
	}
	EXP_INLINE static BOOL Fill(const CGraph& imgDes, pixel_t pixSrc)
	{
		return Fill(imgDes, CRect(), pixSrc);
	}

	// 覆盖
	EXP_INLINE static BOOL Cover(const CGraph& imgDes, image_t imgSrc, 
		const CRect& rcDes = CRect(), const CPoint& ptSrc = CPoint(), chann_t cAlpha = EXP_CM)
	{
		CRect rc_des(rcDes);
		if (rc_des.IsNull())
			rc_des = imgDes.GetRect();
		imgDes.Transform(rc_des);
		return CImgRenderer::Render(imgDes, imgSrc, rc_des, ptSrc, &CRenderCopy(cAlpha));
	}
	EXP_INLINE static BOOL Cover(const CGraph& imgDes, image_t imgSrc, chann_t cAlpha)
	{
		return Cover(imgDes, imgSrc, CRect(), CPoint(), cAlpha);
	}

	// 画图
	EXP_INLINE static BOOL Draw(const CGraph& imgDes, image_t imgSrc, 
		const CRect& rcDes, const CRect& rcSrc, chann_t cAlpha = EXP_CM)
	{
		CImage exp_des;
		exp_des.SetTrust(FALSE);
		exp_des = imgDes;
		if (exp_des.IsNull()) return FALSE;
		CImage exp_src;
		exp_src.SetTrust(FALSE);
		exp_src = imgSrc;
		if (exp_src.IsNull()) return FALSE;
		// 格式化区域
		CSize sz_des(exp_des.GetWidth(), exp_des.GetHeight());
		CSize sz_src(exp_src.GetWidth(), exp_src.GetHeight());
		CRect rc_des(rcDes);
		if (rc_des.IsNull())
			rc_des.Set(CPoint(), CPoint(sz_des.cx, sz_des.cy));
		imgDes.Transform(rc_des);
		CRect rc_src(rcSrc);
		if (rc_src.IsNull())
			rc_src.Set(CPoint(), CPoint(sz_src.cx, sz_src.cy));
		// 校验rc_src
		if (rc_src.Left() < 0)
		{
			rc_des.Left(rc_des.Left() - rc_src.Left() * rc_des.Width() / rc_src.Width());
			rc_src.Left(0);
		}
		if (rc_src.Right() > sz_src.cx)
		{
			rc_des.Right(rc_des.Right() - (rc_src.Right() - sz_src.cx) * rc_des.Width() / rc_src.Width());
			rc_src.Right(sz_src.cx);
		}
		if (rc_src.Top() < 0)
		{
			rc_des.Top(rc_des.Top() - rc_src.Top() * rc_des.Height() / rc_src.Height());
			rc_src.Top(0);
		}
		if (rc_src.Bottom() > sz_src.cy)
		{
			rc_des.Bottom(rc_des.Bottom() - (rc_src.Bottom() - sz_src.cy) * rc_des.Height() / rc_src.Height());
			rc_src.Bottom(sz_src.cy);
		}
		// 开始绘图
		CDC dc_des;
		dc_des.Create();
		dc_des.SetObject(imgDes);
		dc_des.SetClipBox(imgDes.GetRect());
		CDC dc_src;
		dc_src.Create();
		dc_src.SetObject(imgSrc);
		BLENDFUNCTION bl = {0};
		bl.AlphaFormat = AC_SRC_ALPHA;
		bl.SourceConstantAlpha = cAlpha;
		BOOL ret = AlphaBlend(dc_des, rc_des.Left(), rc_des.Top(), rc_des.Width(), rc_des.Height(), 
							  dc_src, rc_src.Left(), rc_src.Top(), rc_src.Width(), rc_src.Height(), bl);
		dc_src.Delete();
		dc_des.Delete();
		return ret;
	}
	EXP_INLINE static BOOL Draw(const CGraph& imgDes, image_t imgSrc, 
		const CRect& rcDes, const CPoint& ptSrc, const CSize& szSrc/*用于形变的宽/高*/, chann_t cAlpha = EXP_CM)
	{
		// 格式化区域
		CRect rc_des(rcDes);
		if (rc_des.IsNull())
		{
			CImage exp_des;
			exp_des.SetTrust(FALSE);
			exp_des = imgDes;
			if (exp_des.IsNull()) return FALSE;
			rc_des.Set(CPoint(), CPoint(exp_des.GetWidth(), exp_des.GetHeight()));
		}
		CImage exp_src;
		exp_src.SetTrust(FALSE);
		exp_src = imgSrc;
		if (exp_src.IsNull()) return FALSE;
		CRect rc_src(0, 0, 
			rc_des.Width() >= szSrc.cx ? 
			exp_src.GetWidth() : exp_src.GetWidth() * rc_des.Width() / szSrc.cx, 
			rc_des.Height() >= szSrc.cy ? 
			exp_src.GetHeight() : exp_src.GetHeight() * rc_des.Height() / szSrc.cy);
		rc_src.MoveTo(ptSrc);
		// 开始绘图
		return Draw(imgDes, imgSrc, rc_des, rc_src, cAlpha);
	}
	EXP_INLINE static BOOL Draw(const CGraph& imgDes, image_t imgSrc, 
		const CRect& rcDes = CRect(), const CPoint& ptSrc = CPoint(), chann_t cAlpha = EXP_CM)
	{
		// 格式化区域
		CRect rc_des(rcDes);
		if (rc_des.IsNull())
		{
			CImage exp_des;
			exp_des.SetTrust(FALSE);
			exp_des = imgDes;
			if (exp_des.IsNull()) return FALSE;
			rc_des.Set(CPoint(), CPoint(exp_des.GetWidth(), exp_des.GetHeight()));
		}
		CRect rc_src(rc_des);
		rc_src.MoveTo(ptSrc);
		// 开始绘图
		return Draw(imgDes, imgSrc, rc_des, rc_src, cAlpha);
	}
	EXP_INLINE static BOOL Draw(const CGraph& imgDes, image_t imgSrc, chann_t cAlpha)
	{
		return Draw(imgDes, imgSrc, CRect(), CPoint(), cAlpha);
	}
};

//////////////////////////////////////////////////////////////////

#pragma pop_macro("EndDraw")
#pragma pop_macro("PreDraw")

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ImgDrawer_h__*/