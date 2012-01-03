#pragma once

#include "resource.h"

#include <xmmintrin.h>
#include <emmintrin.h>

BOOL Render(image_t imgDes, image_t imgSrc, CRect& rc_des, CPoint& pt_src)
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
	if (rc_des.IsEmpty())
		rc_des.Set(CPoint(), CPoint(sz_des.cx, sz_des.cy));
	CRect rc_src(pt_src.x, pt_src.y, sz_src.cx, sz_src.cy);
	// 校验rc_des.pt1
	if (rc_des.pt1.x < 0)
	{
		rc_src.pt1.x -= rc_des.pt1.x;
		rc_des.pt1.x = 0;
	}
	if (rc_des.pt1.y < 0)
	{
		rc_src.pt1.y -= rc_des.pt1.y;
		rc_des.pt1.y = 0;
	}
	// 校验rc_des.pt2
	if (rc_des.pt2.x > sz_des.cx)
		rc_des.pt2.x = sz_des.cx;
	if (rc_des.pt2.y > sz_des.cy)
		rc_des.pt2.y = sz_des.cy;
	// 校验rc_src.pt1
	if (rc_src.pt1.x < 0)
	{
		rc_des.pt1.x -= rc_src.pt1.x;
		rc_src.pt1.x = 0;
	}
	if (rc_src.pt1.y < 0)
	{
		rc_des.pt1.y -= rc_src.pt1.y;
		rc_src.pt1.y = 0;
	}
	// rc_src.pt2不需要校验

	// 获得合适的宽与高
	LONG w = min(rc_des.Width(), rc_src.Width());
	LONG h = min(rc_des.Height(), rc_src.Height());
	if (w <= 0 || h <= 0) return TRUE;
	// 计算坐标起点
	LONG inx_des = (sz_des.cy - rc_des.Top() - 1) * sz_des.cx + rc_des.Left();
	LONG inx_src = (sz_src.cy - rc_src.Top() - 1) * sz_src.cx + rc_src.Left();

	// 遍历像素绘图
	pixel_t* pix_src = exp_src.GetPixels();
	pixel_t* pix_des = exp_des.GetPixels();
	for(LONG y = 0; y < h; ++y, inx_des -= sz_des.cx, inx_src -= sz_src.cx)
	{
		for(LONG x = 0, i_d = inx_des, i_s = inx_src; x < w; ++x, ++i_d, ++i_s)
		{
			chann_t a_s = ExGetA(pix_src[i_s]);
			if (a_s == EXP_CM)
			{
				pix_des[i_d] = pix_src[i_s];
				continue;
			}
			chann_t a_i = EXP_CM - a_s;
			int tmp;
			chann_t a = a_s + (tmp = (ExGetA(pix_des[i_d]) * a_i), ExDivCM(tmp));
			chann_t r = ExGetR(pix_src[i_s]) + (tmp = (ExGetR(pix_des[i_d]) * a_i), ExDivCM(tmp));
			chann_t g = ExGetG(pix_src[i_s]) + (tmp = (ExGetG(pix_des[i_d]) * a_i), ExDivCM(tmp));
			chann_t b = ExGetB(pix_src[i_s]) + (tmp = (ExGetB(pix_des[i_d]) * a_i), ExDivCM(tmp));
			pix_des[i_d] = ExRGBA(r, g, b, a);

			//int tmp = 0;
			//chann_t a_s = ExGetA(pix_src[i_s]);
			//if (a_s == 0)
			//	continue;
			//else
			//if (a_s == EXP_CM) // => m_Alpha == EXP_CM && a_s == EXP_CM
			//{
			//	pix_des[i_d] = pix_src[i_s];
			//	continue;
			//}
			//chann_t r_s = ExGetR(pix_src[i_s]);
			//chann_t g_s = ExGetG(pix_src[i_s]);
			//chann_t b_s = ExGetB(pix_src[i_s]);
			//chann_t a_d = ExGetA(pix_des[i_d]);
			//chann_t r_d = ExGetR(pix_des[i_d]);
			//chann_t g_d = ExGetG(pix_des[i_d]);
			//chann_t b_d = ExGetB(pix_des[i_d]);
			//chann_t a_i = EXP_CM - a_s;
			//chann_t a_r = (a_d + a_s) - (tmp = a_d * a_s, ExDivCM(tmp));
			//chann_t r = (tmp = (r_s * a_s + r_d * a_i), ExDivCM(tmp));
			//chann_t g = (tmp = (g_s * a_s + g_d * a_i), ExDivCM(tmp));
			//chann_t b = (tmp = (b_s * a_s + b_d * a_i), ExDivCM(tmp));
			//pix_des[i_d] = ExRGBA(r, g, b, a_r);
		}
	}

	return TRUE;
}