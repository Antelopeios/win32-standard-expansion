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
// ImgFilter - 图像滤镜
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-07-12
// Version:	1.0.0001.1004
//
// History:
//	- 1.0.0000.2200(2011-07-10)	@ 开始构建ImgFilter
//	- 1.0.0001.1004(2011-07-12)	^ 优化PreFilter()的执行效率
//////////////////////////////////////////////////////////////////

#ifndef __ImgFilter_h__
#define __ImgFilter_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgTypes/Types/Types.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// 滤镜接口
interface IFilterObject : INonCopyable
{
	LONG m_Radius;	// 滤镜半径
	IFilterObject() : m_Radius(0) {}
	virtual void Filter(pixel_t* pix_des, CSize& sz_des, CRect& rc_des) = 0;

protected:
	EXP_INLINE LONG DesToBlc(CSize& sz_des, LONG x_d, LONG y_d, LONG x_b, LONG y_b)
	{
		if (sz_des.cx >= m_Radius)
		{
			x_d += (x_b - m_Radius);
			if (x_d < 0)
				x_d += m_Radius;
			else
			if (x_d >= sz_des.cx)
				x_d -= m_Radius;
		}
		if (sz_des.cy >= m_Radius)
		{
			y_d += (y_b - m_Radius);
			if (y_d < 0)
				y_d += m_Radius;
			else
			if (y_d >= sz_des.cy)
				y_d -= m_Radius;
		}
		return (sz_des.cy - y_d - 1) * sz_des.cx + x_d;
	}
	EXP_INLINE pixel_t* Block(pixel_t* pix_des, CSize& sz_des, LONG i_d, LONG x_d, LONG y_d)
	{
		pixel_t* pix_blc = NULL;
		if (m_Radius == 0)
		{
			pix_blc = ExMem::Alloc<pixel_t>();
			pix_blc[0] = pix_des[i_d];
		}
		else
		{
			LONG diamet = (m_Radius << 1) + 1;
			pix_blc = ExMem::Alloc<pixel_t>(diamet * diamet);
			for(LONG y_b = 0; y_b < diamet; ++y_b)
				for(LONG x_b = 0; x_b < diamet; ++x_b)
					pix_blc[(diamet - y_b - 1) * diamet + x_b] = 
					pix_des[DesToBlc(sz_des, x_d, y_d, x_b, y_b)];
		}
		return pix_blc;
	}
};

//////////////////////////////////////////////////////////////////

#pragma push_macro("PreFilter")
#undef PreFilter
#define PreFilter() \
	LONG w = min(sz_des.cx, rc_des.Right()); \
	LONG h = min(sz_des.cy, rc_des.Bottom()); \
	for(LONG y_d = rc_des.Top(); y_d < h; ++y_d) \
	{ \
		for(LONG x_d = rc_des.Left(); x_d < w; ++x_d) \
		{ \
			LONG i_d = (sz_des.cy - y_d - 1) * sz_des.cx + x_d
//#define PreRender

#pragma push_macro("EndFilter")
#undef EndFilter
#define EndFilter() \
		} \
	}
//#define EndRender

//////////////////////////////////////////////////////////////////

// 颜色画刷
class CFilterFill : public IFilterObject
{
public:
	BYTE m_Mask;
	pixel_t m_Const;
	bool m_bClrMask;
	pixel_t m_ClrMask;

public:
	CFilterFill(pixel_t cConst = 0, BYTE bMask = 0xf, bool bClrMask = false, pixel_t cMask = 0)
		: m_Const(cConst)
		, m_Mask(bMask)
		, m_bClrMask(bClrMask)
		, m_ClrMask(cMask)
	{}

	void Filter(pixel_t* pix_des, CSize& sz_des, CRect& rc_des)
	{
		//PreFilter();
for(LONG y_d = rc_des.Top(); y_d < min(sz_des.cy, rc_des.Bottom()); ++y_d)
{
	for(LONG x_d = rc_des.Left(); x_d < min(sz_des.cx, rc_des.Right()); ++x_d)
	{
		LONG i_d = (sz_des.cy - y_d - 1) * sz_des.cx + x_d;

		if (m_Mask == 0)
			continue;
		else
		if (m_bClrMask && pix_des[i_d] == m_ClrMask)
			continue;
		pix_des[i_d] = ExRGBA
			(
			(m_Mask & 0x08) ? ExGetR(m_Const) : ExGetR(pix_des[i_d]), 
			(m_Mask & 0x04) ? ExGetG(m_Const) : ExGetG(pix_des[i_d]), 
			(m_Mask & 0x02) ? ExGetB(m_Const) : ExGetB(pix_des[i_d]), 
			(m_Mask & 0x01) ? ExGetA(m_Const) : ExGetA(pix_des[i_d])
			);
	}
}
		//EndFilter();
	}
};

//////////////////////////////////////////////////////////////////

// 灰度化
class CFilterGray : public IFilterObject
{
public:
	void Filter(pixel_t* pix_des, CSize& sz_des, CRect& rc_des)
	{
		PreFilter();

		BYTE g = 
			(ExGetR(pix_des[i_d]) * 38 + ExGetG(pix_des[i_d]) * 75 + ExGetB(pix_des[i_d]) * 15) >> 7;
		pix_des[i_d] = ExRGBA(g, g, g, ExGetA(pix_des[i_d]));

		EndFilter();
	}
};

//////////////////////////////////////////////////////////////////

// 反色
class CFilterInverse : public IFilterObject
{
public:
	BYTE m_Mask;

public:
	CFilterInverse(BYTE bMask = 0xe)
		: m_Mask(bMask)
	{}

	void Filter(pixel_t* pix_des, CSize& sz_des, CRect& rc_des)
	{
		PreFilter();

		pix_des[i_d] = ExRGBA
			(
			(m_Mask & 0x08) ? EXP_CM - ExGetR(pix_des[i_d]) : ExGetR(pix_des[i_d]), 
			(m_Mask & 0x04) ? EXP_CM - ExGetG(pix_des[i_d]) : ExGetG(pix_des[i_d]), 
			(m_Mask & 0x02) ? EXP_CM - ExGetB(pix_des[i_d]) : ExGetB(pix_des[i_d]), 
			(m_Mask & 0x01) ? EXP_CM - ExGetA(pix_des[i_d]) : ExGetA(pix_des[i_d])
			);

		EndFilter();
	}
};

//////////////////////////////////////////////////////////////////

// 浮雕
class CFilterRelief : public IFilterObject
{
public:
	pixel_t m_Const;

public:
	CFilterRelief(pixel_t cConst = ExRGBA(125, 68, 29, EXP_CM))
		: m_Const(cConst)
	{ m_Radius = 1; }

	void Filter(pixel_t* pix_des, CSize& sz_des, CRect& rc_des)
	{
		if (m_Radius < 1) return;
		LONG diamet = (m_Radius << 1) + 1;
		LONG key1 = (diamet * diamet) >> 1;
		LONG key2 = (diamet * diamet) - 1;

		PreFilter();

		pixel_t* pix_blc = Block(pix_des, sz_des, i_d, x_d, y_d);
		pix_des[i_d] = ExRGBA
			(
			abs(ExGetR(pix_blc[key1]) - ExGetR(pix_blc[key2]) + ExGetB(m_Const)), 
			abs(ExGetG(pix_blc[key1]) - ExGetG(pix_blc[key2]) + ExGetG(m_Const)), 
			abs(ExGetB(pix_blc[key1]) - ExGetB(pix_blc[key2]) + ExGetR(m_Const)), 
			abs(ExGetA(pix_blc[key1]) - ExGetA(pix_blc[key2]) + ExGetA(m_Const))
			);
		ExMem::Free(pix_blc);

		EndFilter();
	}
};

//////////////////////////////////////////////////////////////////

// 扩散
class CFilterDiffuse : public IFilterObject
{
public:
	CFilterDiffuse(LONG nRadius = 1)
	{ m_Radius = nRadius; }

	void Filter(pixel_t* pix_des, CSize& sz_des, CRect& rc_des)
	{
		if (m_Radius < 1) return;
		LONG diamet = (m_Radius << 1) + 1;
		LONG flt_siz = diamet * diamet;

		ExRandomize();

		PreFilter();

		pixel_t* pix_blc = Block(pix_des, sz_des, i_d, x_d, y_d);
		pix_des[i_d] = pix_blc[ExRandom(flt_siz)];
		ExMem::Free(pix_blc);

		EndFilter();
	}
};

//////////////////////////////////////////////////////////////////

// 高斯模糊
class CFilterGauss : public IFilterObject
{
public:
	CFilterGauss(LONG nRadius = 5)
	{ m_Radius = nRadius; }

	void Filter(pixel_t* pix_des, CSize& sz_des, CRect& rc_des)
	{
		if (m_Radius < 1) return;
		LONG diamet = (m_Radius << 1) + 1;
		LONG flt_siz = diamet * diamet;

		double s = (double)m_Radius / 3.0;						// 正态分布的标准偏差σ
		double sigma2 = 2.0 * s * s;							// 2倍的σ平方,参考N维空间正态分布方程
		double nuclear = 0.0;									// 高斯卷积核
		double* matrix = ExMem::Alloc<double>(diamet * diamet);	// 高斯矩阵定义

		// 计算高斯矩阵
		int i = 0;
		for(long y = -m_Radius; y <= m_Radius; ++y)
			for(long x = -m_Radius; x <= m_Radius; ++x)
			{
				matrix[i] = 
					exp(-(double)(x * x + y * y) / sigma2);
				nuclear += matrix[i];
				++i;
			}

		// 遍历并处理像素

		PreFilter();

		pixel_t* pix_blc = Block(pix_des, sz_des, i_d, x_d, y_d);
		double r = 0.0, g = 0.0, b = 0.0, a = 0.0;
		for(int i = 0; i < flt_siz; ++i)
		{
			double weight = matrix[i] / nuclear;
			r += weight * ExGetR(pix_blc[i]);
			g += weight * ExGetG(pix_blc[i]);
			b += weight * ExGetB(pix_blc[i]);
			a += weight * ExGetA(pix_blc[i]);
		}
		pix_des[i_d] = ExRGBA
			(
			(BYTE)(r > (BYTE)~0 ? (BYTE)~0 : r), 
			(BYTE)(g > (BYTE)~0 ? (BYTE)~0 : g), 
			(BYTE)(b > (BYTE)~0 ? (BYTE)~0 : b), 
			(BYTE)(a > (BYTE)~0 ? (BYTE)~0 : a)
			);
		ExMem::Free(pix_blc);

		EndFilter();
	}
};

//////////////////////////////////////////////////////////////////

// 渐变
class CFilterGradient : public IFilterObject
{
public:
	enum mode_t { normal, less, more };
	BYTE m_Mask;
	pixel_t m_Grad[2];
	bool m_Vert;
	mode_t m_Mode;

public:
	CFilterGradient(pixel_t cStt = ExRGBA(255, 255, 255, 255), pixel_t cEnd = ExRGBA(0, 0, 0, 0), 
					BYTE bMask = 0x1, bool bVert = true, mode_t eMode = normal)
	{
		m_Grad[0] = cStt;
		m_Grad[1] = cEnd;
		m_Mask = bMask;
		m_Vert = bVert;
		m_Mode = eMode;
	}

	void Filter(pixel_t* pix_des, CSize& sz_des, CRect& rc_des)
	{
		if (m_Mask == 0) return;

		// 颜色偏移量
		double div = m_Vert ? sz_des.cy : sz_des.cx;
		double off[4] = 
		{
			(double)(ExGetR(m_Grad[1]) - ExGetR(m_Grad[0])) / div, 
			(double)(ExGetG(m_Grad[1]) - ExGetG(m_Grad[0])) / div, 
			(double)(ExGetB(m_Grad[1]) - ExGetB(m_Grad[0])) / div, 
			(double)(ExGetA(m_Grad[1]) - ExGetA(m_Grad[0])) / div
		};

		// 设置颜色

		PreFilter();

		BYTE a_d = ExGetA(pix_des[i_d]);
		BYTE r_d = ExGetR(pix_des[i_d]);
		BYTE g_d = ExGetG(pix_des[i_d]);
		BYTE b_d = ExGetB(pix_des[i_d]);
		pixel_t pix = ExRGBA
			(
			ExGetR(m_Grad[0]) + off[0] * (m_Vert ? y_d : x_d), 
			ExGetG(m_Grad[0]) + off[1] * (m_Vert ? y_d : x_d), 
			ExGetB(m_Grad[0]) + off[2] * (m_Vert ? y_d : x_d), 
			ExGetA(m_Grad[0]) + off[3] * (m_Vert ? y_d : x_d)
			);

		switch (m_Mode)
		{
		case normal:
			break;
		case less:
			pix = ExRGBA
				(
				ExGetR(pix) < r_d ? ExGetR(pix) : r_d, 
				ExGetG(pix) < g_d ? ExGetG(pix) : g_d, 
				ExGetB(pix) < b_d ? ExGetB(pix) : b_d, 
				ExGetA(pix) < a_d ? ExGetA(pix) : a_d
				);
			break;													   
		case more:														   
			pix = ExRGBA
				(
				ExGetR(pix) > r_d ? ExGetR(pix) : r_d, 
				ExGetG(pix) > g_d ? ExGetG(pix) : g_d, 
				ExGetB(pix) > b_d ? ExGetB(pix) : b_d, 
				ExGetA(pix) > a_d ? ExGetA(pix) : a_d
				);
			break;
		}

		pix_des[i_d] = ExRGBA
			(
			(m_Mask & 0x08) ? ExGetR(pix) : r_d, 
			(m_Mask & 0x04) ? ExGetG(pix) : g_d, 
			(m_Mask & 0x02) ? ExGetB(pix) : b_d, 
			(m_Mask & 0x01) ? ExGetA(pix) : a_d
			);

		EndFilter();
	}
};

//////////////////////////////////////////////////////////////////

#pragma pop_macro("EndFilter")
#pragma pop_macro("PreFilter")

//////////////////////////////////////////////////////////////////

class CImgFilter
{
public:
	EXP_INLINE static bool Filter(image_t imgDes, CRect& rcDes, IFilterObject* pFilter)
	{
		if (!pFilter) return false;
		CImage exp_des;
		exp_des.SetTrust(false);
		exp_des = imgDes;
		if (exp_des.IsNull()) return false;
		CSize sz_des(exp_des.GetWidth(), exp_des.GetHeight());
		if (rcDes.IsEmpty())
			rcDes.Set(CPoint(), CPoint(sz_des.cx, sz_des.cy));
		// 遍历像素绘图
		pixel_t* pix_des = exp_des.GetPixels();
		pFilter->Filter(pix_des, sz_des, rcDes);
		return true;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ImgFilter_h__*/