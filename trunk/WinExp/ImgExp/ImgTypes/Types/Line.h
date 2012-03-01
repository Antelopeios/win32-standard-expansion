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
// Line - 线
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-21
// Version:	1.0.0002.1008
//
// History:
//	- 1.0.0001.2343(2011-04-20)	+ 添加CLineT::Length()计算两点距离
//								^ 改进CLineT::PtInLine(),支持判断点与直线之间的位置关系
//								- 移除点与线段之间的判断函数
//	- 1.0.0002.1008(2011-04-21)	+ CLineT支持通过一个点和斜率定位直线
//								+ 重新添加CLineT::InterSect线段交点函数
//								# 修正一些疏忽和编译错误
//////////////////////////////////////////////////////////////////

#ifndef __Line_h__
#define __Line_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgTypes/Types/Rect.h"
#include <math.h>

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename TypeT = LONG>
class CLineT
{
public:
	CPointT<TypeT> pt1, pt2;

public:
	CLineT()
	{}
	CLineT(TypeT nX1, TypeT nY1, TypeT nX2, TypeT nY2)
	{ Set(CPointT<TypeT>(nX1, nY1), CPointT<TypeT>(nX2, nY2)); }
	CLineT(const CPointT<TypeT>& Pt1, const CPointT<TypeT>& Pt2)
	{ Set(Pt1, Pt2); }
	CLineT(const CPointT<TypeT>& Pt1, const TypeT k)
	{ Set(Pt1, k); }
	CLineT(const CPointT<TypeT>& Pt1)
	{ Set(Pt1); }
	CLineT(const CRectT<TypeT>& tRect)
	{ (*this) = tRect; }
	CLineT(const CLineT& tLine)
	{ (*this) = tLine; }

public:
	EXP_INLINE void Set(const CPointT<TypeT>& Pt1, const CPointT<TypeT>& Pt2)
	{
		pt1 = Pt1;
		pt2 = Pt2;
	}
	EXP_INLINE void Set(const CPointT<TypeT>& Pt1, const TypeT k)
	{
		Set(Pt1, Pt1 + CPointT<TypeT>(1, k));
	}
	EXP_INLINE void Set(const CPointT<TypeT>& Pt1)
	{
		Set(Pt1, Pt1 + CPointT<TypeT>(0, 1));
	}
	EXP_INLINE void Offset(const CPointT<TypeT>& Pt)
	{
		pt1 += Pt;
		pt2 += Pt;
	}
	EXP_INLINE void MoveTo(const CPointT<TypeT>& Pt)
	{
		CPointT<TypeT> pt(pt2 - pt1);
		pt1 = Pt;
		pt2 = pt1 + pt;
	}

	EXP_INLINE BOOL IsEmpty() const
	{ return (pt1.x == pt2.x || pt1.y == pt2.y); }
	EXP_INLINE BOOL IsNull() const
	{ return (pt1.x == 0 && pt2.x == 0 && pt1.y == 0 && pt2.y == 0); }

	// 求两点距离
	EXP_INLINE TypeT Length() const
	{
		TypeT dif_x = (pt1.x - pt2.x), dif_y = (pt1.y - pt2.y);
		return (TypeT)sqrt((double)(dif_x * dif_x + dif_y * dif_y));
	}

	// 求斜率
	EXP_INLINE BOOL K(_OT_ TypeT& k) const
	{
		TypeT div = pt2.x - pt1.x;
		if (ExIsZero(div)) return FALSE;
		k = (pt2.y - pt1.y) / (pt2.x - pt1.x);
		return TRUE;
	}

	// 点与直线的关系
	EXP_INLINE int PtInLine(const CPointT<TypeT>& Pt) const
	{
		TypeT a = pt2.y - pt1.y;
		TypeT b = pt1.x - pt2.x;
		TypeT c = a * pt1.x + b * pt1.y;
		TypeT r = (a * Pt.x + b * Pt.y - c);
		if (ExIsZero(r))
			return 0;	// 点在直线上
		else
		if ((b > 0 && r > 0) || (b < 0 && r < 0))
			return 1;	// 点在直线上方
		else
			return -1;	// 点在直线下方
	}

	// 求交点
	EXP_INLINE BOOL InterLine(_IN_ const CLineT& tLine, _OT_ CPointT<TypeT>& Pt) const
	{
		TypeT b1 = pt1.x - pt2.x, b2 = tLine.pt1.x - tLine.pt2.x;
		if (ExIsZero(b1) && ExIsZero(b2)) return FALSE;
		TypeT a1 = pt2.y - pt1.y, a2 = tLine.pt2.y - tLine.pt1.y;
		if (ExIsEqual(a1 / b1, a2 / b2)) return FALSE;
		TypeT c1 = a1 * pt1.x + b1 * pt1.y, c2 = a2 * tLine.pt1.x + b2 * tLine.pt1.y;
		TypeT div = a1 * b2 - b1 * a2;
		Pt.x = (b2 * c1 - b1 * c2) / div;
		Pt.y = (a1 * c2 - a2 * c1) / div;
		return TRUE;
	}
	EXP_INLINE BOOL InterSect(_IN_ const CLineT& tLine, _OT_ CPointT<TypeT>& Pt) const
	{
		CPointT<TypeT> pt;
		if(!InterLine(tLine, pt)) return FALSE;
		// 判断x
		if (ExIsEqual(pt1.x, pt2.x))
		{
			if (!ExIsEqual(pt.x, pt1.x))
				return FALSE;
		}
		else
		if (pt1.x < pt2.x)
		{
			if (pt.x < pt1.x || pt.x > pt2.x)
				return FALSE;
		}
		else
		{
			if (pt.x > pt1.x || pt.x < pt2.x)
				return FALSE;
		}
		// 判断y
		if (ExIsEqual(pt1.y, pt2.y))
		{
			if (!ExIsEqual(pt.y, pt1.y))
				return FALSE;
		}
		else
		if (pt1.y < pt2.y)
		{
			if (pt.y < pt1.y || pt.y > pt2.y)
				return FALSE;
		}
		else
		{
			if (pt.y > pt1.y || pt.y < pt2.y)
				return FALSE;
		}
		Pt = pt;
		return TRUE;
	}

	EXP_INLINE CLineT& operator=(const CRectT<TypeT>& tRect)
	{
		Set(tRect.pt1, tRect.pt2);
		return (*this);
	}
	EXP_INLINE CLineT& operator=(const CLineT& tLine)
	{
		Set(tLine.pt1, tLine.pt2);
		return (*this);
	}

	EXP_INLINE BOOL operator==(const CLineT& tLine) const
	{ return ((pt1 == tLine.pt1) && (pt2 == tLine.pt2)); }
	EXP_INLINE BOOL operator!=(const CLineT& tLine) const
	{ return !((*this) == tLine); }

	EXP_INLINE CLineT& operator+=(const CPointT<TypeT>& Pt)
	{
		Offset(Pt);
		return (*this);
	}
	EXP_INLINE CLineT& operator-=(const CPointT<TypeT>& Pt)
	{
		Offset(CPointT<TypeT>(-tPoint.x, -tPoint.y));
		return (*this);
	}

	EXP_INLINE CLineT operator+(const CPointT<TypeT>& Pt) const
	{ return (CLineT(*this) += Pt); }
	EXP_INLINE CLineT operator-(const CPointT<TypeT>& Pt) const
	{ return (CLineT(*this) -= Pt); }
};

typedef CLineT<> CLine;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Line_h__*/