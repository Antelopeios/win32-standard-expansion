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
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-07-12
// Version:	1.0.0009.2328
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
//	- 1.0.0007.2200(2011-07-10)	^ ���˾�ģ���ImgRenderer�з���,���ImgRenderer����ȾЧ��
//	- 1.0.0008.2350(2011-07-11)	^ ��΢�Ż�PreRender()��ִ��Ч��
//								^ �Ż���Ⱦ���㷨,���ִ��Ч��(+25%)
//	- 1.0.0009.2328(2011-07-12)	^ �Ż�PreRender()��ִ��Ч��(+40-60%)
//////////////////////////////////////////////////////////////////

#ifndef __ImgRenderer_h__
#define __ImgRenderer_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgTypes/Types/Types.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// ��Ⱦ���ӿ�
interface IRenderObject : INonCopyable
{
	virtual void Render
		(
		pixel_t* pix_des, pixel_t* pix_src, 
		CSize& sz_des, CSize& sz_src, 
		CRect& rc_des, CPoint& pt_src
		) = 0;
};

//////////////////////////////////////////////////////////////////

#pragma push_macro("PreRender")
#undef PreRender
#define PreRender() \
	LONG w = min(sz_src.cx, pt_src.x + rc_des.Width()); \
	LONG h = min(sz_src.cy, pt_src.y + rc_des.Height()); \
	LONG y_s = pt_src.y; \
	LONG y_d = rc_des.Top() + y_s - pt_src.y; \
	for(; y_s < h && y_d < sz_des.cy; ++y_s, ++y_d) \
	{ \
		if (y_d < 0) continue; \
		LONG x_s = pt_src.x; \
		LONG x_d = rc_des.Left() + x_s - pt_src.x; \
		LONG i_s = (sz_src.cy - y_s - 1) * sz_src.cx + x_s; \
		LONG i_d = (sz_des.cy - y_d - 1) * sz_des.cx + x_d; \
		for(; x_s < w && x_d < sz_des.cx; ++x_s, ++x_d, ++i_s, ++i_d) \
		{ \
			if (x_d < 0) continue
//#define PreRender

#pragma push_macro("EndRender")
#undef EndRender
#define EndRender() \
		} \
	}
//#define EndRender

//////////////////////////////////////////////////////////////////

// ���ؿ���
class CRenderCopy : public IRenderObject
{
protected:
	BYTE m_Alpha;

public:
	CRenderCopy(BYTE a = EXP_CM)
		: m_Alpha(a)
	{}

	void Render
		(
		pixel_t* pix_des, pixel_t* pix_src, 
		CSize& sz_des, CSize& sz_src, 
		CRect& rc_des, CPoint& pt_src
		)
	{
		if (m_Alpha == 0)
			return;
		else
		if (m_Alpha == EXP_CM)
		{
			PreRender();
			pix_des[i_d] = pix_src[i_s];
			EndRender();
		}
		else
		{
			PreRender();
			int r_dif = 
				(m_Alpha == EXP_CM ? 
				((int)ExGetR(pix_src[i_s]) - (int)ExGetR(pix_des[i_d])) : 
				((int)ExGetR(pix_src[i_s]) - (int)ExGetR(pix_des[i_d])) * m_Alpha >> 8);
			int g_dif = 
				(m_Alpha == EXP_CM ? 
				((int)ExGetG(pix_src[i_s]) - (int)ExGetG(pix_des[i_d])) : 
				((int)ExGetG(pix_src[i_s]) - (int)ExGetG(pix_des[i_d])) * m_Alpha >> 8);
			int b_dif = 
				(m_Alpha == EXP_CM ? 
				((int)ExGetB(pix_src[i_s]) - (int)ExGetB(pix_des[i_d])) : 
				((int)ExGetB(pix_src[i_s]) - (int)ExGetB(pix_des[i_d])) * m_Alpha >> 8);
			int a_dif = 
				(m_Alpha == EXP_CM ? 
				((int)ExGetA(pix_src[i_s]) - (int)ExGetA(pix_des[i_d])) : 
				((int)ExGetA(pix_src[i_s]) - (int)ExGetA(pix_des[i_d])) * m_Alpha >> 8);
			pix_des[i_d] = ExRGBA
				(
				ExGetR(pix_des[i_d]) + r_dif, 
				ExGetG(pix_des[i_d]) + g_dif, 
				ExGetB(pix_des[i_d]) + b_dif, 
				ExGetA(pix_des[i_d]) + a_dif
				);
			EndRender();
		}
	}
};

//////////////////////////////////////////////////////////////////

// ������Ⱦ
class CRenderNormal : public IRenderObject
{
protected:
	BYTE m_Alpha;

public:
	CRenderNormal(BYTE a = EXP_CM)
		: m_Alpha(a)
	{}

	void Render
		(
		pixel_t* pix_des, pixel_t* pix_src, 
		CSize& sz_des, CSize& sz_src, 
		CRect& rc_des, CPoint& pt_src
		)
	{
		PreRender();

		BYTE a_s = 
			(m_Alpha == EXP_CM ? ExGetA(pix_src[i_s]) : (ExGetA(pix_src[i_s]) * m_Alpha) >> 8);
		if (a_s == 0)
			continue;
		else
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
			(r_s * a_s + r_d * a_i) >> 8, 
			(g_s * a_s + g_d * a_i) >> 8, 
			(b_s * a_s + b_d * a_i) >> 8, 
			(a_d + a_s) - (a_d * a_s >> 8)
			);

		EndRender();
	}
};

#ifndef EXP_IMG_RENDER
#define EXP_IMG_RENDER CRenderNormal
#endif/*EXP_IMG_RENDER*/

//////////////////////////////////////////////////////////////////

// ͼƬ����
class CRenderOverlay : public IRenderObject
{
protected:
	BYTE m_Alpha;

public:
	CRenderOverlay(BYTE a = EXP_CM)
		: m_Alpha(a)
	{}

	void Render
		(
		pixel_t* pix_des, pixel_t* pix_src, 
		CSize& sz_des, CSize& sz_src, 
		CRect& rc_des, CPoint& pt_src
		)
	{
		PreRender();

		BYTE a_s = 
			(m_Alpha == EXP_CM ? ExGetA(pix_src[i_s]) : (ExGetA(pix_src[i_s]) * m_Alpha) >> 8);
		if (a_s == 0)
			continue;
		else
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
		LONG a_i = (EXP_CM - a_s) * a_d >> 8;
		BYTE a_r = (a_d + a_s) - (a_d * a_s >> 8);
		if (a_r == 0) a_r = EXP_CM;
		pix_des[i_d] = ExRGBA
			(
			(r_d * a_i + r_s * a_s) / a_r, 
			(g_d * a_i + g_s * a_s) / a_r, 
			(b_d * a_i + b_s * a_s) / a_r, 
			a_r
			);

		EndRender();
	}
};

//////////////////////////////////////////////////////////////////

#pragma pop_macro("EndRender")
#pragma pop_macro("PreRender")

//////////////////////////////////////////////////////////////////

class CImgRenderer
{
public:
	EXP_INLINE static bool Render(image_t imgDes, image_t imgSrc, CRect& rcDes, CPoint& ptSrc, 
								  IRenderObject* pRender = NULL)
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
		// �������ػ�ͼ
		pixel_t* pix_des = exp_des.GetPixels();
		pixel_t* pix_src = exp_src.GetPixels();
		if (!pRender) pRender = &EXP_IMG_RENDER();
		pRender->Render(pix_des, pix_src, sz_des, sz_src, rcDes, ptSrc);
		return true;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ImgRenderer_h__*/