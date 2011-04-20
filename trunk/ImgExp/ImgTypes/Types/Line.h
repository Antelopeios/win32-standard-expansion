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
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-20
// Version:	1.0.0000.2200
//////////////////////////////////////////////////////////////////

#ifndef __Line_h__
#define __Line_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgTypes/Types/Point.h"

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
	CLineT(const CRectT& tRect)
	{ (*this) = tRect; }
	CLineT(const CLineT& tLine)
	{ (*this) = tLine; }

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

	EXP_INLINE bool IsEmpty()
	{ return (pt1.x == pt2.x || pt1.y == pt2.y); }
	EXP_INLINE bool IsNull()
	{ return (pt1.x == 0 && pt2.x == 0 && pt1.y == 0 && pt2.y == 0); }

	// 求斜率
	EXP_INLINE bool K(_OT_ TypeT& k)
	{
		TypeT div = pt2.x - pt1.x;
		if (ExIsZero(div)) return false;
		k = (pt2.y - pt1.y) / (pt2.x - pt1.x);
		return true;
	}

	// 点与直线的关系
	EXP_INLINE bool PtInLine(const CPointT<TypeT>& Pt)
	{
		CLineT l1(pt1, Pt), l2(Pt, pt2);
		TypeT k1 = 0, k2 = 0;
		bool e1 = l1.K(k1), e2 = l2.K(k2);
		return (e1 == e2 && ExIsEqual(k1, k2));
	}
	EXP_INLINE bool PtInSect(const CPointT<TypeT>& Pt)
	{
		// 判断x
		if (ExIsEqual(pt1.x, pt2.x))
		{
			if (!ExIsEqual(Pt.x, pt1.x))
				return false;
		}
		else
		if (pt1.x < pt2.x)
		{
			if (Pt.x < pt1.x || Pt.x > pt2.x)
				return false;
		}
		else
		{
			if (Pt.x > pt1.x || Pt.x < pt2.x)
				return false;
		}
		// 判断y
		if (ExIsEqual(pt1.y, pt2.y))
		{
			if (!ExIsEqual(Pt.y, pt1.y))
				return false;
		}
		else
		if (pt1.y < pt2.y)
		{
			if (Pt.y < pt1.y || Pt.y > pt2.y)
				return false;
		}
		else
		{
			if (Pt.y > pt1.y || Pt.y < pt2.y)
				return false;
		}
		return PtInLine(Pt);
	}

	// 求交点
	EXP_INLINE bool InterLine(const CLineT& tLine, _OT_ CPointT<TypeT>& Pt)
	{
		TypeT b1 = pt1.x - pt2.x, b2 = tLine.pt1.x - tLine.pt2.x;
		if (ExIsZero(b1) && ExIsZero(b2)) return false;
		TypeT a1 = pt2.y - pt1.y, a2 = tLine.pt2.y - tLine.pt1.y;
		if (ExIsEqual(a1 / b1, a2 / b2)) return false;
		TypeT c1 = a1 * pt1.x + b1 * pt1.y, c2 = a2 * tLine.pt1.x + b2 * tLine.pt1.y;
		TypeT div = a1 * b2 - b1 * a1;
		Pt.x = (b2 * c1 - b1 * c2) / div;
		Pt.y = (a1 * c2 - a2 * c1) / div;
		return true;
	}
	EXP_INLINE bool InterSect(const CLineT& tLine, _OT_ CPointT<TypeT>& Pt)
	{
		CPointT<TypeT> pt;
		if (!InterLine(tLine, pt)) return false;
		if (!PtInSect(pt)) return false;
		Pt = pt;
		return true;
	}

	EXP_INLINE CLineT& operator=(const CRectT& tRect)
	{
		Set(tRect.pt1, tRect.pt2);
		return (*this);
	}
	EXP_INLINE CLineT& operator=(const CLineT& tLine)
	{
		Set(tLine.pt1, tLine.pt2);
		return (*this);
	}
	EXP_INLINE bool operator==(const CLineT& tLine)
	{ return ((pt1 == tLine.pt1) && (pt2 == tLine.pt2)); }

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

	EXP_INLINE CRectT operator+(const CPointT<TypeT>& Pt)
	{ return (CRectT(*this) += Pt); }
	EXP_INLINE CRectT operator-(const CPointT<TypeT>& Pt)
	{ return (CRectT(*this) -= Pt); }
};

typedef CLineT<> CLine;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Line_h__*/