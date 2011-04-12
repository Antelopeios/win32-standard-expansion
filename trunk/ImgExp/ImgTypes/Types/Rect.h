// Copyright 2011, Ä¾Í·ÔÆ
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
// Rect - ¾ØÐÎ
//
// Author:	Ä¾Í·ÔÆ
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-12
// Version:	1.0.0000.1400
//////////////////////////////////////////////////////////////////

#ifndef __Rect_h__
#define __Rect_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgTypes/Types/Point.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CRect
{
public:
	CPoint m_Pt1, m_Pt2;

public:
	CRect()
	{}
	CRect(LONG nX1, LONG nY1, LONG nX2, LONG nY2)
	{ Set(CPoint(nX1, nY1), CPoint(nX2, nY2)); }
	CRect(const CPoint& Pt1, const CPoint& Pt2)
	{ Set(Pt1, Pt2); }
	CRect(const CRect& tRect)
	{ (*this) = tRect; }
	CRect(RECT& tRect)
	{ (*this) = tRect; }

public:
	EXP_INLINE void Set(const CPoint& Pt1, const CPoint& Pt2)
	{
		m_Pt1 = Pt1;
		m_Pt2 = Pt2;
	}
	EXP_INLINE void Offset(const CPoint& Pt)
	{
		m_Pt1 += Pt;
		m_Pt2 += Pt;
	}
	EXP_INLINE void MoveTo(const CPoint& Pt)
	{ m_Pt2 = m_Pt1 = Pt; }

	EXP_INLINE void Inter(const CRect& tRect)
	{
		Set(CPoint(max(m_Pt1.m_X, tRect.m_Pt1.m_X), min(m_Pt1.m_Y, tRect.m_Pt1.m_Y)), 
			CPoint(min(m_Pt2.m_X, tRect.m_Pt2.m_X), min(m_Pt2.m_Y, tRect.m_Pt2.m_Y)));
	}
	EXP_INLINE void Union(const CRect& tRect)
	{
		Set(CPoint(min(m_Pt1.m_X, tRect.m_Pt1.m_X), min(m_Pt1.m_Y, tRect.m_Pt1.m_Y)), 
			CPoint(max(m_Pt2.m_X, tRect.m_Pt2.m_X), min(m_Pt2.m_Y, tRect.m_Pt2.m_Y)));
	}

	EXP_INLINE void Inflate(const CPoint& Pt)
	{
		m_Pt1.m_X -= Pt.m_X;
		m_Pt2.m_X += Pt.m_X;
		m_Pt1.m_Y -= Pt.m_Y;
		m_Pt2.m_Y += Pt.m_Y;
	}
	EXP_INLINE void Inflate(const CRect& tRect)
	{
		m_Pt1 -= tRect.m_Pt1;
		m_Pt2 += tRect.m_Pt2;
	}
	EXP_INLINE void Deflate(const CPoint& Pt)
	{
		m_Pt1.m_X += Pt.m_X;
		m_Pt2.m_X -= Pt.m_X;
		m_Pt1.m_Y += Pt.m_Y;
		m_Pt2.m_Y -= Pt.m_Y;
	}
	EXP_INLINE void Deflate(const CRect& tRect)
	{
		m_Pt1 += tRect.m_Pt1;
		m_Pt2 -= tRect.m_Pt2;
	}

	EXP_INLINE CRect& operator=(const CRect& tRect)
	{
		Set(tRect.m_Pt1, tRect.m_Pt2);
		return (*this);
	}
	EXP_INLINE bool operator==(const CRect& tRect)
	{ return ((m_Pt1 == tRect.m_Pt1) && (m_Pt2 == tRect.m_Pt2)); }

	EXP_INLINE CRect& operator+=(const CPoint& Pt)
	{
		Inflate(Pt);
		return (*this);
	}
	EXP_INLINE CRect& operator-=(const CPoint& Pt)
	{
		Deflate(Pt);
		return (*this);
	}
	EXP_INLINE CRect& operator+=(const CRect& tRect)
	{
		Inflate(tRect);
		return (*this);
	}
	EXP_INLINE CRect& operator-=(const CRect& tRect)
	{
		Deflate(tRect);
		return (*this);
	}

	EXP_INLINE CRect operator+(const CPoint& Pt)
	{ return (CRect(*this) += Pt); }
	EXP_INLINE CRect operator-(const CPoint& Pt)
	{ return (CRect(*this) -= Pt); }
	EXP_INLINE CRect operator+(const CRect& tRect)
	{ return (CRect(*this) += tRect); }
	EXP_INLINE CRect operator-(const CRect& tRect)
	{ return (CRect(*this) -= tRect); }

	EXP_INLINE LONG Left()	 { return m_Pt1.m_X; }
	EXP_INLINE LONG Top()	 { return m_Pt1.m_Y; }
	EXP_INLINE LONG Right()	 { return m_Pt2.m_X; }
	EXP_INLINE LONG Bottom() { return m_Pt2.m_Y; }
	EXP_INLINE LONG Width()	 { return Right() - Left(); }
	EXP_INLINE LONG Height() { return Bottom() - Top(); }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Rect_h__*/