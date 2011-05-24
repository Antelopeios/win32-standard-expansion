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
// FilterObject - 滤镜对象
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-24
// Version:	1.0.0003.1054
//
// History:
//	- 1.0.0001.1328(2011-05-02)	+ 添加渐变滤镜
//	- 1.0.0002.1948(2011-05-18)	= 调整CFilterNormal滤镜返回像素的alpha通道计算方法
//	- 1.0.0003.1054(2011-05-24)	+ 调整CFilterCopy;CFilterNormal滤镜支持设置半透明渲染参数
//////////////////////////////////////////////////////////////////

#ifndef __FilterObject_h__
#define __FilterObject_h__

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
	LONG m_w, m_h;	// 目标区域大小

	IFilterObject() : m_Radius(0), m_w(0), m_h(0) {}

	virtual LONG GetRadius() { return m_Radius; }
	virtual void SetSize(LONG w, LONG h) { m_w = w; m_h = h; }
	virtual pixel_t Render(pixel_t* pixSrc, pixel_t pixDes, LONG nKey) = 0;
};

//////////////////////////////////////////////////////////////////

// 像素拷贝
class CFilterCopy : public IFilterObject
{
protected:
	BYTE m_Alpha;

public:
	CFilterCopy(BYTE a = (BYTE)~0)
		: IFilterObject()
	{
		m_Radius = 1;
		m_Alpha = a;
	}

	pixel_t Render(pixel_t* pixSrc, pixel_t pixDes, LONG nKey)
	{
		if (m_Alpha == 0)
			return 0;
		else
		if (m_Alpha == (BYTE)~0)
			return pixSrc[nKey];
		else
			return ExRGBA
				(
				ExGetR(pixSrc[nKey]) * m_Alpha / (BYTE)~0, 
				ExGetG(pixSrc[nKey]) * m_Alpha / (BYTE)~0, 
				ExGetB(pixSrc[nKey]) * m_Alpha / (BYTE)~0, 
				ExGetA(pixSrc[nKey]) * m_Alpha / (BYTE)~0
				);
	}
};

//////////////////////////////////////////////////////////////////

// 正常渲染
class CFilterNormal : public IFilterObject
{
protected:
	BYTE m_Alpha;

public:
	CFilterNormal(BYTE a = (BYTE)~0)
		: IFilterObject()
	{
		m_Radius = 1;
		m_Alpha = a;
	}

	pixel_t Render(pixel_t* pixSrc, pixel_t pixDes, LONG nKey)
	{
		BYTE a_s = (ExGetA(pixSrc[nKey]) * m_Alpha) / (BYTE)~0;
		if (a_s == 0) return pixDes;
		if (a_s == (BYTE)~0) return pixSrc[nKey];
		BYTE r_s = ExGetR(pixSrc[nKey]);
		BYTE g_s = ExGetG(pixSrc[nKey]);
		BYTE b_s = ExGetB(pixSrc[nKey]);
		BYTE a_d = ExGetA(pixDes);
		BYTE r_d = ExGetR(pixDes);
		BYTE g_d = ExGetG(pixDes);
		BYTE b_d = ExGetB(pixDes);
		BYTE a_i = (BYTE)~0 - a_s;
		return ExRGBA
			(
			(r_s * a_s + r_d * a_i) / (BYTE)~0, 
			(g_s * a_s + g_d * a_i) / (BYTE)~0, 
			(b_s * a_s + b_d * a_i) / (BYTE)~0, 
			(a_d + a_s) - a_d * a_s / (BYTE)~0
			);
	}
};

//////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////

// 外发光
template <typename BaseT = CFilterNormal>
class CFilterOuterGlowT : public CFilterGaussT<BaseT>
{
protected:
	CFilterNormal m_Filter;
	pixel_t* m_PixSdw;
	pixel_t* m_PixChk[2];
public:
	pixel_t m_Const;

public:
	CFilterOuterGlowT(LONG nRadius = 7, pixel_t cConst = ExRGB(255, 255, 190))
		: CFilterGaussT<BaseT>(nRadius)
		, m_Const(cConst)
	{
		m_PixSdw = ExMem::Alloc<pixel_t>(m_FltSiz);
		m_PixChk[0] = ExMem::Alloc<pixel_t>(m_FltSiz);
		m_PixChk[1] = ExMem::Alloc<pixel_t>(m_FltSiz);
		for(int i = 0; i <= m_FltSiz; ++i)
		{
			m_PixChk[0][i] = ExRGBA
				(
				ExGetR(m_Const), 
				ExGetG(m_Const), 
				ExGetB(m_Const), 
				0
				);
			m_PixChk[1][i] = ExRGBA
				(
				ExGetR(m_Const), 
				ExGetG(m_Const), 
				ExGetB(m_Const), 
				(BYTE)~0
				);
		}
	}
	~CFilterOuterGlowT()
	{
		ExMem::Free(m_PixChk[1]);
		ExMem::Free(m_PixChk[0]);
		ExMem::Free(m_PixSdw);
	}

	pixel_t Render(pixel_t* pixSrc, pixel_t pixDes, LONG nKey)
	{
		if (m_Radius < 2) return BaseT::Render(pixSrc, pixDes, nKey);
		for(int i = 0; i <= m_FltSiz; ++i)
		{
			m_PixSdw[i] = ExRGBA
				(
				ExGetB(m_Const), 
				ExGetG(m_Const), 
				ExGetR(m_Const), 
				ExGetA(pixSrc[i])
				);
		}
		if (memcmp(m_PixSdw, m_PixChk[0], m_FltSiz * sizeof(pixel_t)) == 0 || 
			memcmp(m_PixSdw, m_PixChk[1], m_FltSiz * sizeof(pixel_t)) == 0)
			return m_Filter.Render(pixSrc, BaseT::Render(m_PixSdw, pixDes, nKey), nKey);
		else
			return m_Filter.Render(pixSrc, CFilterGaussT<BaseT>::Render(m_PixSdw, pixDes, nKey), nKey);
	}
};
typedef CFilterOuterGlowT<> CFilterOuterGlow;

//////////////////////////////////////////////////////////////////

// 渐变
template <typename BaseT = CFilterNormal>
class CFilterGradientT : public BaseT
{
protected:
	double m_cOff[4];
	double m_div;
public:
	enum mode_t { normal, less, more };
	BYTE m_Mask;
	pixel_t m_Grad[2];
	bool m_Vert;
	mode_t m_Mode;

public:
	CFilterGradientT(pixel_t cStt = ExRGBA(255, 255, 255, 255), pixel_t cEnd = ExRGBA(0, 0, 0, 0), 
		BYTE bMask = 0x1, bool bVert = true, mode_t eMode = normal)
		: BaseT()
	{
		ZeroMemory(m_cOff, sizeof(m_cOff));
		m_Grad[0] = cStt;
		m_Grad[1] = cEnd;
		m_Mask = bMask;
		m_Vert = bVert;
		m_Mode = eMode;
	}

	void SetSize(LONG w, LONG h)
	{
		BaseT::SetSize(w, h);
		m_div = m_Vert ? m_h : m_w;
		m_cOff[0] = (double)(ExGetR(m_Grad[1]) - ExGetR(m_Grad[0])) / m_div;
		m_cOff[1] = (double)(ExGetG(m_Grad[1]) - ExGetG(m_Grad[0])) / m_div;
		m_cOff[2] = (double)(ExGetB(m_Grad[1]) - ExGetB(m_Grad[0])) / m_div;
		m_cOff[3] = (double)(ExGetA(m_Grad[1]) - ExGetA(m_Grad[0])) / m_div;
	}

	pixel_t Render(pixel_t* pixSrc, pixel_t pixDes, LONG nKey)
	{
		if (m_Radius < 1) return BaseT::Render(pixSrc, pixDes, nKey);
		if (m_Mask == 0) return BaseT::Render(pixSrc, pixDes, nKey);
		// 颜色偏移量
		static LONG off_x = 0, off_y = 0;
		// 设置颜色
		pixel_t pix = ExRGBA
			(
			ExGetR(m_Grad[0]) + m_cOff[0] * (m_Vert ? off_y : off_x), 
			ExGetG(m_Grad[0]) + m_cOff[1] * (m_Vert ? off_y : off_x), 
			ExGetB(m_Grad[0]) + m_cOff[2] * (m_Vert ? off_y : off_x), 
			ExGetA(m_Grad[0]) + m_cOff[3] * (m_Vert ? off_y : off_x)
			);
		switch (m_Mode)
		{
		case normal:
			break;
		case less:
			pix = ExRGBA
				(
				ExGetR(pix) < ExGetR(pixSrc[nKey]) ? ExGetR(pix) : ExGetR(pixSrc[nKey]), 
				ExGetG(pix) < ExGetG(pixSrc[nKey]) ? ExGetG(pix) : ExGetG(pixSrc[nKey]), 
				ExGetB(pix) < ExGetB(pixSrc[nKey]) ? ExGetB(pix) : ExGetB(pixSrc[nKey]), 
				ExGetA(pix) < ExGetA(pixSrc[nKey]) ? ExGetA(pix) : ExGetA(pixSrc[nKey])
				);
			break;													   
		case more:														   
			pix = ExRGBA
				(
				ExGetR(pix) > ExGetR(pixSrc[nKey]) ? ExGetR(pix) : ExGetR(pixSrc[nKey]), 
				ExGetG(pix) > ExGetG(pixSrc[nKey]) ? ExGetG(pix) : ExGetG(pixSrc[nKey]), 
				ExGetB(pix) > ExGetB(pixSrc[nKey]) ? ExGetB(pix) : ExGetB(pixSrc[nKey]), 
				ExGetA(pix) > ExGetA(pixSrc[nKey]) ? ExGetA(pix) : ExGetA(pixSrc[nKey])
				);
			break;
		}
		pixSrc[nKey] = ExRGBA
			(
			(m_Mask & 0x08) ? ExGetR(pix) : ExGetR(pixSrc[nKey]), 
			(m_Mask & 0x04) ? ExGetG(pix) : ExGetG(pixSrc[nKey]), 
			(m_Mask & 0x02) ? ExGetB(pix) : ExGetB(pixSrc[nKey]), 
			(m_Mask & 0x01) ? ExGetA(pix) : ExGetA(pixSrc[nKey])
			);
		// 移动偏移量
		++off_x;
		if (off_x >= m_w)
		{
			off_x = 0;
			++off_y;
		}
		return BaseT::Render(pixSrc, pixDes, nKey);
	}
};
typedef CFilterGradientT<> CFilterGradient;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__FilterObject_h__*/