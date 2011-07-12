#pragma once

#include "resource.h"

#include <xmmintrin.h>
#include <emmintrin.h>

bool Render(image_t imgDes, image_t imgSrc, CRect& rc_des, CPoint& pt_src)
{
	CImage exp_des;
	exp_des.SetTrust(false);
	exp_des = imgDes;
	if (exp_des.IsNull()) return false;
	CImage exp_src;
	exp_src.SetTrust(false);
	exp_src = imgSrc;
	if (exp_src.IsNull()) return false;

	CSize sz_des(exp_des.GetWidth(), exp_des.GetHeight());
	CSize sz_src(exp_src.GetWidth(), exp_src.GetHeight());
	if (rc_des.IsEmpty())
		rc_des.Set(CPoint(), CPoint(sz_des.cx, sz_des.cy));

	// ±éÀúÏñËØ»æÍ¼
	pixel_t* pix_des = exp_des.GetPixels();
	pixel_t* pix_src = exp_src.GetPixels();
	LONG w = min(sz_src.cx, pt_src.x + rc_des.Width());
	LONG h = min(sz_src.cy, pt_src.y + rc_des.Height());
	LONG y_s = pt_src.y;
	LONG y_d = rc_des.Top() + y_s - pt_src.y;
	for(; y_s < h && y_d < sz_des.cy; ++y_s, ++y_d)
	{
		if (y_d < 0) continue;

		LONG x_s = pt_src.x;
		LONG x_d = rc_des.Left() + x_s - pt_src.x;
		LONG i_s = (sz_src.cy - y_s - 1) * sz_src.cx + x_s;
		LONG i_d = (sz_des.cy - y_d - 1) * sz_des.cx + x_d;
		for(; x_s < w && x_d < sz_des.cx; ++x_s, ++x_d, ++i_s, ++i_d)
		{
			if (x_d < 0) continue;

			BYTE a_s = ExGetA(pix_src[i_s]);
			if (a_s == 0)
				continue;
			else
			if (a_s == EXP_CM)
			{
				pix_des[i_d] = pix_src[i_s];
				continue;
			}
			else
			{
				BYTE r_s = ExGetR(pix_src[i_s]);
				BYTE g_s = ExGetG(pix_src[i_s]);
				BYTE b_s = ExGetB(pix_src[i_s]);
				BYTE a_d = ExGetA(pix_des[i_d]);
				BYTE r_d = ExGetR(pix_des[i_d]);
				BYTE g_d = ExGetG(pix_des[i_d]);
				BYTE b_d = ExGetB(pix_des[i_d]);
				BYTE a_i = EXP_CM - a_s;
				pix_des[i_d] = ExRGBA
					(
					(r_s * a_s + r_d * a_i) >> 8, 
					(g_s * a_s + g_d * a_i) >> 8, 
					(b_s * a_s + b_d * a_i) >> 8, 
					(a_d + a_s) - (a_d * a_s >> 8)
					);
			}
		}
	}

	return true;
}