// Copyright 2011, ľͷ��
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
// ImgRenderer - ͼ����Ⱦ��
//
// Author:	ľͷ��
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-26
// Version:	1.0.0006.1130
//
// History:
//	- 1.0.0001.1730(2011-04-27)	= ����Ⱦ���ڲ�����Ⱦ�ص�ָ���Ϊ�˾��ӿ�
//								+ ���һЩ�˾�Ч����
//	- 1.0.0002.1410(2011-04-28)	+ �������ֱ�ӿ������˹ģ���˾�
//								^ ���˾��������Ϊ��ģ��,֧���ⲿ��̬������Ⱦ��
//	- 1.0.0003.1643(2011-04-29)	# ������˹ģ������ͼ��䰵��Աȶ�ʧ�������
//								= ����Ĭ�ϸ�˹ģ���뾶Ϊ5
//	- 1.0.0004.2350(2011-05-01)	# ����������Ⱦ�˾������˱���Alphaͨ��,����ͼ���Ϻ���͸��������
//								# �������˾����ؿ�ɼ��㷨��ͼ���Ե���������޷��ɼ��Ĳ���,����һЩ�˾���Ե��Ⱦ���쳣
//								+ ����ⷢ���˾�
//	- 1.0.0005.1330(2011-05-02)	= ���˾������ImgRenderer�ڲ�����
//	- 1.0.0006.1130(2011-05-26)	# ����ImgRenderer::Render()�ڻ�ͼʱ��������ƫ�Ƶļ������
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
		// ��������ڴ��
		ZeroMemory(pixBlk, nFltSiz * sizeof(pixel_t));
		// ��������
		LONG x_1 = nX - nFltRad, x_2 = nX + nFltRad;
		LONG y_1 = nY - nFltRad, y_2 = nY + nFltRad;
		LONG flt = nDiamet * sizeof(pixel_t);
		LONG x_b = max(x_1, rcSrc.Left()), x_e = min(x_2, rcSrc.Right() - 1);
		LONG y_b = max(y_1, rcSrc.Top()), y_e = min(y_2, rcSrc.Bottom() - 1);
		LONG dif_x = x_e - x_b + 1, dif_y = y_e - y_b + 1;
		LONG dif_flt = dif_x * sizeof(pixel_t);
		LONG dif_x_b = x_b - x_1, dif_y_b = y_b - y_1;
		// �ж��Ƿ����
		if (dif_x < nDiamet || dif_y < nDiamet)
		{
			// ���¶�������
			if (x_b > x_1) { x_1 = x_b; x_2 = x_1 + nDiamet - 1; }
			else
			if (x_e < x_2) { x_2 = x_e; x_1 = x_2 - nDiamet + 1; }
			if (y_b > y_1) { y_1 = y_b; y_2 = y_1 + nDiamet - 1; }
			else
			if (y_e < y_2) { y_2 = y_e; y_1 = y_2 - nDiamet + 1; }
			// ���Ƕ�������
			for(LONG n = y_1; n <= y_2; ++n)
			{
				LONG i_pix = (nH - n - 1) * nW + x_1;
				LONG i_flt = (nDiamet - (n - y_1) - 1) * nDiamet;
				memcpy(pixBlk + i_flt, pixSrc + i_pix, flt);
			}
		}
		// ����Ŀ������
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
		// �������ػ�ͼ
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
		pixel_t* flt_src = ExMem::Alloc<pixel_t>(fltsiz); // �˾����ؿ�
		pixel_t* pix_des = exp_des.GetPixels();
		pixel_t* pix_src = exp_src.GetPixels();
		for(LONG y_s = ptSrc.y; y_s < sz_src.cy; ++y_s)
		{
			for(LONG x_s = ptSrc.x; x_s < sz_src.cx; ++x_s)
			{
				// У����������
				if (x_s >= ptSrc.x + rcDes.Width() || 
					y_s >= ptSrc.y + rcDes.Height()) continue;
				LONG x_d = rcDes.Left() + x_s - ptSrc.x;
				if (x_d < 0) continue;
				if (x_d >= sz_des.cx) break;
				LONG y_d = rcDes.Top() + y_s - ptSrc.y;
				if (y_d < 0) continue;
				if (y_d >= sz_des.cy) break;
				// ������ؿ�
				CRect rc(ptSrc, CPoint(sz_src.cx, sz_src.cy));
				GetFilterBlock
					(
					pix_src, x_s, y_s, sz_src.cx, sz_src.cy, rc, 
					flt_src, fltsiz, diamet, fltrad
					);
				// ��Ⱦ����
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