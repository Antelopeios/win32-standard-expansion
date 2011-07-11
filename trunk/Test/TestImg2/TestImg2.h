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
	//CRect rc_src(ptSrc.x, ptSrc.y, exp_src.GetWidth(), exp_src.GetHeight());

	// »æÍ¼

	//CGraph grp_des;
	//grp_des.Create();
	//grp_des.SetObject(exp_des.Get());
	//CGraph grp_src;
	//grp_src.Create();
	//grp_src.SetObject(exp_src.Get());

	//BLENDFUNCTION bl = {0};
	//bl.AlphaFormat = AC_SRC_OVER;
	//bl.SourceConstantAlpha = 255;
	//::AlphaBlend(grp_des, rcDes.Left(), rcDes.Top(), min(rcDes.Width(), rc_src.Width()), min(rcDes.Height(), rc_src.Height()), 
	//			 grp_src, ptSrc.x, ptSrc.y, min(rcDes.Width(), rc_src.Width()), min(rcDes.Height(), rc_src.Height()), bl);

	//grp_src.Delete();
	//grp_des.Delete();


	// ±éÀúÏñËØ»æÍ¼
	pixel_t* pix_des = exp_des.GetPixels();
	pixel_t* pix_src = exp_src.GetPixels();
	for(LONG y_s = ptSrc.y; y_s < min(sz_src.cy, ptSrc.y + rcDes.Height()); ++y_s)
	{
		for(LONG x_s = ptSrc.x; x_s < min(sz_src.cx, ptSrc.x + rcDes.Width()); ++x_s)
		{
			// Ð£ÑéÏñËØÇøÓò
			LONG x_d = rcDes.Left() + x_s - ptSrc.x;
			if (x_d < 0) continue;
			if (x_d >= sz_des.cx) break;
			LONG y_d = rcDes.Top() + y_s - ptSrc.y;
			if (y_d < 0) continue;
			if (y_d >= sz_des.cy) break;
			// äÖÈ¾ÏñËØ
		/*	LONG i_d = (sz_des.cy - y_d - 1) * sz_des.cx + x_d;
			LONG i_s = (sz_src.cy - y_s - 1) * sz_src.cx + x_s;
			DWORD* p_des = pix_des + i_d;
			DWORD p_src = pix_src[i_s];
			BYTE a_s = ExGetA(pix_src[i_s]);
			__asm
			{
				pxor		mm2,	mm2
				mov			edx,	p_des
				movd		mm0,	[edx]
				movd		mm1,	p_src
				punpcklbw	mm0,	mm2
				punpcklbw	mm1,	mm2
				movq		mm3,	mm1
				punpckhwd	mm3,	mm3
				punpckhdq	mm3,	mm3
				movd		mm4,	a_s
				punpcklwd	mm4,	mm4
				punpckldq	mm4,	mm4
				pmullw		mm3,	mm4		// Alpha * SrcAlpha
				psrlw		mm3,	8
				movq		mm4,	mm0
				movq		mm5,	mm1
				psubusw		mm4,	mm1
				psubusw		mm5,	mm0
				pmullw		mm4,	mm3
				pmullw		mm5,	mm3
				psrlw		mm4,	8
				psrlw		mm5,	8
				paddusw		mm0,	mm5
				psubusw		mm0,	mm4
				packuswb	mm0,	mm0
				movd		[edx],	mm0		// ±£´æ½á¹û
				emms
			}*/

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