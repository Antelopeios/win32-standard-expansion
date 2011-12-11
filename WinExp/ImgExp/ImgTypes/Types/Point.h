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
// Point - 点
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-24
// Version:	1.0.0005.2242
//
// History:
//	- 1.0.0001.1630(2011-04-19)	+ CPoint改为CPointT<>,支持通过模板参数控制内部数据的类型
//	- 1.0.0002.1513(2011-04-20)	^ 简化CPointT的内部数据命名
//	- 1.0.0003.1730(2011-05-05)	# 修正构造时传入基本数据类型将导致堆栈溢出的问题
//	- 1.0.0004.1020(2011-05-17)	+ 添加CPointT::operator-()负号重载
//	- 1.0.0005.2242(2011-05-24)	+ 添加CPointT::operator!=()与CPointT::operator POINT()
//////////////////////////////////////////////////////////////////

#ifndef __Point_h__
#define __Point_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename TypeT = LONG>
class CPointT
{
public:
	TypeT x, y;

public:
	CPointT(TypeT nX = 0, TypeT nY = 0)
		: x(0), y(0)
	{ Set(nX, nY); }
	CPointT(const CPointT& tPoint)
		: x(0), y(0)
	{ (*this) = tPoint; }
	CPointT(POINT& tPoint)
		: x(0), y(0)
	{ (*this) = tPoint; }

public:
	EXP_INLINE void Set(TypeT nX = 0, TypeT nY = 0)
	{
		x = nX;
		y = nY;
	}
	EXP_INLINE void Offset(TypeT nX = 0, TypeT nY = 0)
	{
		x += nX;
		y += nY;
	}

	EXP_INLINE CPointT& operator=(const CPointT& tPoint)
	{
		Set(tPoint.x, tPoint.y);
		return (*this);
	}
	EXP_INLINE CPointT& operator=(POINT& tPoint)
	{
		Set(tPoint.x, tPoint.y);
		return (*this);
	}
	EXP_INLINE BOOL operator==(const CPointT& tPoint)
	{ return ((x == tPoint.x) && (y == tPoint.y)); }
	EXP_INLINE BOOL operator==(POINT& tPoint)
	{ return ((x == tPoint.x) && (y == tPoint.y)); }
	EXP_INLINE BOOL operator!=(const CPointT& tPoint)
	{ return !((*this) == tPoint); }
	EXP_INLINE BOOL operator!=(POINT& tPoint)
	{ return !((*this) == tPoint); }

	EXP_INLINE CPointT& operator+=(const CPointT& tPoint)
	{
		Offset(tPoint.x, tPoint.y);
		return (*this);
	}
	EXP_INLINE CPointT& operator-=(const CPointT& tPoint)
	{
		Offset(-tPoint.x, -tPoint.y);
		return (*this);
	}

	EXP_INLINE CPointT operator+(const CPointT& tPoint)
	{ return CPointT(x + tPoint.x, y + tPoint.y); }
	EXP_INLINE CPointT operator-(const CPointT& tPoint)
	{ return CPointT(x - tPoint.x, y - tPoint.y); }

	EXP_INLINE CPointT operator-()
	{ return CPointT(-x, -y); }

	EXP_INLINE operator POINT()
	{
		POINT pt = {x, y};
		return pt;
	}
};

typedef CPointT<> CPoint;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Point_h__*/