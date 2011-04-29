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
// Date:	2011-04-29
// Version:	1.0.0003.1643
//
// History:
//	- 1.0.0001.1730(2011-04-27)	= 将渲染器内部的渲染回调指针改为滤镜接口
//								+ 添加一些滤镜效果类
//	- 1.0.0002.1410(2011-04-28)	+ 添加像素直接拷贝与高斯模糊滤镜
//								^ 将滤镜功能类改为类模板,支持外部动态调整渲染基
//	- 1.0.0003.1643(2011-04-29)	# 修正高斯模糊导致图像变暗与对比度失真等问题
//								= 调整默认高斯模糊半径为5
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
	// 滤镜接口
	interface IFilter : INonCopyable
	{
		LONG m_Radius;

		IFilter() : m_Radius(3) {}

		virtual LONG GetRadius() { return m_Radius; }
		virtual pixel_t Render(pixel_t* pixSrc, pixel_t pixDes, LONG nKey) = 0;
	};
	// 像素拷贝
	class CFilterCopy : public IFilter
	{
	public:
		CFilterCopy() : IFilter() { m_Radius = 1; }

		pixel_t Render(pixel_t* pixSrc, pixel_t pixDes, LONG nKey)
		{
			return pixSrc[nKey];
		}
	};
	// 正常渲染
	class CFilterNormal : public IFilter
	{
	public:
		CFilterNormal() : IFilter() { m_Radius = 1; }

		pixel_t Render(pixel_t* pixSrc, pixel_t pixDes, LONG nKey)
		{
			BYTE a_s = ExGetA(pixSrc[nKey]);
			if (a_s == 0) return pixDes;
			if (a_s == (BYTE)~0) return pixSrc[nKey];
			BYTE r_s = ExGetR(pixSrc[nKey]);
			BYTE g_s = ExGetG(pixSrc[nKey]);
			BYTE b_s = ExGetB(pixSrc[nKey]);
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
	};
	// 灰度化
	template <typename BaseT = CFilterNormal>
	class CFilterGrayT : public BaseT
	{
	public:
		pixel_t Render(pixel_t* pixSrc, pixel_t pixDes, LONG nKey)
		{
			BYTE g = 
				(ExGetR(pixSrc[nKey]) * 38 + ExGetG(pixSrc[nKey]) * 75 + ExGetB(pixSrc[nKey]) * 15) >> 7;
			pixel_t src = ExRGBA(g, g, g, ExGetA(pixSrc[nKey]));
			return BaseT::Render(&src, pixDes, nKey);
		}
	};
	typedef CFilterGrayT<> CFilterGray;
	// 反色
	template <typename BaseT = CFilterNormal>
	class CFilterInverseT : public BaseT
	{
	public:
		pixel_t Render(pixel_t* pixSrc, pixel_t pixDes, LONG nKey)
		{
			pixel_t src = ExRGBA
				(
				(BYTE)~0 - ExGetR(pixSrc[nKey]), 
				(BYTE)~0 - ExGetG(pixSrc[nKey]), 
				(BYTE)~0 - ExGetB(pixSrc[nKey]), 
				ExGetA(pixSrc[nKey])
				);
			return BaseT::Render(&src, pixDes, nKey);
		}
	};
	typedef CFilterInverseT<> CFilterInverse;
	// 浮雕
	template <typename BaseT = CFilterNormal>
	class CFilterReliefT : public BaseT
	{
	protected:
		LONG m_Diamet;
	public:
		pixel_t m_Const;

	public:
		CFilterReliefT(pixel_t cConst = ExRGBA(125, 68, 29, (BYTE)~0))
			: BaseT()
			, m_Const(cConst)
		{
			m_Radius = 2;
			m_Diamet = (m_Radius << 1) - 1;
		}

		pixel_t Render(pixel_t* pixSrc, pixel_t pixDes, LONG nKey)
		{
			if (m_Radius < 2) return BaseT::Render(pixSrc, pixDes, nKey);
			pixel_t pix1 = pixSrc[nKey];
			pixel_t pix2 = pixSrc[nKey + m_Diamet + 1];
			pixSrc[nKey] = ExRGBA
				(
				abs(ExGetR(pix1) - ExGetR(pix2) + ExGetB(m_Const)), 
				abs(ExGetG(pix1) - ExGetG(pix2) + ExGetG(m_Const)), 
				abs(ExGetB(pix1) - ExGetB(pix2) + ExGetR(m_Const)), 
				abs(ExGetA(pix1) - ExGetA(pix2) + ExGetA(m_Const))
				);
			return BaseT::Render(pixSrc, pixDes, nKey);
		}
	};
	typedef CFilterReliefT<> CFilterRelief;
	// 扩散
	template <typename BaseT = CFilterNormal>
	class CFilterDiffuseT : public BaseT
	{
	protected:
		LONG m_FltSiz;

	public:
		CFilterDiffuseT(LONG nRadius = 1)
			: BaseT()
		{
			m_Radius = nRadius + 1;
			LONG diamet = (m_Radius << 1) - 1;
			m_FltSiz = diamet * diamet;
			ExRandomize();
		}

		pixel_t Render(pixel_t* pixSrc, pixel_t pixDes, LONG nKey)
		{
			if (m_Radius < 2) return BaseT::Render(pixSrc, pixDes, nKey);
			pixSrc[nKey] = pixSrc[ExRandom(m_FltSiz)];
			return BaseT::Render(pixSrc, pixDes, nKey);
		}
	};
	typedef CFilterDiffuseT<> CFilterDiffuse;
	// 高斯模糊
	template <typename BaseT = CFilterNormal>
	class CFilterGaussT : public BaseT
	{
	protected:
		LONG m_Diamet, m_FltSiz;
		double* m_gMatrix;	// 卷积矩阵
		double m_Nuclear;	// 卷积核

	public:
		CFilterGaussT(LONG nRadius = 5)
			: BaseT()
		{
			m_Radius = nRadius + 1;
			m_Diamet = (m_Radius << 1) - 1;
			m_FltSiz = m_Diamet * m_Diamet;
			double s = (double)nRadius / 3.0;
			double sigma2 = 2.0 * s * s;
			m_Nuclear = 0.0;
			m_gMatrix = ExMem::Alloc<double>(m_Diamet * m_Diamet);
			// 计算高斯矩阵
			int i = 0;
			for(long y = -nRadius; y <= nRadius; ++y)
				for(long x = -nRadius; x <= nRadius; ++x)
				{
					m_gMatrix[i] = 
						exp(-(double)(x * x + y * y) / sigma2);
					m_Nuclear += m_gMatrix[i];
					++i;
				}
		}
		~CFilterGaussT()
		{
			ExMem::Free(m_gMatrix);
		}

		pixel_t Render(pixel_t* pixSrc, pixel_t pixDes, LONG nKey)
		{
			if (m_Radius < 2) return BaseT::Render(pixSrc, pixDes, nKey);
			double r = 0.0, g = 0.0, b = 0.0, a = 0.0;
			for(int i = 0; i <= m_FltSiz; ++i)
			{
				double weight = m_gMatrix[i] / m_Nuclear;
				r += weight * ExGetR(pixSrc[i]);
				g += weight * ExGetG(pixSrc[i]);
				b += weight * ExGetB(pixSrc[i]);
				a += weight * ExGetA(pixSrc[i]);
			}
			pixSrc[nKey] = ExRGBA
				(
				r > (BYTE)~0 ? (BYTE)~0 : r, 
				g > (BYTE)~0 ? (BYTE)~0 : g, 
				b > (BYTE)~0 ? (BYTE)~0 : b, 
				a > (BYTE)~0 ? (BYTE)~0 : a
				);
			return BaseT::Render(pixSrc, pixDes, nKey);
		}
	};
	typedef CFilterGaussT<> CFilterGauss;

protected:
	EXP_INLINE static void GetFilterBlock(pixel_t* pixSrc, LONG nX, LONG nY, LONG nW, LONG nH, CRect& rcSrc, 
										  pixel_t* pixBlk, LONG nDiamet, LONG nFltRad)
	{
		LONG x_b = max(nX - nFltRad, rcSrc.Left()), x_e = min(nX + nFltRad, rcSrc.Right() - 1);
		LONG y_b = max(nY - nFltRad, rcSrc.Top()), y_e = min(nY + nFltRad, rcSrc.Bottom() - 1);
		LONG div_x_b = x_b - (nX - nFltRad), div_y_b = y_b - (nY - nFltRad), div_flt = (x_e - x_b + 1) * sizeof(pixel_t);
		for(LONG n = y_b; n <= y_e; ++n)
		{
			LONG i_pix = (nH - n - 1) * nW + x_b;
			LONG i_flt = (nDiamet - (div_y_b + n - y_b) - 1) * nDiamet + div_x_b;
			memcpy(pixBlk + i_flt, pixSrc + i_pix, div_flt);
		}
	}

public:
	EXP_INLINE static bool Render(image_t imgDes, image_t imgSrc, CRect& rcDes, CPoint& ptSrc, IFilter* pFilter = NULL)
	{
		CImage exp_des(imgDes);
		if (exp_des.IsNull()) return false;
		CImage exp_src(imgSrc);
		if (exp_src.IsNull()) return false;
		if (rcDes.IsEmpty())
			rcDes.Set(CPoint(), CPoint(exp_des.GetWidth(), exp_des.GetHeight()));
		// 遍历像素绘图
		if (!pFilter) pFilter = &CFilterNormal();
		LONG radius = pFilter->GetRadius();
		if (radius <= 0) return false;
		LONG diamet = (radius << 1) - 1;
		LONG fltsiz = diamet * diamet;
		LONG fltrad = radius - 1;
		LONG fltind = diamet * fltrad + fltrad;
		pixel_t* flt_src = ExMem::Alloc<pixel_t>(fltsiz); // 滤镜像素块
		pixel_t* pix_des = exp_des.GetPixels();
		pixel_t* pix_src = exp_src.GetPixels();
		for(LONG y_s = ptSrc.y; y_s < (LONG)exp_src.GetHeight(); ++y_s)
		{
			for(LONG x_s = ptSrc.x; x_s < (LONG)exp_src.GetWidth(); ++x_s)
			{
				// 校验像素区域
				if (x_s >= ptSrc.x + rcDes.Width() || 
					y_s >= ptSrc.y + rcDes.Height()) continue;
				LONG x_d = rcDes.Left() + x_s;
				if (x_d < 0) continue;
				if (x_d >= (LONG)exp_des.GetWidth()) break;
				LONG y_d = rcDes.Top() + y_s;
				if (y_d < 0) continue;
				if (y_d >= (LONG)exp_des.GetHeight()) break;
				// 获得像素块
				CRect rc(ptSrc, CPoint(exp_src.GetWidth(), exp_src.GetHeight()));
				GetFilterBlock(pix_src, x_s, y_s, exp_src.GetWidth(), exp_src.GetHeight(), rc, flt_src, diamet, fltrad);
				// 渲染像素
				LONG i_d = (exp_des.GetHeight() - y_d - 1) * exp_des.GetWidth() + x_d;
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