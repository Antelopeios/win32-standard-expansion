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
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-19
// Version:	1.0.0001.1630
//
// History:
//	- 1.0.0001.1630(2011-04-19)	+ CPoint改为CPointT<>,支持通过模板参数控制内部数据的类型
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
	TypeT m_X, m_Y;

public:
	CPointT(TypeT nX = 0, TypeT nY = 0)
		: m_X(0), m_Y(0)
	{ Set(nX, nY); }
	CPointT(const CPointT& tPoint)
		: m_X(0), m_Y(0)
	{ (*this) = tPoint; }
	CPointT(POINT& tPoint)
		: m_X(0), m_Y(0)
	{ (*this) = tPoint; }

public:
	EXP_INLINE void Set(TypeT nX = 0, TypeT nY = 0)
	{
		m_X = nX;
		m_Y = nY;
	}
	EXP_INLINE void Offset(TypeT nX = 0, TypeT nY = 0)
	{
		m_X += nX;
		m_Y += nY;
	}

	EXP_INLINE CPointT& operator=(const CPointT& tPoint)
	{
		Set(tPoint.m_X, tPoint.m_Y);
		return (*this);
	}
	EXP_INLINE bool operator==(const CPointT& tPoint)
	{ return ((m_X == tPoint.m_X) && (m_Y == tPoint.m_Y)); }

	EXP_INLINE CPointT& operator+=(const CPointT& tPoint)
	{
		Offset(tPoint.m_X, tPoint.m_Y);
		return (*this);
	}
	EXP_INLINE CPointT& operator-=(const CPointT& tPoint)
	{
		Offset(-tPoint.m_X, -tPoint.m_Y);
		return (*this);
	}

	EXP_INLINE CPointT operator+(const CPointT& tPoint)
	{ return CPointT(m_X + tPoint.m_X, m_Y + tPoint.m_Y); }
	EXP_INLINE CPointT operator-(const CPointT& tPoint)
	{ return CPointT(m_X - tPoint.m_X, m_Y - tPoint.m_Y); }
};

typedef CPointT<> CPoint;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Point_h__*/