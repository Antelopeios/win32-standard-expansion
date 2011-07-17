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
// Date:	2011-07-16
// Version:	1.0.0013.1947
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
//	- 1.0.0008.2350(2011-07-11)	^ 略微优化PreRender()的执行效率
//								^ 优化渲染器算法,提高执行效率(+25%)
//	- 1.0.0009.2328(2011-07-12)	^ 优化PreRender()的执行效率(+40-60%)
//	- 1.0.0010.2345(2011-07-13)	^ 优化CImgRenderer::Render()与PreRender()的执行效率
//								# 修正CImgRenderer::Render()区域校验中的错误
//	- 1.0.0011.2350(2011-07-14)	^ 使用sse优化CRenderCopy的执行效率
//	- 1.0.0012.1640(2011-07-15)	# 修正CRenderNormal对alpha通道处理时出现的数据溢出误差
//	- 1.0.0013.1947(2011-07-16)	# 修正CImgRenderer::Render()在执行后有可能修改传入区域的问题
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
		LONG w, LONG h, LONG inx_des, LONG inx_src
		) = 0;
};

//////////////////////////////////////////////////////////////////

#pragma push_macro("PreRender")
#undef PreRender
#define PreRender() \
	for(LONG y = 0; y < h; ++y, inx_des -= sz_des.cx, inx_src -= sz_src.cx) \
	{ \
		for(LONG x = 0, i_d = inx_des, i_s = inx_src; x < w; ++x, ++i_d, ++i_s) \
		{ \
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
		LONG w, LONG h, LONG inx_des, LONG inx_src
		)
	{
		if (m_Alpha == 0)
			return;
		else
		if (m_Alpha == EXP_CM)
		{	// 拷贝像素
			__asm
			{
				push ecx
				push edx
				push eax
			}
			for(LONG y = 0; y < h; ++y, inx_des -= sz_des.cx, inx_src -= sz_src.cx)
			{
				int sse_len = w >> 2;
				pixel_t* ps = pix_src + inx_src;
				pixel_t* pd = pix_des + inx_des;
				__asm
				{
					mov	eax, [ps]
					mov edx, [pd]
					mov ecx, [sse_len]
				sse_loop_1:
					movups xmm0, [eax]
					movups [edx], xmm0
					add eax, 10h
					add edx, 10h
					dec ecx
					jnz sse_loop_1
				}
				sse_len <<= 2;
				int nor_len = w - sse_len;
				if (nor_len)
					memcpy(pd + sse_len, ps + sse_len, (nor_len << 2));
			}
			__asm
			{
				pop eax
				pop edx
				pop ecx
				emms
			}
		}
		else
		{
			WORD p_m[8] = 
			{
				0x00ff, 0x00ff, 0x00ff, 0x00ff, 
				0x00ff, 0x00ff, 0x00ff, 0x00ff
			};
			WORD p_a[8] = 
			{
				m_Alpha, m_Alpha, m_Alpha, m_Alpha, 
				m_Alpha, m_Alpha, m_Alpha, m_Alpha
			};
			pixel_t* pm = (pixel_t*)p_m;
			pixel_t* pa = (pixel_t*)p_a;
			__asm
			{
				push ecx
				push edx
				push eax
				push esi
				push edi
				mov edi, [pm]
				mov esi, [pa]
				movups xmm6, [edi]
				movups xmm7, [esi]
			}
			for(LONG y = 0; y < h; ++y, inx_des -= sz_des.cx, inx_src -= sz_src.cx)
			{
				int sse_len = w >> 1;
				pixel_t* ps = pix_src + inx_src;
				pixel_t* pd = pix_des + inx_des;
				__asm
				{
					mov	eax, [ps]
					mov edx, [pd]
					mov ecx, [sse_len]
				sse_loop_2:
					// 拓展ps
					movups xmm0, [eax]
					punpcklbw xmm0, xmm0
					pand xmm0, xmm6
					// 拓展pd
					movups xmm1, [edx]
					punpcklbw xmm1, xmm1
					pand xmm1, xmm6
					// 混色计算
					psubw xmm0, xmm1
					pmullw xmm0, xmm7
					psraw xmm0, 8
					paddw xmm0, xmm1
					// 恢复像素
					packuswb xmm0, xmm0
					// 保存结果
					movlpd [edx], xmm0
					// 循环扫描
					add eax, 08h
					add edx, 08h
					dec ecx
					jnz sse_loop_2
				}
				sse_len <<= 1;
				for(LONG x = sse_len; x < w; ++x)
				{
					LONG i_d = inx_des + sse_len, i_s = inx_src + sse_len;
					BYTE r_dif = (ExGetR(pix_src[i_s]) - ExGetR(pix_des[i_d])) * m_Alpha >> 8;
					BYTE g_dif = (ExGetG(pix_src[i_s]) - ExGetG(pix_des[i_d])) * m_Alpha >> 8;
					BYTE b_dif = (ExGetB(pix_src[i_s]) - ExGetB(pix_des[i_d])) * m_Alpha >> 8;
					BYTE a_dif = (ExGetA(pix_src[i_s]) - ExGetA(pix_des[i_d])) * m_Alpha >> 8;
					pix_des[i_d] = ExRGBA
						(
						ExGetR(pix_des[i_d]) + r_dif, 
						ExGetG(pix_des[i_d]) + g_dif, 
						ExGetB(pix_des[i_d]) + b_dif, 
						ExGetA(pix_des[i_d]) + a_dif
						);
					++i_d, ++i_s;
				}
			}
			__asm
			{
				pop edi
				pop esi
				pop eax
				pop edx
				pop ecx
				emms
			}
		}
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
		LONG w, LONG h, LONG inx_des, LONG inx_src
		)
	{
		if (m_Alpha == 0) return;

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
		BYTE a_r = (a_d + a_s) - (a_d * a_s >> 8);
		if (a_r == 0) a_r = EXP_CM;
		pix_des[i_d] = ExRGBA
			(
			(r_s * a_s + r_d * a_i) >> 8, 
			(g_s * a_s + g_d * a_i) >> 8, 
			(b_s * a_s + b_d * a_i) >> 8, 
			a_r
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
		LONG w, LONG h, LONG inx_des, LONG inx_src
		)
	{
		if (m_Alpha == 0) return;

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
	EXP_INLINE static bool Render(image_t imgDes, image_t imgSrc, const CRect& rcDes, const CPoint& ptSrc, 
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

		// 格式化区域

		CSize sz_des(exp_des.GetWidth(), exp_des.GetHeight());
		CSize sz_src(exp_src.GetWidth(), exp_src.GetHeight());
		CRect rc_des(rcDes);
		if (rc_des.IsEmpty())
			rc_des.Set(CPoint(), CPoint(sz_des.cx, sz_des.cy));
		CRect rc_src(ptSrc.x, ptSrc.y, sz_src.cx, sz_src.cy);
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
		if (w <= 0 || h <= 0) return true;
		// 计算坐标起点
		LONG inx_des = (sz_des.cy - rc_des.Top() - 1) * sz_des.cx + rc_des.Left();
		LONG inx_src = (sz_src.cy - rc_src.Top() - 1) * sz_src.cx + rc_src.Left();

		// 遍历像素绘图
		pixel_t* pix_des = exp_des.GetPixels();
		pixel_t* pix_src = exp_src.GetPixels();
		if (!pRender) pRender = &EXP_IMG_RENDER();
		pRender->Render(pix_des, pix_src, sz_des, sz_src, w, h, inx_des, inx_src);
		return true;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ImgRenderer_h__*/