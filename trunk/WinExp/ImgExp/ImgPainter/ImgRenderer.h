// Copyright 2011-2012, 木头云
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
// Date:	2012-01-03
// Version:	1.0.0017.1925
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
//	- 1.0.0014.1540(2011-08-10)	^ 为CImgRenderer::Render()增加默认参数
//	- 1.0.0015.2300(2011-12-20)	# 修正原先渲染算法中的计算误差
//								^ 使用sse优化CRenderNormal与CRenderOverlay
//	- 1.0.0016.1737(2011-12-22)	# 修正v.0015中sse算法里的错误
//	- 1.0.0017.1925(2012-01-03)	^ 采用CImgASM中的算法实现CRenderCopy与CRenderNormal
//								- 移除CRenderOverlay(因采用预乘图片进行渲染,此算法已不再需要)
//////////////////////////////////////////////////////////////////

#ifndef __ImgRenderer_h__
#define __ImgRenderer_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgTypes/Image.h"
#include "ImgTypes/Types/Size.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// 渲染器接口
interface IRenderObject : INonCopyable
{
	virtual void Render(
		pixel_t* pix_des, pixel_t* pix_src, CSize& sz_des, CSize& sz_src, 
		LONG w, LONG h, LONG inx_des, LONG inx_src) = 0;
};

//////////////////////////////////////////////////////////////////

// 像素拷贝
class CRenderCopy : public IRenderObject
{
protected:
	chann_t m_Alpha;

public:
	CRenderCopy(chann_t a = EXP_CM)
		: m_Alpha(a)
	{}

	void Render(
		pixel_t* pix_des, pixel_t* pix_src, CSize& sz_des, CSize& sz_src, 
		LONG w, LONG h, LONG inx_des, LONG inx_src)
	{
		for(LONG y = 0; y < h; ++y, inx_des -= sz_des.cx, inx_src -= sz_src.cx)
			CImgASM::PixTra(pix_des + inx_des, pix_src + inx_src, w, m_Alpha);
	}
};

//////////////////////////////////////////////////////////////////

// 正常渲染
class CRenderNormal : public IRenderObject
{
public:
	void Render(
		pixel_t* pix_des, pixel_t* pix_src, CSize& sz_des, CSize& sz_src, 
		LONG w, LONG h, LONG inx_des, LONG inx_src)
	{
		for(LONG y = 0; y < h; ++y, inx_des -= sz_des.cx, inx_src -= sz_src.cx)
			CImgASM::PixBlend(pix_des + inx_des, pix_src + inx_src, w);
	}
};

#ifndef EXP_IMG_RENDER
#define EXP_IMG_RENDER CRenderNormal
#endif/*EXP_IMG_RENDER*/

//////////////////////////////////////////////////////////////////

class CImgRenderer
{
public:
	EXP_INLINE static BOOL Render(image_t imgDes, image_t imgSrc, 
								  const CRect& rcDes = CRect(), 
								  const CPoint& ptSrc = CPoint(), 
								  IRenderObject* pRender = NULL)
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
		if (w <= 0 || h <= 0) return TRUE;
		// 计算坐标起点
		LONG inx_des = (sz_des.cy - rc_des.Top() - 1) * sz_des.cx + rc_des.Left();
		LONG inx_src = (sz_src.cy - rc_src.Top() - 1) * sz_src.cx + rc_src.Left();

		// 遍历像素绘图
		pixel_t* pix_des = exp_des.GetPixels();
		pixel_t* pix_src = exp_src.GetPixels();
		if (!pRender) pRender = &EXP_IMG_RENDER();
		pRender->Render(pix_des, pix_src, sz_des, sz_src, w, h, inx_des, inx_src);
		return TRUE;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ImgRenderer_h__*/