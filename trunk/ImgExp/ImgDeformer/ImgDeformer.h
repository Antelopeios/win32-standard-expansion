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
		ptDes.m_X = ptSrc.m_X * mtxTans[0] + ptSrc.m_Y * mtxTans[2] + mtxTans[4];
		ptDes.m_Y = ptSrc.m_X * mtxTans[1] + ptSrc.m_Y * mtxTans[3] + mtxTans[5];
	}

	// 差值回调指针定义
	typedef pixel_t (*inter_proc_t)(pixel_t*, CPointT<double>&, CRectT<double>&);
	// 最邻近插值
	static pixel_t Neighbor(pixel_t* pixSrc, CPointT<double>& ptSrc, CRectT<double>& rcSrc)
	{
		if (!pixSrc) return 0;
		if (!rcSrc.PtInRect(ptSrc)) return 0;
		return pixSrc[((DWORD)ptSrc.m_X + (DWORD)ptSrc.m_Y * (DWORD)rcSrc.Width())];
	}
	// 双线性插值
	static pixel_t Bilinear(pixel_t* pixSrc, CPointT<double>& ptSrc, CRectT<double>& rcSrc)
	{
		if (!pixSrc) return 0;
		if (!rcSrc.PtInRect(ptSrc)) return 0;
		return 0;
	}

	// 图像转换
	EXP_INLINE static image_t Deform(_IN_ image_t imgSrc, _IN_ const double (&mtxTans)[4], inter_proc_t interProc = Neighbor)
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
		LONG min_x = (LONG)min(min(ver_des[0].m_X, ver_des[1].m_X), min(ver_des[2].m_X, ver_des[3].m_X));
		LONG max_x = (LONG)max(max(ver_des[0].m_X, ver_des[1].m_X), max(ver_des[2].m_X, ver_des[3].m_X));
		LONG min_y = (LONG)min(min(ver_des[0].m_Y, ver_des[1].m_Y), min(ver_des[2].m_Y, ver_des[3].m_Y));
		LONG max_y = (LONG)max(max(ver_des[0].m_Y, ver_des[1].m_Y), max(ver_des[2].m_Y, ver_des[3].m_Y));
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