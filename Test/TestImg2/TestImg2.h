#pragma once

#include "resource.h"

bool Render(image_t imgDes, image_t imgSrc, CRect& rcDes, CPoint& ptSrc)
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
	if (rcDes.IsEmpty())
		rcDes.Set(CPoint(), CPoint(sz_des.cx, sz_des.cy));
	// ±éÀúÏñËØ»æÍ¼
	pixel_t* pix_des = exp_des.GetPixels();
	pixel_t* pix_src = exp_src.GetPixels();
	for(LONG y_s = ptSrc.y; y_s < sz_src.cy; ++y_s)
	{
		for(LONG x_s = ptSrc.x; x_s < sz_src.cx; ++x_s)
		{
			// Ð£ÑéÏñËØÇøÓò
			if (x_s >= ptSrc.x + rcDes.Width() || 
				y_s >= ptSrc.y + rcDes.Height()) continue;
			LONG x_d = rcDes.Left() + x_s - ptSrc.x;
			if (x_d < 0) continue;
			if (x_d >= sz_des.cx) break;
			LONG y_d = rcDes.Top() + y_s - ptSrc.y;
			if (y_d < 0) continue;
			if (y_d >= sz_des.cy) break;
			// äÖÈ¾ÏñËØ
			LONG i_d = (sz_des.cy - y_d - 1) * sz_des.cx + x_d;
			LONG i_s = (sz_src.cy - y_s - 1) * sz_src.cx + x_s;
			BYTE a_s = ExGetA(pix_src[i_s]);
			if (a_s == 0) continue;
			if (a_s == EXP_CM)
			{
				pix_des[i_d] = pix_src[i_s];
				continue;
			}
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
				(r_s * a_s + r_d * a_i) / EXP_CM, 
				(g_s * a_s + g_d * a_i) / EXP_CM, 
				(b_s * a_s + b_d * a_i) / EXP_CM, 
				(a_d + a_s) - a_d * a_s / EXP_CM
				);
		}
	}
	return true;
}