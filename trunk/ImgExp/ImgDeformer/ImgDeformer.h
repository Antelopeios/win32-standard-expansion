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
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-11
// Version:	1.0.0000.1100
//////////////////////////////////////////////////////////////////

#ifndef __ImgDeformer_h__
#define __ImgDeformer_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgCommon/ImgCommon.h"
#include "ImgTypes/Types/Types.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CImgDeformer
{
public:
	// 坐标矩阵变换
	EXP_INLINE static void Transform(_IN_ CPointT<double>& ptSrc, _OT_ CPointT<double>& ptDes, _IN_ const double (&mtxTans)[6])
	{
		ptDes.x = ptSrc.x * mtxTans[0] + ptSrc.y * mtxTans[2] + mtxTans[4];
		ptDes.y = ptSrc.x * mtxTans[1] + ptSrc.y * mtxTans[3] + mtxTans[5];
	}

public:
	// 差值回调指针定义
	typedef pixel_t (*inter_proc_t)(pixel_t*, CPointT<double>&, CRectT<double>&);
	// 最邻近插值
	static pixel_t Neighbor(pixel_t* pixSrc, CPointT<double>& ptSrc, CRectT<double>& rcSrc)
	{
		if (!pixSrc) return 0;
		if (!rcSrc.PtInRect(ptSrc)) return 0;
		return pixSrc[((LONG)ptSrc.x + (LONG)ptSrc.y * (LONG)rcSrc.Width())];
	}
	// 双线性插值
	static pixel_t Bilinear(pixel_t* pixSrc, CPointT<double>& ptSrc, CRectT<double>& rcSrc)
	{
		if (!pixSrc) return 0;
		if (!rcSrc.PtInRect(ptSrc)) return 0;
		// 计算待差值的4个顶点
		LONG lx = (LONG)ptSrc.x;
		LONG ly = (LONG)ptSrc.y;
		double dx = ptSrc.x - lx;
		double dy = ptSrc.y - ly;
		if (dx < 0.00000001) dx = 0;
		if (dy < 0.00000001) dy = 0;
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
		// 求解差值
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

public:
	// 图像转换
	EXP_INLINE static image_t Deform(_IN_ image_t imgSrc, _IN_ const double (&mtxTans)[4], inter_proc_t interProc = Bilinear/*Neighbor*/)
	{
		CImage exp_src(imgSrc);
		if (exp_src.IsNull()) return NULL;
		// 拿到顶点坐标
		CPointT<double> ver_src[4] = 
		{
			CPointT<double>(0.0f, 0.0f), 
			CPointT<double>(exp_src.GetWidth(), 0.0f), 
			CPointT<double>(exp_src.GetWidth(), exp_src.GetHeight()), 
			CPointT<double>(0.0f, exp_src.GetHeight())
		};
		CPointT<double> ver_des[4];
		double matrix[6] = 
		{
			mtxTans[0], mtxTans[1], 
			mtxTans[2], mtxTans[3], 
			0.0f, 0.0f
		};
		Transform(ver_src[0], ver_des[0], matrix);
		Transform(ver_src[1], ver_des[1], matrix);
		Transform(ver_src[2], ver_des[2], matrix);
		Transform(ver_src[3], ver_des[3], matrix);
		// 得到目标图像的宽与高
		LONG min_x = (LONG)min(min(ver_des[0].x, ver_des[1].x), min(ver_des[2].x, ver_des[3].x));
		LONG max_x = (LONG)max(max(ver_des[0].x, ver_des[1].x), max(ver_des[2].x, ver_des[3].x));
		LONG min_y = (LONG)min(min(ver_des[0].y, ver_des[1].y), min(ver_des[2].y, ver_des[3].y));
		LONG max_y = (LONG)max(max(ver_des[0].y, ver_des[1].y), max(ver_des[2].y, ver_des[3].y));
		DWORD w_des = max_x - min_x;
		DWORD h_des = max_y - min_y;
		// 增加平移坐标矩阵
		matrix[4] -= min_x;
		matrix[5] -= min_y;
		// 创建目标图像
		CImage exp_des;
		exp_des.Create(w_des, h_des);
		if (exp_des.IsNull()) return NULL;
		// 计算反向映射矩阵
		double inv_div = matrix[0] * matrix[3] - matrix[2] * matrix[1];
		if (inv_div < 0.00000001 && inv_div > -0.00000001) return NULL;
		double inv_mtx[6] = 
		{
			(matrix[3] / inv_div), -(matrix[1] / inv_div), 
			-(matrix[2] / inv_div), (matrix[0] / inv_div), 
			((matrix[2] * matrix[5] - matrix[4] * matrix[3]) / inv_div), 
			((matrix[4] * matrix[1] - matrix[0] * matrix[5]) / inv_div)
		};
		// 映射坐标点
		if (!interProc) interProc = Neighbor;
		CRectT<double> rc_src(0.0f, 0.0f, exp_src.GetWidth(), exp_src.GetHeight());
		CPointT<double> crd_src, crd_des;
		pixel_t* pix_src = exp_src.GetPixels();
		pixel_t* pix_des = exp_des.GetPixels();
		for(DWORD y = 0; y < h_des; ++y)
		{
			for(DWORD x = 0; x < w_des; ++x)
			{
				// 存储目标坐标
				crd_des.Set(x, y);
				// 反向映射坐标
				Transform(crd_des, crd_src, inv_mtx);
				// 改写目标像素
				pix_des[x + y * w_des] = interProc(pix_src, crd_src, rc_src);
			}
		}
		return exp_des.Get();
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ImgDeformer_h__*/