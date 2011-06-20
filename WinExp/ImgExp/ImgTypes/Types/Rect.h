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
// Rect - 矩形
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-18
// Version:	1.0.0007.1546
//
// History:
//	- 1.0.0002.2350(2011-04-19)	+ CRect改为CRectT<>,支持通过模板参数控制内部数据的类型
//								+ 添加CRectT::IsEmpty()与CRectT::IsNull()接口
//								= CRectT::PtInRect判断坐标时不包括最右边与最下边
//	- 1.0.0003.2200(2011-04-20)	^ 简化CRectT的内部数据命名
//								# 修正CRectT::MoveTo()的错误算法
//	- 1.0.0004.1800(2011-04-21)	+ CRectT::PtInRect()支持自定义是否忽略右边界与下边界
//	- 1.0.0005.1452(2011-04-27)	# 修正CRectT::Inter()与CRectT::Union()中的错误
//	- 1.0.0006.1730(2011-05-05)	# 修正构造时传入基本数据类型将导致堆栈溢出的问题
//	- 1.0.0007.1546(2011-05-18)	+ 添加CRectT::operator!=()与CRectT::operator RECT()
//////////////////////////////////////////////////////////////////

#ifndef __Rect_h__
#define __Rect_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgTypes/Types/Point.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename TypeT = LONG>
class CRectT
{
public:
	CPointT<TypeT> pt1, pt2;

public:
	CRectT()
	{}
	CRectT(TypeT nX1, TypeT nY1, TypeT nX2, TypeT nY2)
	{ Set(CPointT<TypeT>(nX1, nY1), CPointT<TypeT>(nX2, nY2)); }
	CRectT(const CPointT<TypeT>& Pt1, const CPointT<TypeT>& Pt2)
	{ Set(Pt1, Pt2); }
	CRectT(const CRectT& tRect)
	{ (*this) = tRect; }
	CRectT(RECT& tRect)
	{ (*this) = tRect; }

public:
	EXP_INLINE void Set(const CPointT<TypeT>& Pt1, const CPointT<TypeT>& Pt2)
	{
		pt1 = Pt1;
		pt2 = Pt2;
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

	EXP_INLINE void Inter(const CRectT& tRect)
	{
		Set(CPointT<TypeT>(max(pt1.x, tRect.pt1.x), max(pt1.y, tRect.pt1.y)), 
			CPointT<TypeT>(min(pt2.x, tRect.pt2.x), min(pt2.y, tRect.pt2.y)));
	}
	EXP_INLINE void Union(const CRectT& tRect)
	{
		Set(CPointT<TypeT>(min(pt1.x, tRect.pt1.x), min(pt1.y, tRect.pt1.y)), 
			CPointT<TypeT>(max(pt2.x, tRect.pt2.x), max(pt2.y, tRect.pt2.y)));
	}

	EXP_INLINE void Inflate(const CPointT<TypeT>& Pt)
	{
		pt1.x -= Pt.x;
		pt2.x += Pt.x;
		pt1.y -= Pt.y;
		pt2.y += Pt.y;
	}
	EXP_INLINE void Inflate(const CRectT& tRect)
	{
		pt1 -= tRect.pt1;
		pt2 += tRect.pt2;
	}
	EXP_INLINE void Deflate(const CPointT<TypeT>& Pt)
	{
		pt1.x += Pt.x;
		pt2.x -= Pt.x;
		pt1.y += Pt.y;
		pt2.y -= Pt.y;
	}
	EXP_INLINE void Deflate(const CRectT& tRect)
	{
		pt1 += tRect.pt1;
		pt2 -= tRect.pt2;
	}

	EXP_INLINE bool IsEmpty()
	{ return (pt1.x == pt2.x || pt1.y == pt2.y); }
	EXP_INLINE bool IsNull()
	{ return (pt1.x == 0 && pt2.x == 0 && pt1.y == 0 && pt2.y == 0); }

	EXP_INLINE bool PtInRect(const CPointT<TypeT>& Pt, bool bEgSide = true)
	{
		if (bEgSide)
			return (Pt.x >= pt1.x && Pt.x < pt2.x && 
					Pt.y >= pt1.y && Pt.y < pt2.y);
		else
			return (Pt.x >= pt1.x && Pt.x <= pt2.x && 
					Pt.y >= pt1.y && Pt.y <= pt2.y);
	}

	EXP_INLINE CRectT& operator=(const CRectT& tRect)
	{
		Set(tRect.pt1, tRect.pt2);
		return (*this);
	}
	EXP_INLINE CRectT& operator=(RECT& tRect)
	{
		Set(CPointT<TypeT>(tRect.left, tRect.top), 
			CPointT<TypeT>(tRect.right, tRect.bottom));
		return (*this);
	}
	EXP_INLINE bool operator==(const CRectT& tRect)
	{ return ((pt1 == tRect.pt1) && (pt2 == tRect.pt2)); }
	EXP_INLINE bool operator==(RECT& tRect)
	{
		return ((pt1 == CPointT<TypeT>(tRect.left, tRect.top)) && 
				(pt2 == CPointT<TypeT>(tRect.right, tRect.bottom)));
	}
	EXP_INLINE bool operator!=(const CRectT& tRect)
	{ return !((*this) == tRect); }
	EXP_INLINE bool operator!=(RECT& tRect)
	{ return !((*this) == tRect); }

	EXP_INLINE CRectT& operator+=(const CPointT<TypeT>& Pt)
	{
		Inflate(Pt);
		return (*this);
	}
	EXP_INLINE CRectT& operator-=(const CPointT<TypeT>& Pt)
	{
		Deflate(Pt);
		return (*this);
	}
	EXP_INLINE CRectT& operator+=(const CRectT& tRect)
	{
		Inflate(tRect);
		return (*this);
	}
	EXP_INLINE CRectT& operator-=(const CRectT& tRect)
	{
		Deflate(tRect);
		return (*this);
	}

	EXP_INLINE CRectT operator+(const CPointT<TypeT>& Pt)
	{ return (CRectT(*this) += Pt); }
	EXP_INLINE CRectT operator-(const CPointT<TypeT>& Pt)
	{ return (CRectT(*this) -= Pt); }
	EXP_INLINE CRectT operator+(const CRectT& tRect)
	{ return (CRectT(*this) += tRect); }
	EXP_INLINE CRectT operator-(const CRectT& tRect)
	{ return (CRectT(*this) -= tRect); }

	EXP_INLINE operator RECT()
	{
		RECT rc = {pt1.x, pt1.y, pt2.x, pt2.y};
		return rc;
	}

	EXP_INLINE TypeT Left()		{ return pt1.x; }
	EXP_INLINE TypeT Top()		{ return pt1.y; }
	EXP_INLINE TypeT Right()	{ return pt2.x; }
	EXP_INLINE TypeT Bottom()	{ return pt2.y; }
	EXP_INLINE TypeT Width()	{ return Right() - Left(); }
	EXP_INLINE TypeT Height()	{ return Bottom() - Top(); }
};

typedef CRectT<> CRect;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Rect_h__*/