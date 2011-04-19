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
// ImgRender - 图像渲染器
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-11
// Version:	1.0.0000.1100
//////////////////////////////////////////////////////////////////

#ifndef __ImgRender_h__
#define __ImgRender_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgCommon/ImgCommon.h"
#include "ImgTypes/Types/Types.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CImgRender
{
public:
	// 坐标矩阵变换
	EXP_INLINE static void Transform(_IN_ CPointT<double>& ptSrc, _OT_ CPointT<double>& ptDes, _IN_ DWORD mtxTans[6])
	{
		ptDes.m_X = ptSrc.m_X * mtxTans[0] + ptSrc.m_Y * mtxTans[2] + mtxTans[4];
		ptDes.m_Y = ptSrc.m_X * mtxTans[1] + ptSrc.m_Y * mtxTans[3] + mtxTans[5];
	}
	// 图像转换
	EXP_INLINE static image_t TransImage(_IN_ image_t imgSrc, _IN_ DWORD mtxTans[6])
	{
		CImage exp_src(imgSrc);
		if (exp_src.IsNull()) return NULL;
		// 拿到顶点坐标
		CPointT<double> ver_src[4] = 
		{
			CPointT<double>(0, 0), 
			CPointT<double>(exp_src.GetWidth(), 0), 
			CPointT<double>(exp_src.GetWidth(), exp_src.GetHeight()), 
			CPointT<double>(0, exp_src.GetHeight())
		};
		CPointT<double> ver_des[4];
		Transform(ver_src[0], ver_des[0], mtxTans);
		Transform(ver_src[1], ver_des[1], mtxTans);
		Transform(ver_src[2], ver_des[2], mtxTans);
		Transform(ver_src[3], ver_des[3], mtxTans);
		// 得到目标图像的宽与高
		LONG min_x = (LONG)min(min(ver_des[0].m_X, ver_des[1].m_X), min(ver_des[2].m_X, ver_des[3].m_X));
		LONG max_x = (LONG)max(max(ver_des[0].m_X, ver_des[1].m_X), max(ver_des[2].m_X, ver_des[3].m_X));
		LONG min_y = (LONG)min(min(ver_des[0].m_Y, ver_des[1].m_Y), min(ver_des[2].m_Y, ver_des[3].m_Y));
		LONG max_y = (LONG)max(max(ver_des[0].m_Y, ver_des[1].m_Y), max(ver_des[2].m_Y, ver_des[3].m_Y));
		DWORD w_des = max_x - min_x;
		DWORD h_des = max_y - min_y;
		// 增加平移坐标矩阵
		DWORD matrix[6] = 
		{
			mtxTans[0], 
			mtxTans[1], 
			mtxTans[2], 
			mtxTans[3], 
			mtxTans[4] - min_x, 
			mtxTans[5] - min_y
		};
		// 创建目标图像
		CImage exp_des;
		exp_des.Create(w_des, h_des);
		// 映射坐标点
		CRectT<double> rc_src(0, 0, exp_src.GetWidth(), exp_src.GetHeight());
		CPointT<double> crd_src, crd_des;
		for(DWORD y = 0; y < h_des; ++y)
		{
			for(DWORD x = 0; x < w_des; ++x)
			{
				crd_des.Set(x, y);
				Transform(crd_des, crd_src, matrix);
			}
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ImgRender_h__*/