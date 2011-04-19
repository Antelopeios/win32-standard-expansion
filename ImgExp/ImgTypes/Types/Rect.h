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
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-19
// Version:	1.0.0002.2350
//
// History:
//	- 1.0.0002.2350(2011-04-19)	+ CRect改为CRectT<>,支持通过模板参数控制内部数据的类型
//								+ 添加CRectT::IsEmpty()与CRectT::IsNull()接口
//								= CRectT::PtInRect判断坐标时不包括最右边与最下边
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
	CPointT<TypeT> m_Pt1, m_Pt2;

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
		m_Pt1 = Pt1;
		m_Pt2 = Pt2;
	}
	EXP_INLINE void Offset(const CPointT<TypeT>& Pt)
	{
		m_Pt1 += Pt;
		m_Pt2 += Pt;
	}
	EXP_INLINE void MoveTo(const CPointT<TypeT>& Pt)
	{ m_Pt2 = m_Pt1 = Pt; }

	EXP_INLINE void Inter(const CRectT& tRect)
	{
		Set(CPointT<TypeT>(max(m_Pt1.m_X, tRect.m_Pt1.m_X), min(m_Pt1.m_Y, tRect.m_Pt1.m_Y)), 
			CPointT<TypeT>(min(m_Pt2.m_X, tRect.m_Pt2.m_X), min(m_Pt2.m_Y, tRect.m_Pt2.m_Y)));
	}
	EXP_INLINE void Union(const CRectT& tRect)
	{
		Set(CPointT<TypeT>(min(m_Pt1.m_X, tRect.m_Pt1.m_X), min(m_Pt1.m_Y, tRect.m_Pt1.m_Y)), 
			CPointT<TypeT>(max(m_Pt2.m_X, tRect.m_Pt2.m_X), min(m_Pt2.m_Y, tRect.m_Pt2.m_Y)));
	}

	EXP_INLINE void Inflate(const CPointT<TypeT>& Pt)
	{
		m_Pt1.m_X -= Pt.m_X;
		m_Pt2.m_X += Pt.m_X;
		m_Pt1.m_Y -= Pt.m_Y;
		m_Pt2.m_Y += Pt.m_Y;
	}
	EXP_INLINE void Inflate(const CRectT& tRect)
	{
		m_Pt1 -= tRect.m_Pt1;
		m_Pt2 += tRect.m_Pt2;
	}
	EXP_INLINE void Deflate(const CPointT<TypeT>& Pt)
	{
		m_Pt1.m_X += Pt.m_X;
		m_Pt2.m_X -= Pt.m_X;
		m_Pt1.m_Y += Pt.m_Y;
		m_Pt2.m_Y -= Pt.m_Y;
	}
	EXP_INLINE void Deflate(const CRectT& tRect)
	{
		m_Pt1 += tRect.m_Pt1;
		m_Pt2 -= tRect.m_Pt2;
	}

	EXP_INLINE CRectT& operator=(const CRectT& tRect)
	{
		Set(tRect.m_Pt1, tRect.m_Pt2);
		return (*this);
	}
	EXP_INLINE bool operator==(const CRectT& tRect)
	{ return ((m_Pt1 == tRect.m_Pt1) && (m_Pt2 == tRect.m_Pt2)); }

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

	EXP_INLINE TypeT Left()	 { return m_Pt1.m_X; }
	EXP_INLINE TypeT Top()	 { return m_Pt1.m_Y; }
	EXP_INLINE TypeT Right()	 { return m_Pt2.m_X; }
	EXP_INLINE TypeT Bottom() { return m_Pt2.m_Y; }
	EXP_INLINE TypeT Width()	 { return Right() - Left(); }
	EXP_INLINE TypeT Height() { return Bottom() - Top(); }

	EXP_INLINE bool IsEmpty()
	{ return (m_Pt1.m_X == m_Pt2.m_X || m_Pt1.m_Y == m_Pt2.m_Y); }
	EXP_INLINE bool IsNull()
	{ return (m_Pt1.m_X == 0 && m_Pt2.m_X == 0 && m_Pt1.m_Y == 0 && m_Pt2.m_Y == 0); }

	EXP_INLINE bool PtInRect(const CPointT<TypeT>& Pt)
	{ return (Pt.m_X >= m_Pt1.m_X && Pt.m_X < m_Pt2.m_X && 
			  Pt.m_Y >= m_Pt1.m_Y && Pt.m_Y < m_Pt2.m_Y); }
};

typedef CRectT<> CRect;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Rect_h__*/