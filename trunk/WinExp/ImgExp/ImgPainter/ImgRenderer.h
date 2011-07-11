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
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-07-10
// Version:	1.0.0007.2200
//
// History:
//	- 1.0.0001.1730(2011-04-27)	= 将渲染器内部的渲染回调指针改为滤镜接口
//								+ 添加一些滤镜效果类
//	- 1.0.0002.1410(2011-04-28)	+ 添加像素直接拷贝与高斯模糊滤镜
//								^ 将滤镜功能类改为类模板,支持外部动态调整渲染基
//	- 1.0.0003.1643(2011-04-29)	# 修正高斯模糊导致图像变暗与对比度失真等问题
//								= 调整默认高斯模糊半径为5
//	- 1.0.0004.2350(2011-05-01)	# 修正正常渲染滤镜忽略了背景Alpha通道,导致图像混合后不再透明的问题
//								# 修正因滤镜像素块采集算法在图像边缘处忽略了无法采集的部分,导致一些滤镜边缘渲染的异常
//								+ 添加外发光滤镜
//	- 1.0.0005.1330(2011-05-02)	= 将滤镜对象从ImgRenderer内部分离
//	- 1.0.0006.1130(2011-05-26)	# 修正ImgRenderer::Render()在绘图时顶点坐标偏移的计算错误
//	- 1.0.0007.2200(2011-07-10)	^ 将滤镜模块从ImgRenderer中分离,提高ImgRenderer的渲染效率
//////////////////////////////////////////////////////////////////

#ifndef __ImgRenderer_h__
#define __ImgRenderer_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgTypes/Types/Types.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// 渲染器接口
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
	for(LONG y_s = pt_src.y; y_s < sz_src.cy; ++y_s) \
	{ \
		for(LONG x_s = pt_src.x; x_s < sz_src.cx; ++x_s) \
		{ /*校验像素区域*/ \
			if (x_s >= pt_src.x + rc_des.Width() ||  \
				y_s >= pt_src.y + rc_des.Height()) continue; \
			LONG x_d = rc_des.Left() + x_s - pt_src.x; \
			if (x_d < 0) continue; \
			if (x_d >= sz_des.cx) break; \
			LONG y_d = rc_des.Top() + y_s - pt_src.y; \
			if (y_d < 0) continue; \
			if (y_d >= sz_des.cy) break; \
			LONG i_d = (sz_des.cy - y_d - 1) * sz_des.cx + x_d; \
			LONG i_s = (sz_src.cy - y_s - 1) * sz_src.cx + x_s
//#define PreRender

#pragma push_macro("EndRender")
#undef EndRender
#define EndRender() \
		} \
	}
//#define EndRender

//////////////////////////////////////////////////////////////////

// 像素拷贝
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
		PreRender();

		if (m_Alpha == 0)
			continue;
		else
		if (m_Alpha == EXP_CM)
		{
			pix_des[i_d] = pix_src[i_s];
			continue;
		}
		else
		{
			int r_dif = ((int)ExGetR(pix_src[i_s]) - (int)ExGetR(pix_des[i_d])) * m_Alpha / EXP_CM;
			int g_dif = ((int)ExGetG(pix_src[i_s]) - (int)ExGetG(pix_des[i_d])) * m_Alpha / EXP_CM;
			int b_dif = ((int)ExGetB(pix_src[i_s]) - (int)ExGetB(pix_des[i_d])) * m_Alpha / EXP_CM;
			int a_dif = ((int)ExGetA(pix_src[i_s]) - (int)ExGetA(pix_des[i_d])) * m_Alpha / EXP_CM;
			pix_des[i_d] = ExRGBA
				(
				ExGetR(pix_des[i_d]) + r_dif, 
				ExGetG(pix_des[i_d]) + g_dif, 
				ExGetB(pix_des[i_d]) + b_dif, 
				ExGetA(pix_des[i_d]) + a_dif
				);
		}

		EndRender();
	}
};

//////////////////////////////////////////////////////////////////

// 正常渲染
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

		BYTE a_s = (ExGetA(pix_src[i_s]) * m_Alpha) / EXP_CM;
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
			(r_s * a_s + r_d * a_i) / EXP_CM, 
			(g_s * a_s + g_d * a_i) / EXP_CM, 
			(b_s * a_s + b_d * a_i) / EXP_CM, 
			(a_d + a_s) - a_d * a_s / EXP_CM
			);

		EndRender();
	}
};

#ifndef EXP_IMG_RENDER
#define EXP_IMG_RENDER CRenderNormal
#endif/*EXP_IMG_RENDER*/


//////////////////////////////////////////////////////////////////

// 图片叠加
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

		BYTE a_s = (ExGetA(pix_src[i_s]) * m_Alpha) / EXP_CM;
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
		BYTE a_r = (a_d + a_s) - a_d * a_s / EXP_CM;
		pix_des[i_d] = ExRGBA
			(
			(r_d * a_d * a_i + EXP_CM * r_s * a_s) / (EXP_CM * a_r), 
			(g_d * a_d * a_i + EXP_CM * g_s * a_s) / (EXP_CM * a_r), 
			(b_d * a_d * a_i + EXP_CM * b_s * a_s) / (EXP_CM * a_r), 
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
		// 遍历像素绘图
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