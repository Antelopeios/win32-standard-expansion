// Copyright 2011, Ä¾Í·ÔÆ
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
// ImgRenderer - Í¼ÏñäÖÈ¾Æ÷
//
// Author:	Ä¾Í·ÔÆ
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

#include "ImgDrawer/ImgDeformer.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CImgRenderer
{
public:
	// äÖÈ¾»Øµ÷Ö¸Õë¶¨Òå
	typedef pixel_t (*render_proc_t)(pixel_t, pixel_t);
	// ³£¹æäÖÈ¾
	static pixel_t RenderNormal(pixel_t pixSrc, pixel_t pixDes)
	{
		BYTE a_s = ExGetA(pixSrc);
		if (a_s == 0) return pixDes;
		if (a_s == (BYTE)~0) return pixSrc;
		BYTE r_s = ExGetR(pixSrc);
		BYTE g_s = ExGetG(pixSrc);
		BYTE b_s = ExGetB(pixSrc);
		BYTE a_d = (BYTE)~0 - a_s;
		BYTE r_d = ExGetR(pixDes);
		BYTE g_d = ExGetG(pixDes);
		BYTE b_d = ExGetB(pixDes);
		return ExRGBA
			(
			(r_s * a_s + r_d * a_d) / (BYTE)~0, 
			(g_s * a_s + g_d * a_d) / (BYTE)~0, 
			(b_s * a_s + b_d * a_d) / (BYTE)~0, 
			(BYTE)~0
			);
	}
	static pixel_t RenderGray(pixel_t pixSrc, pixel_t pixDes)
	{
		BYTE g = (ExGetR(pixSrc) * 38 + ExGetG(pixSrc) * 75 + ExGetB(pixSrc) * 15) >> 7;
		return RenderNormal(ExRGBA(g, g, g, ExGetA(pixSrc)), pixDes);
	}
	static pixel_t RenderInverse(pixel_t pixSrc, pixel_t pixDes)
	{
		return RenderNormal(
			ExRGBA
				(
				(BYTE)~0 - ExGetR(pixSrc), 
				(BYTE)~0 - ExGetG(pixSrc), 
				(BYTE)~0 - ExGetB(pixSrc), 
				ExGetA(pixSrc)
				), 
			pixDes);
	}

public:
	EXP_INLINE static bool Render(image_t imgDes, image_t imgSrc, 
								  CRect& rcDes, CPoint& ptSrc, 
								  render_proc_t renderProc = RenderNormal)
	{
		CImage exp_des(imgDes);
		if (exp_des.IsNull()) return false;
		CImage exp_src(imgSrc);
		if (exp_src.IsNull()) return false;
		if (rcDes.IsEmpty())
			rcDes.Set(CPoint(), CPoint(exp_des.GetWidth(), exp_des.GetHeight()));
		// ±éÀúÏñËØ»æÍ¼
		if (!renderProc) renderProc = RenderNormal;
		pixel_t* pix_des = exp_des.GetPixels();
		pixel_t* pix_src = exp_src.GetPixels();
		for(long y_s = 0; y_s < (long)exp_src.GetHeight(); ++y_s)
		{
			for(long x_s = 0; x_s < (long)exp_src.GetWidth(); ++x_s)
			{
				// Ð£ÑéÏñËØÇøÓò
				if (x_s < ptSrc.x || x_s >= ptSrc.x + rcDes.Width() || 
					y_s < ptSrc.y || y_s >= ptSrc.y + rcDes.Height()) continue;
				long x_d = rcDes.Left() + x_s;
				if (x_d < 0) continue;
				if (x_d >= (long)exp_des.GetWidth()) break;
				long y_d = rcDes.Top() + y_s;
				if (y_d < 0) continue;
				if (y_d >= (long)exp_des.GetHeight()) break;
				// äÖÈ¾ÏñËØ
				pixel_t* pdst = pix_des + ((exp_des.GetHeight() - y_d - 1) * exp_des.GetWidth() + x_d);
				pixel_t* psrc = pix_src + ((exp_src.GetHeight() - y_s - 1) * exp_src.GetWidth() + x_s);
				(*pdst) = renderProc((*psrc), (*pdst));
			}
		}
		return true;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ImgRenderer_h__*/