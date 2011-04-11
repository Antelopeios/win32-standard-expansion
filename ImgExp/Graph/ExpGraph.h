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
// ExpGraph - 画布拓展类
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-11
// Version:	1.0.0000.1100
//////////////////////////////////////////////////////////////////

#ifndef __ExpGraph_h__
#define __ExpGraph_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph/GraphObject.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CExpGraph : public IGraphObject
{
protected:
	typedef CListT<DWORD>	typlst_t;
	typedef CListT<HGDIOBJ>	objlst_t;

	typlst_t m_TypLst;
	objlst_t m_ObjLst;

public:
	CExpGraph()
		: IGraphObject()
	{}
	CExpGraph(graph_t tGraph)
		: IGraphObject()
	{ SetGraph(tGraph); }
	virtual ~CExpGraph()
	{}

public:
	bool Delete()
	{
		if (IsNull()) return true;
		for(objlst_t::iterator_t ite = m_ObjLst.Head(); ite != m_ObjLst.Tail(); ++ite)
			SelectObject(m_Graph, ite->Val());
		m_ObjLst.Clear();
		m_TypLst.Clear();
		DeleteDC(m_Graph);
		m_Handle = NULL;
	}
	graph_t Create(graph_t tGraph = NULL)
	{
		SetGraph(CreateCompatibleDC(hDC));
		return GetGraph();
	}

	HGDIOBJ SetObject(HGDIOBJ hObj)
	{
		HGDIOBJ tmp_obj(SelectObject(m_Graph, hObj));
		DWORD type = GetObjectType(hObj);
		if (typlst_t::finder_t::Find(m_TypLst, type) == m_TypLst.Tail())
		{
			m_TypLst.Add(type);
			if (tmp_obj && (objlst_t::finder_t::Find(m_ObjLst, tmp_obj) == m_ObjLst.Tail()))
				m_ObjLst.Add(tmp_obj);
		}
		return tmp_obj;
	}
	HGDIOBJ GetObject(UINT uType)
	{ return GetCurrentObject(m_Graph, uType); }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ExpGraph_h__*/