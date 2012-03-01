// Copyright 2011-2012, ľͷ��
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
// Rect - ����
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-04
// Version:	1.0.0010.2204
//
// History:
//	- 1.0.0002.2350(2011-04-19)	+ CRect��ΪCRectT<>,֧��ͨ��ģ����������ڲ����ݵ�����
//								+ ���CRectT::IsEmpty()��CRectT::IsNull()�ӿ�
//								= CRectT::PtInRect�ж�����ʱ���������ұ������±�
//	- 1.0.0003.2200(2011-04-20)	^ ��CRectT���ڲ���������
//								# ����CRectT::MoveTo()�Ĵ����㷨
//	- 1.0.0004.1800(2011-04-21)	+ CRectT::PtInRect()֧���Զ����Ƿ�����ұ߽����±߽�
//	- 1.0.0005.1452(2011-04-27)	# ����CRectT::Inter()��CRectT::Union()�еĴ���
//	- 1.0.0006.1730(2011-05-05)	# ��������ʱ��������������ͽ����¶�ջ���������
//	- 1.0.0007.1546(2011-05-18)	+ ���CRectT::operator!=()��CRectT::operator RECT()
//	- 1.0.0008.2229(2011-08-30)	+ ��ӿ��Է���ֱ�۵��޸�CRectT��Left;Right�����ԵĽӿ�
//								+ ���ֱ�ӻ�ȡ���޸Ķ���Ľӿ�
//	- 1.0.0009.1308(2011-08-31)	= ����CRectT::IsEmpty()�ӿ�,�������СΪ����ʱҲ�ж�ΪTRUE
//	- 1.0.0010.2204(2012-01-04)	+ CRectT�������޸Ľӿھ��ṩ����ֵ
//								+ ����µ�CRectT::Set(),֧��ֱ��ͨ����ֵ����CRectT
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
	EXP_INLINE CRectT& Set(const CPointT<TypeT>& Pt1, const CPointT<TypeT>& Pt2)
	{
		pt1 = Pt1;
		pt2 = Pt2;
		return (*this);
	}
	EXP_INLINE CRectT& Set(TypeT nX1, TypeT nY1, TypeT nX2, TypeT nY2)
	{
		return Set(CPointT<TypeT>(nX1, nY1), CPointT<TypeT>(nX2, nY2));
	}
	EXP_INLINE CRectT& Offset(const CPointT<TypeT>& Pt)
	{
		pt1 += Pt;
		pt2 += Pt;
		return (*this);
	}
	EXP_INLINE CRectT& MoveTo(const CPointT<TypeT>& Pt)
	{
		CPointT<TypeT> pt(pt2 - pt1);
		pt1 = Pt;
		pt2 = pt1 + pt;
		return (*this);
	}

	EXP_INLINE CRectT& Inter(const CRectT& tRect)
	{
		return Set(CPointT<TypeT>(max(pt1.x, tRect.pt1.x), max(pt1.y, tRect.pt1.y)), 
				   CPointT<TypeT>(min(pt2.x, tRect.pt2.x), min(pt2.y, tRect.pt2.y)));
	}
	EXP_INLINE CRectT& Union(const CRectT& tRect)
	{
		return Set(CPointT<TypeT>(min(pt1.x, tRect.pt1.x), min(pt1.y, tRect.pt1.y)), 
				   CPointT<TypeT>(max(pt2.x, tRect.pt2.x), max(pt2.y, tRect.pt2.y)));
	}

	EXP_INLINE CRectT& Inflate(const CPointT<TypeT>& Pt)
	{
		pt1.x -= Pt.x;
		pt2.x += Pt.x;
		pt1.y -= Pt.y;
		pt2.y += Pt.y;
		return (*this);
	}
	EXP_INLINE CRectT& Inflate(const CRectT& tRect)
	{
		pt1 -= tRect.pt1;
		pt2 += tRect.pt2;
		return (*this);
	}
	EXP_INLINE CRectT& Deflate(const CPointT<TypeT>& Pt)
	{
		pt1.x += Pt.x;
		pt2.x -= Pt.x;
		pt1.y += Pt.y;
		pt2.y -= Pt.y;
		return (*this);
	}
	EXP_INLINE CRectT& Deflate(const CRectT& tRect)
	{
		pt1 += tRect.pt1;
		pt2 -= tRect.pt2;
		return (*this);
	}

	EXP_INLINE BOOL IsEmpty() const
	{ return (pt1.x >= pt2.x || pt1.y >= pt2.y); }
	EXP_INLINE BOOL IsNull() const
	{ return (pt1.x == 0 && pt2.x == 0 && pt1.y == 0 && pt2.y == 0); }

	EXP_INLINE BOOL PtInRect(const CPointT<TypeT>& Pt, BOOL bEgSide = TRUE) const
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
		return Set(tRect.pt1, tRect.pt2);
	}
	EXP_INLINE CRectT& operator=(RECT& tRect)
	{
		return Set(CPointT<TypeT>(tRect.left, tRect.top), 
				   CPointT<TypeT>(tRect.right, tRect.bottom));
	}
	EXP_INLINE BOOL operator==(const CRectT& tRect) const
	{ return ((pt1 == tRect.pt1) && (pt2 == tRect.pt2)); }
	EXP_INLINE BOOL operator==(RECT& tRect) const
	{
		return ((pt1 == CPointT<TypeT>(tRect.left, tRect.top)) && 
				(pt2 == CPointT<TypeT>(tRect.right, tRect.bottom)));
	}
	EXP_INLINE BOOL operator!=(const CRectT& tRect) const
	{ return !((*this) == tRect); }
	EXP_INLINE BOOL operator!=(RECT& tRect) const
	{ return !((*this) == tRect); }

	EXP_INLINE CRectT& operator+=(const CPointT<TypeT>& Pt)
	{
		return Inflate(Pt);
	}
	EXP_INLINE CRectT& operator-=(const CPointT<TypeT>& Pt)
	{
		return Deflate(Pt);
	}
	EXP_INLINE CRectT& operator+=(const CRectT& tRect)
	{
		return Inflate(tRect);
	}
	EXP_INLINE CRectT& operator-=(const CRectT& tRect)
	{
		return Deflate(tRect);
	}

	EXP_INLINE CRectT operator+(const CPointT<TypeT>& Pt) const
	{ return (CRectT(*this) += Pt); }
	EXP_INLINE CRectT operator-(const CPointT<TypeT>& Pt) const
	{ return (CRectT(*this) -= Pt); }
	EXP_INLINE CRectT operator+(const CRectT& tRect) const
	{ return (CRectT(*this) += tRect); }
	EXP_INLINE CRectT operator-(const CRectT& tRect) const
	{ return (CRectT(*this) -= tRect); }

	EXP_INLINE operator RECT() const
	{
		RECT rc = {pt1.x, pt1.y, pt2.x, pt2.y};
		return rc;
	}

	EXP_INLINE TypeT Left() const	{ return pt1.x; }
	EXP_INLINE TypeT Top() const	{ return pt1.y; }
	EXP_INLINE TypeT Right() const	{ return pt2.x; }
	EXP_INLINE TypeT Bottom() const	{ return pt2.y; }
	EXP_INLINE TypeT Width() const	{ return Right() - Left(); }
	EXP_INLINE TypeT Height() const	{ return Bottom() - Top(); }

	EXP_INLINE TypeT Left(TypeT nSize)
	{ return pt1.x = nSize; }
	EXP_INLINE TypeT Top(TypeT nSize)
	{ return pt1.y = nSize; }
	EXP_INLINE TypeT Right(TypeT nSize)
	{ return pt2.x = nSize; }
	EXP_INLINE TypeT Bottom(TypeT nSize)
	{ return pt2.y = nSize; }
	EXP_INLINE TypeT Width(TypeT nSize)
	{
		Right(Left() + nSize);
		return Width();
	}
	EXP_INLINE TypeT Height(TypeT nSize)
	{
		Bottom(Top() + nSize);
		return Height();
	}

	EXP_INLINE CPointT<TypeT> LeftTop() const		{ return pt1; }
	EXP_INLINE CPointT<TypeT> RightTop() const		{ return CPointT<TypeT>(pt2.x, pt1.y); }
	EXP_INLINE CPointT<TypeT> LeftBottom() const	{ return CPointT<TypeT>(pt1.x, pt2.y); }
	EXP_INLINE CPointT<TypeT> RightBottom() const	{ return pt2; }

	EXP_INLINE CPointT<TypeT>& LeftTop(CPointT<TypeT>& pt)
	{ return pt1 = pt; }
	EXP_INLINE CPointT<TypeT> RightTop(CPointT<TypeT>& pt)
	{ return CPointT<TypeT>(pt2.x = pt.x, pt1.y = pt.y); }
	EXP_INLINE CPointT<TypeT> LeftBottom(CPointT<TypeT>& pt)
	{ return CPointT<TypeT>(pt1.x = pt.x, pt2.y = pt.y); }
	EXP_INLINE CPointT<TypeT>& RightBottom(CPointT<TypeT>& pt)
	{ return pt2 = pt; }
};

typedef CRectT<> CRect;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Rect_h__*/