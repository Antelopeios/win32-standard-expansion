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
// Size - ÇøÓò
//
// Author:	Ä¾Í·ÔÆ
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-24
// Version:	1.0.0000.2238
//////////////////////////////////////////////////////////////////

#ifndef __Size_h__
#define __Size_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgTypes/Types/Point.h"
#include "ImgTypes/Types/Rect.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename TypeT = LONG>
class CSizeT
{
public:
	TypeT cx, cy;

public:
	CSizeT(TypeT nX = 0, TypeT nY = 0)
		: cx(0), cy(0)
	{ Set(nX, nY); }
	CSizeT(const CSizeT& tSize)
		: cx(0), cy(0)
	{ (*this) = tSize; }
	CSizeT(SIZE& tSize)
		: cx(0), cy(0)
	{ (*this) = tSize; }
	CSizeT(POINT& tPoint)
		: cx(0), cy(0)
	{ (*this) = tPoint; }
	CSizeT(RECT& tRect)
		: cx(0), cy(0)
	{ (*this) = tRect; }

public:
	EXP_INLINE void Set(TypeT nX = 0, TypeT nY = 0)
	{
		cx = nX;
		cy = nY;
	}

	EXP_INLINE CSizeT& operator=(const CSizeT& tSize)
	{
		Set(tSize.cx, tSize.cy);
		return (*this);
	}
	EXP_INLINE CSizeT& operator=(SIZE& tSize)
	{
		Set(tSize.cx, tSize.cy);
		return (*this);
	}
	EXP_INLINE CSizeT& operator=(POINT& tPoint)
	{
		Set(tPoint.x, tPoint.y);
		return (*this);
	}
	EXP_INLINE CSizeT& operator=(RECT& tRect)
	{
		Set(tRect.right - tRect.left, tRect.bottom - tRect.top);
		return (*this);
	}

	EXP_INLINE BOOL operator==(const CSizeT& tSize)
	{ return ((cx == tSize.cx) && (cy == tSize.cy)); }
	EXP_INLINE BOOL operator==(SIZE& tSize)
	{ return ((cx == tSize.cx) && (cy == tSize.cy)); }
	EXP_INLINE BOOL operator!=(const CSizeT& tSize)
	{ return !((*this) == tSize); }
	EXP_INLINE BOOL operator!=(SIZE& tSize)
	{ return !((*this) == tSize); }

	EXP_INLINE CSizeT& operator+=(const CSizeT& tSize)
	{
		cx += tSize.cx;
		cy += tSize.cy;
		return (*this);
	}
	EXP_INLINE CSizeT& operator-=(const CSizeT& tSize)
	{
		cx -= tSize.cx;
		cy -= tSize.cy;
		return (*this);
	}

	EXP_INLINE CSizeT& operator+(const CSizeT& tSize)
	{
		return CSizeT(cx + tSize.cx, cy + tSize.cy);
	}
	EXP_INLINE CSizeT& operator-(const CSizeT& tSize)
	{
		return CSizeT(cx - tSize.cx, cy - tSize.cy);
	}

	EXP_INLINE CSizeT operator-()
	{ return CSizeT(-cx, -cy); }

	EXP_INLINE operator SIZE() const
	{
		SIZE sz = {cx, cy};
		return sz;
	}
};

typedef CSizeT<> CSize;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Size_h__*/