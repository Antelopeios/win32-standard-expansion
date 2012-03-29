// Copyright 2011-2012, 木头云
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
// Graph - 画布对象类
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-16
// Version:	1.0.0001.2324
//
// History:
//	- 1.0.0001.2324(2012-03-16)	@ 开始构建Graph
//////////////////////////////////////////////////////////////////

#ifndef __Graph_h__
#define __Graph_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgTypes/Image.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGraph
{
protected:
	CImage	m_Image;
	CRect	m_Rect;
	CPoint	m_Coor;

private:
	CGraph()
	{
		m_Image.SetTrust(FALSE);
	}
public:
	CGraph(const CGraph& dat)
	{
		this->CGraph::CGraph();
		(*this) = dat;
	}
	CGraph(const CImage& img, const CRect& rc, const CPoint& cr)
	{
		this->CGraph::CGraph();
		m_Image = img;
		m_Rect = rc;
		m_Coor = cr;
	}
	CGraph(const CImage& img, const CRect& rc)
	{
		this->CGraph::CGraph(img, rc, rc.LeftTop());
	}
	CGraph(const CImage& img)
	{
		this->CGraph::CGraph(img, CRect(0, 0, img.GetWidth(), img.GetHeight()));
	}

public:
	BOOL IsNull() const
	{
		return m_Image.IsNull();
	}

	CGraph& operator=(const CGraph& tType)
	{
		m_Image = tType.m_Image;
		m_Rect = tType.m_Rect;
		m_Coor = tType.m_Coor;
		return (*this);
	}

	operator image_t() const
	{
		return m_Image;
	}
	operator CImage() const
	{
		return m_Image;
	}

	CImage& GetImage()
	{
		return m_Image;
	}
	void SetImage(const CImage& tType)
	{
		m_Image = tType;
	}

	const CRect& GetRect() const
	{
		return m_Rect;
	}
	void SetRect(const CRect& tType)
	{
		m_Rect = tType;
	}

	const CPoint& GetCoor() const
	{
		return m_Coor;
	}
	void SetCoor(const CPoint& tType)
	{
		m_Coor = tType;
	}

	CPoint& Transform(CPoint& pt) const
	{
		return pt.Offset(m_Coor);
	}
	CLine& Transform(CLine& ln) const
	{
		return ln.Offset(m_Coor);
	}
	CRect& Transform(CRect& rc) const
	{
		return rc.Offset(m_Coor);
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Graph_h__*/