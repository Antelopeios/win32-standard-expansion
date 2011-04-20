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
// ImgDeformer - 图像变换器
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-11
// Version:	1.0.0000.1100
//////////////////////////////////////////////////////////////////

#ifndef __ImgDeformer_h__
#define __ImgDeformer_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgCommon/ImgCommon.h"
#include "ImgTypes/Types/Types.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CImgDeformer
{
public:
	// 差值回调指针定义
	typedef pixel_t (*inter_proc_t)(pixel_t*, CPointT<double>&, CRectT<double>&);
	// 最邻近插值
	static pixel_t Neighbor(pixel_t* pixSrc, CPointT<double>& ptSrc, CRectT<double>& rcSrc)
	{
		if (!pixSrc) return 0;
		if (!rcSrc.PtInRect(ptSrc)) return 0;
		return pixSrc[((LONG)ptSrc.x + (LONG)ptSrc.y * (LONG)rcSrc.Width())];
	}
	// 双线性插值
	static pixel_t Bilinear(pixel_t* pixSrc, CPointT<double>& ptSrc, CRectT<double>& rcSrc)
	{
		if (!pixSrc) return 0;
		if (!rcSrc.PtInRect(ptSrc)) return 0;
		// 计算待插值的4个顶点
		LONG lx = (LONG)ptSrc.x;
		LONG ly = (LONG)ptSrc.y;
		double dx = ptSrc.x - lx;
		double dy = ptSrc.y - ly;
		if (dx < 0.00000001) dx = 0;
		if (dy < 0.00000001) dy = 0;
		LONG w = (LONG)rcSrc.Width(), h = (LONG)rcSrc.Height();
		LONG inx[4] = {0};
		inx[0] = lx + ly * w;
		inx[1] = (lx >= (w - 1)) ? inx[0] : ((dx == 0) ? inx[0] : inx[0] + 1);
		inx[2] = (ly >= (h - 1)) ? inx[0] : ((dy == 0) ? inx[0] : inx[0] + w);
		inx[3] = (lx >= (w - 1)) ? inx[2] : ((dx == 0) ? inx[2] : inx[2] + 1);
		pixel_t pix[4] = 
		{
			pixSrc[inx[0]], 
			pixSrc[inx[1]], 
			pixSrc[inx[2]], 
			pixSrc[inx[3]]
		};
		// 求解插值
		double mx = 1 - dx, my = 1 - dy;
		double d[4] = 
		{
			mx * my, 
			mx * dy, 
			dx * my, 
			dx * dy
		};
		return ExRGBA
			(
			(d[0] * ExGetR(pix[0]) + d[1] * ExGetR(pix[2]) + d[2] * ExGetR(pix[1]) + d[3] * ExGetR(pix[3])), 
			(d[0] * ExGetG(pix[0]) + d[1] * ExGetG(pix[2]) + d[2] * ExGetG(pix[1]) + d[3] * ExGetG(pix[3])), 
			(d[0] * ExGetB(pix[0]) + d[1] * ExGetB(pix[2]) + d[2] * ExGetB(pix[1]) + d[3] * ExGetB(pix[3])), 
			(d[0] * ExGetA(pix[0]) + d[1] * ExGetA(pix[2]) + d[2] * ExGetA(pix[1]) + d[3] * ExGetA(pix[3]))
			);
	}

public:
	// 坐标变换
	EXP_INLINE static void Transform(_IN_ CPointT<double>& ptSrc, _OT_ CPointT<double>& ptDes, 
									 _IN_ const CPoint (&verSrc)[3], const double (&disSrc)[4], 
									 _IN_ const CPoint (&verDes)[3], const double (&disDes)[4])
	{
		CLineT<double> h_src(ptSrc, ptSrc + CPointT<double>(1, 0));
		CLineT<double> v_src(ptSrc, ptSrc + CPointT<double>(0, 1));
		CLineT<double> t_src(CPointT<double>(0, 0), verSrc[0]);
		CLineT<double> r_src(verSrc[0], verSrc[1]);
		CLineT<double> b_src(verSrc[1], verSrc[2]);
		CLineT<double> l_src(verSrc[2], CPointT<double>(0, 0));
	}

public:
	// 任意四边形变换
	EXP_INLINE static image_t RndDeform(_IN_ image_t imgSrc, _IN_ const CPoint (&ptVer)[3], inter_proc_t interProc = Bilinear)
	{
		CImage exp_src(imgSrc);
		if (exp_src.IsNull()) return NULL;
		// 
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ImgDeformer_h__*/