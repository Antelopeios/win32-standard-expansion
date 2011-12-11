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
// ImgDeformer - 图像变换器
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-07-07
// Version:	1.0.0002.2340
//
// History:
//	- 1.0.0000.0810(2011-04-22)	= 调整差值函数接口命名
//	- 1.0.0001.1630(2011-04-25)	+ 添加其他变换函数接口
//	- 1.0.0002.2340(2011-07-07)	= 将默认的双线性差值调整为最邻近插值
//////////////////////////////////////////////////////////////////

#ifndef __ImgDeformer_h__
#define __ImgDeformer_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgTypes/Types/Types.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

#ifndef EXP_IMG_INTER
#define EXP_IMG_INTER CImgDeformer::InterNeighbor
#endif/*EXP_IMG_INTER*/

class CImgDeformer
{
public:
	// 差值回调指针定义
	typedef pixel_t (*inter_proc_t)(pixel_t*, CPointT<double>&, CRectT<double>&);
	// 最邻近插值
	static pixel_t InterNeighbor(pixel_t* pixSrc, CPointT<double>& ptSrc, CRectT<double>& rcSrc)
	{
		if (!pixSrc || !rcSrc.PtInRect(ptSrc)) return 0;
		return pixSrc[((LONG)ptSrc.x + (LONG)ptSrc.y * (LONG)rcSrc.Width())];
	}
	// 双线性插值
	static pixel_t InterBilinear(pixel_t* pixSrc, CPointT<double>& ptSrc, CRectT<double>& rcSrc)
	{
		if (!pixSrc) return 0;
		if (!rcSrc.PtInRect(ptSrc))
			return 0;
		// 计算待插值的4个顶点
		LONG lx = (LONG)ptSrc.x;
		LONG ly = (LONG)ptSrc.y;
		double dx = ptSrc.x - lx;
		double dy = ptSrc.y - ly;
		if (dx < EXP_ZERO) dx = 0;
		if (dy < EXP_ZERO) dy = 0;
		LONG w = (LONG)rcSrc.Width(), h = (LONG)rcSrc.Height();
		LONG inx[4] = {0};
		inx[0] = lx + ly * w;
		inx[1] = (lx >= (w - 1)) ? inx[0] : ((dx == 0) ? inx[0] : inx[0] + 1);
		inx[2] = (ly >= (h - 1)) ? inx[0] : ((dy == 0) ? inx[0] : inx[0] + w);
		inx[3] = (lx >= (w - 1)) ? inx[2] : ((dx == 0) ? inx[2] : inx[2] + 1);
		pixel_t pix[4] = 
		{
			pixSrc[inx[0]], 
			pixSrc[inx[1]], 
			pixSrc[inx[2]], 
			pixSrc[inx[3]]
		};
		// 求解插值
		double mx = 1 - dx, my = 1 - dy;
		double d[4] = 
		{
			mx * my, 
			mx * dy, 
			dx * my, 
			dx * dy
		};
		return ExRGBA
			(
			(d[0] * ExGetR(pix[0]) + d[1] * ExGetR(pix[2]) + d[2] * ExGetR(pix[1]) + d[3] * ExGetR(pix[3])), 
			(d[0] * ExGetG(pix[0]) + d[1] * ExGetG(pix[2]) + d[2] * ExGetG(pix[1]) + d[3] * ExGetG(pix[3])), 
			(d[0] * ExGetB(pix[0]) + d[1] * ExGetB(pix[2]) + d[2] * ExGetB(pix[1]) + d[3] * ExGetB(pix[3])), 
			(d[0] * ExGetA(pix[0]) + d[1] * ExGetA(pix[2]) + d[2] * ExGetA(pix[1]) + d[3] * ExGetA(pix[3]))
			);
	}

protected:
	// 生成变换矩阵
	EXP_INLINE static void MakMatrix(_IN_ const CPointT<double> (&ptSrc)[2], _IN_ const CPointT<double> (&ptDes)[2], _OT_ double (&mtxTans)[4])
	{
		double mtx_div = ptSrc[0].x * ptSrc[1].y - ptSrc[0].y * ptSrc[1].x;
		mtxTans[0] = ((ptSrc[1].y * ptDes[0].x) - (ptSrc[0].y * ptDes[1].x)) / mtx_div;
		mtxTans[1] = ((ptSrc[1].y * ptDes[0].y) - (ptSrc[0].y * ptDes[1].y)) / mtx_div;
		mtxTans[2] = ((ptSrc[0].x * ptDes[1].x) - (ptSrc[1].x * ptDes[0].x)) / mtx_div;
		mtxTans[3] = ((ptSrc[0].x * ptDes[1].y) - (ptSrc[1].x * ptDes[0].y)) / mtx_div;
	}
	// 反向矩阵计算
	EXP_INLINE static void InvMatrix(_IN_ const double (&mtxSrc)[4], _OT_ double (&mtxDes)[4])
	{
		double inv_div = mtxSrc[0] * mtxSrc[3] - mtxSrc[2] * mtxSrc[1];
		if (ExIsZero(inv_div)) return;
		mtxDes[0] = (mtxSrc[3] / inv_div);
		mtxDes[1] = -(mtxSrc[1] / inv_div);
		mtxDes[2] = -(mtxSrc[2] / inv_div);
		mtxDes[3] = (mtxSrc[0] / inv_div);
	}

	// 坐标矩阵变换
	EXP_INLINE static void Transform(_IN_ CPointT<double>& ptSrc, _OT_ CPointT<double>& ptDes, _IN_ const double (&mtxTans)[4])
	{
		ptDes.x = ptSrc.x * mtxTans[0] + ptSrc.y * mtxTans[2];
		ptDes.y = ptSrc.x * mtxTans[1] + ptSrc.y * mtxTans[3];
	}
	// 图像矩阵变换
	EXP_INLINE static image_t MtxDeform(_IN_ image_t imgSrc, _IN_ const CPointT<double> (&ptVer)[2], 
										_IN_ const double (&mtxTans)[4], inter_proc_t interProc = EXP_IMG_INTER)
	{
		CImage exp_src;
		exp_src.SetTrust(FALSE);
		exp_src = imgSrc;
		if (exp_src.IsNull()) return NULL;
		// 拿到顶点坐标
		CPointT<double> ver_src[4] = 
		{
			CPointT<double>(0.0f, 0.0f), 
			CPointT<double>((double)exp_src.GetWidth(), 0.0f), 
			CPointT<double>((double)exp_src.GetWidth(), exp_src.GetHeight()), 
			CPointT<double>(0.0f, (double)exp_src.GetHeight())
		};
		CPointT<double> ver_des[4] = 
		{
			CPointT<double>(0.0f, 0.0f), 
			CPointT<double>(ptVer[0].x, ptVer[0].y), 
			CPointT<double>(ptVer[1].x, ptVer[1].y), 
			CPointT<double>(ptVer[1].x - ptVer[0].x, ptVer[1].y - ptVer[0].y)
		};
		// 得到目标图像的宽与高
		LONG min_x = (LONG)min(min(ver_des[0].x, ver_des[1].x), min(ver_des[2].x, ver_des[3].x));
		LONG max_x = (LONG)max(max(ver_des[0].x, ver_des[1].x), max(ver_des[2].x, ver_des[3].x));
		LONG min_y = (LONG)min(min(ver_des[0].y, ver_des[1].y), min(ver_des[2].y, ver_des[3].y));
		LONG max_y = (LONG)max(max(ver_des[0].y, ver_des[1].y), max(ver_des[2].y, ver_des[3].y));
		LONG w_des = max_x - min_x;
		LONG h_des = max_y - min_y;
		double matrix[4] = 
		{
			mtxTans[0], mtxTans[1], 
			mtxTans[2], mtxTans[3]
		};
		// 创建目标图像
		CImage exp_des;
		exp_des.SetTrust(FALSE);
		exp_des.Create(w_des, h_des);
		if (exp_des.IsNull()) return NULL;
		// 映射坐标点
		if (!interProc) interProc = EXP_IMG_INTER;
		CRectT<double> rc_src(0.0f, 0.0f, exp_src.GetWidth(), exp_src.GetHeight());
		CPointT<double> crd_src, crd_des;
		pixel_t* pix_src = exp_src.GetPixels();
		pixel_t* pix_des = exp_des.GetPixels();
		LONG y = 0;
		LONG crd_y = h_des - y + min_y;
		for(; y < h_des; ++y, --crd_y)
		{
			LONG x = 0;
			LONG crd_x = x + min_x;
			for(; x < w_des; ++x, ++crd_x)
			{
				// 存储目标坐标
				crd_des.Set((double)crd_x, (double)crd_y);
				// 反向映射坐标
				Transform(crd_des, crd_src, matrix);
				// 改写目标像素
				crd_src.y = exp_src.GetHeight() - crd_src.y;
				pix_des[x + y * w_des] = interProc(pix_src, crd_src, rc_src);
			}
		}
		return exp_des.Get();
	}

public:
	// 矩阵变换
	EXP_INLINE static image_t MtxDeform(_IN_ image_t imgSrc, _IN_ const double (&mtxTans)[4], inter_proc_t interProc = EXP_IMG_INTER)
	{
		CImage exp_src;
		exp_src.SetTrust(FALSE);
		exp_src = imgSrc;
		if (exp_src.IsNull()) return NULL;
		// 拿到顶点坐标
		CPointT<double> ver_src[2] = 
		{
			CPointT<double>(exp_src.GetWidth(), 0), 
			CPointT<double>(exp_src.GetWidth(), exp_src.GetHeight()), 
		};
		CPointT<double> ver_des[2];
		Transform(ver_src[0], ver_des[0], mtxTans);
		Transform(ver_src[1], ver_des[1], mtxTans);
		// 计算形变矩阵
		double matrix[4] = {0};
		InvMatrix(mtxTans, matrix);
		// 图像变形
		return MtxDeform(imgSrc, ver_des, matrix, interProc);
	}
	// 平行四边形变换
	EXP_INLINE static image_t PlgDeform(_IN_ image_t imgSrc, _IN_ const CPoint (&ptVer)[2], inter_proc_t interProc = EXP_IMG_INTER)
	{
		CImage exp_src;
		exp_src.SetTrust(FALSE);
		exp_src = imgSrc;
		if (exp_src.IsNull()) return NULL;
		// 拿到顶点坐标
		CPointT<double> ver_src[2] = 
		{
			CPointT<double>(exp_src.GetWidth(), 0), 
			CPointT<double>(exp_src.GetWidth(), exp_src.GetHeight())
		};
		CPointT<double> ver_des[2] = 
		{
			CPointT<double>(ptVer[0].x, ptVer[0].y), 
			CPointT<double>(ptVer[1].x, ptVer[1].y)
		};
		// 计算形变矩阵
		double matrix[4] = {0};
		MakMatrix(ver_des, ver_src, matrix);
		// 图像变形
		return MtxDeform(imgSrc, ver_des, matrix, interProc);
	}
	// 旋转变换
	EXP_INLINE static image_t WhlDeform(_IN_ image_t imgSrc, _IN_ int16_t nDegree, inter_proc_t interProc = EXP_IMG_INTER)
	{
		CImage exp_src;
		exp_src.SetTrust(FALSE);
		exp_src = imgSrc;
		if (exp_src.IsNull()) return NULL;
		// 角度换算弧度
		nDegree = nDegree % 360;
		if (nDegree == 0) return NULL;
		double radian = (double)EXP_PI * (nDegree / 180);
		// 计算形变矩阵
		double matrix[4] = 
		{
			cos(radian), sin(radian), 
			-sin(radian), cos(radian)
		};
		// 图像变形
		return MtxDeform(imgSrc, matrix, interProc);
	}
	// 缩放变换
	EXP_INLINE static image_t ZomDeform(_IN_ image_t imgSrc, _IN_ LONG nW, _IN_ LONG nH, inter_proc_t interProc = EXP_IMG_INTER)
	{
		CImage exp_src;
		exp_src.SetTrust(FALSE);
		exp_src = imgSrc;
		if (exp_src.IsNull()) return NULL;
		// 计算形变矩阵
		double matrix[4] = 
		{
			(double)nW / exp_src.GetWidth(), 0, 
			0, (double)nH / exp_src.GetHeight()
		};
		// 图像变形
		return MtxDeform(imgSrc, matrix, interProc);
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ImgDeformer_h__*/