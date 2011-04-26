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
// Date:	2011-04-11
// Version:	1.0.0000.1100
//////////////////////////////////////////////////////////////////

#ifndef __ImgRenderer_h__
#define __ImgRenderer_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgDeformer/ImgDeformer.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CImgRenderer
{
public:
	// 渲染回调指针定义
	typedef pixel_t (*render_proc_t)(pixel_t, pixel_t);
	// 常规渲染
	static pixel_t RenderNormal(pixel_t pixSrc, pixel_t pixDes)
	{
		int a = ExGetA(pixSrc);
		if (a == 0) return pixDes;
		if (a == (BYTE)~0) return pixSrc;
		int r_d = ExGetR(pixDes);
		int g_d = ExGetG(pixDes);
		int b_d = ExGetB(pixDes);
		int r_s = (ExGetR(pixSrc) << 8) / a;
		int g_s = (ExGetG(pixSrc) << 8) / a;
		int b_s = (ExGetB(pixSrc) << 8) / a;
		pixDes = ExRGBA
			(
			(((r_s - r_d) * a) >> 8) + r_d, 
			(((g_s - g_d) * a) >> 8) + g_d, 
			(((b_s - b_d) * a) >> 8) + b_d, 
			(BYTE)~0
			);
		return pixDes;
	}

public:
	EXP_INLINE static bool ImgRender(graph_t grpDes, image_t imgSrc, CRect& rcDes, render_proc_t renderProc = RenderNormal)
	{
		CGraph exp_grp(grpDes);
		if (exp_grp.IsNull()) return false;
		CImage exp_des((image_t)exp_grp.GetObject(OBJ_BITMAP));
		if (exp_des.IsNull()) return false;
		CImage exp_src(imgSrc);
		if (exp_src.IsNull()) return false;
		// 遍历像素绘图
		if (!renderProc) renderProc = RenderNormal;
		pixel_t* pix_src = exp_src.GetPixels();
		pixel_t* pix_des = exp_des.GetPixels();
		for(long y_s = 0; y_s < (long)exp_src.GetHeight(); ++y_s)
		{
			for(long x_s = 0; x_s < (long)exp_src.GetWidth(); ++x_s)
			{
				// 校验像素区域
				long x_d = rcDes.Left() + x_s;
				if (x_d < 0) continue;
				if (x_d >= (long)exp_des.GetWidth()) break;
				long y_d = rcDes.Top() + y_s;
				if (y_d < 0) continue;
				if (y_d >= (long)exp_des.GetHeight()) break;
				// 获得像素索引
				long i_s = x_s + exp_src.GetWidth() * y_s;
				long i_d = x_d + exp_des.GetWidth() * y_d;
				// 渲染像素
				pix_des[i_d] = renderProc(pix_src[i_s], pix_des[i_d]);
			}
		}
		return true;
	}
	//EXP_INLINE static bool PlgRender(graph_t grpDes, image_t imgSrc, const CPoint (&ptVer)[2], 
	//								 render_proc_t renderProc = RenderNormal, 
	//								 CImgDeformer::inter_proc_t interProc = CImgDeformer::InterBilinear)
	//{
	//}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ImgRenderer_h__*/