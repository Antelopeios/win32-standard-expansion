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
// ImgRenderer - 图像渲染器
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-26
// Version:	1.0.0006.1130
//
// History:
//	- 1.0.0001.1730(2011-04-27)	= 将渲染器内部的渲染回调指针改为滤镜接口
//								+ 添加一些滤镜效果类
//	- 1.0.0002.1410(2011-04-28)	+ 添加像素直接拷贝与高斯模糊滤镜
//								^ 将滤镜功能类改为类模板,支持外部动态调整渲染基
//	- 1.0.0003.1643(2011-04-29)	# 修正高斯模糊导致图像变暗与对比度失真等问题
//								= 调整默认高斯模糊半径为5
//	- 1.0.0004.2350(2011-05-01)	# 修正正常渲染滤镜忽略了背景Alpha通道,导致图像混合后不再透明的问题
//								# 修正因滤镜像素块采集算法在图像边缘处忽略了无法采集的部分,导致一些滤镜边缘渲染的异常
//								+ 添加外发光滤镜
//	- 1.0.0005.1330(2011-05-02)	= 将滤镜对象从ImgRenderer内部分离
//	- 1.0.0006.1130(2011-05-26)	# 修正ImgRenderer::Render()在绘图时顶点坐标偏移的计算错误
//////////////////////////////////////////////////////////////////

#ifndef __ImgRenderer_h__
#define __ImgRenderer_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgDrawer/FilterObject.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CImgRenderer
{
protected:
	EXP_INLINE static void GetFilterBlock(pixel_t* pixSrc, LONG nX, LONG nY, LONG nW, LONG nH, CRect& rcSrc, 
										  pixel_t* pixBlk, LONG nFltSiz, LONG nDiamet, LONG nFltRad)
	{
		// 清空像素内存块
		ZeroMemory(pixBlk, nFltSiz * sizeof(pixel_t));
		// 计算坐标
		LONG x_1 = nX - nFltRad, x_2 = nX + nFltRad;
		LONG y_1 = nY - nFltRad, y_2 = nY + nFltRad;
		LONG flt = nDiamet * sizeof(pixel_t);
		LONG x_b = max(x_1, rcSrc.Left()), x_e = min(x_2, rcSrc.Right() - 1);
		LONG y_b = max(y_1, rcSrc.Top()), y_e = min(y_2, rcSrc.Bottom() - 1);
		LONG dif_x = x_e - x_b + 1, dif_y = y_e - y_b + 1;
		LONG dif_flt = dif_x * sizeof(pixel_t);
		LONG dif_x_b = x_b - x_1, dif_y_b = y_b - y_1;
		// 判断是否对齐
		if (dif_x < nDiamet || dif_y < nDiamet)
		{
			// 重新对齐坐标
			if (x_b > x_1) { x_1 = x_b; x_2 = x_1 + nDiamet - 1; }
			else
			if (x_e < x_2) { x_2 = x_e; x_1 = x_2 - nDiamet + 1; }
			if (y_b > y_1) { y_1 = y_b; y_2 = y_1 + nDiamet - 1; }
			else
			if (y_e < y_2) { y_2 = y_e; y_1 = y_2 - nDiamet + 1; }
			// 覆盖对齐像素
			for(LONG n = y_1; n <= y_2; ++n)
			{
				LONG i_pix = (nH - n - 1) * nW + x_1;
				LONG i_flt = (nDiamet - (n - y_1) - 1) * nDiamet;
				memcpy(pixBlk + i_flt, pixSrc + i_pix, flt);
			}
		}
		// 覆盖目标像素
		for(LONG n = y_b; n <= y_e; ++n)
		{
			LONG i_pix = (nH - n - 1) * nW + x_b;
			LONG i_flt = (nDiamet - (dif_y_b + n - y_b) - 1) * nDiamet + dif_x_b;
			memcpy(pixBlk + i_flt, pixSrc + i_pix, dif_flt);
		}
	}

public:
	EXP_INLINE static bool Render(image_t imgDes, image_t imgSrc, CRect& rcDes, CPoint& ptSrc, 
								  IFilterObject* pFilter = NULL)
	{
		CImage exp_des(imgDes);
		if (exp_des.IsNull()) return false;
		CImage exp_src(imgSrc);
		if (exp_src.IsNull()) return false;
		CSize sz_des(exp_des.GetWidth(), exp_des.GetHeight());
		CSize sz_src(exp_src.GetWidth(), exp_src.GetHeight());
		if (rcDes.IsEmpty())
			rcDes.Set(CPoint(), CPoint(sz_des.cx, sz_des.cy));
		// 遍历像素绘图
		if (!pFilter) pFilter = &CFilterNormal();
		pFilter->SetSize(sz_des.cx, sz_des.cy);
		LONG radius = pFilter->GetRadius();
		if (radius <= 0) return false;
		LONG diamet = (radius << 1) - 1;
		if (diamet > sz_des.cx || diamet > sz_des.cy || 
			diamet > sz_src.cx || diamet > sz_src.cy) return false;
		LONG fltsiz = diamet * diamet;
		LONG fltrad = radius - 1;
		LONG fltind = diamet * fltrad + fltrad;
		pixel_t* flt_src = ExMem::Alloc<pixel_t>(fltsiz); // 滤镜像素块
		pixel_t* pix_des = exp_des.GetPixels();
		pixel_t* pix_src = exp_src.GetPixels();
		for(LONG y_s = ptSrc.y; y_s < sz_src.cy; ++y_s)
		{
			for(LONG x_s = ptSrc.x; x_s < sz_src.cx; ++x_s)
			{
				// 校验像素区域
				if (x_s >= ptSrc.x + rcDes.Width() || 
					y_s >= ptSrc.y + rcDes.Height()) continue;
				LONG x_d = rcDes.Left() + x_s - ptSrc.x;
				if (x_d < 0) continue;
				if (x_d >= sz_des.cx) break;
				LONG y_d = rcDes.Top() + y_s - ptSrc.y;
				if (y_d < 0) continue;
				if (y_d >= sz_des.cy) break;
				// 获得像素块
				CRect rc(ptSrc, CPoint(sz_src.cx, sz_src.cy));
				GetFilterBlock
					(
					pix_src, x_s, y_s, sz_src.cx, sz_src.cy, rc, 
					flt_src, fltsiz, diamet, fltrad
					);
				// 渲染像素
				LONG i_d = (sz_des.cy - y_d - 1) * sz_des.cx + x_d;
				pix_des[i_d] = pFilter->Render(flt_src, pix_des[i_d], fltind);
			}
		}
		ExMem::Free(flt_src);
		return true;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ImgRenderer_h__*/