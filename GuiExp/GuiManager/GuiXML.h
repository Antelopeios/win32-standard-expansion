// Copyright 2011, ľͷ��
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
// GuiXML - ���������ļ��ӿ�(XML����)
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-06-10
// Version:	1.0.0000.1420
//
// History:
//	- 1.0.0000.1420(2011-06-10)	@ ��ʼ����GuiXML
//////////////////////////////////////////////////////////////////

#ifndef __GuiXML_h__
#define __GuiXML_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiCommon/GuiCommon.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiXML
{
public:
	typedef CMapT<CString, CString> map_t;
	typedef struct _Node
	{
		CString nam; // ����
		CString val; // ֵ
		map_t	att; // ����
		CString tmp; // ��ʱ�洢
	} node_t;
	typedef CTreeT<node_t*> tree_t;
	typedef tree_t::iterator_t iter_t;

	// ��ȡ״̬
	enum state_t
	{
		sta_nor, // ����
		sta_tag, // ��ǩ
		sta_tco, // ��ǩ����
		sta_tce, // ��ǩ�������
		sta_wat, // �ȴ�����
		sta_avl, // ���Ը�ֵ
		sta_aco, // ��������
		sta_ace  // �����������
	};

protected:
	IFileObject* m_pFile;
	CMemFile	 m_mFile;
	tree_t		 m_xData;
	CGC			 m_GC;

protected:
	// ����
	bool Nor(state_t eSta, iter_t ite)
	{
		if (eSta != sta_nor && 
			eSta != sta_tce && 
			eSta != sta_wat) return false;
		char buf = '\0';
		if (m_mFile.Read(&buf, 1, sizeof(buf)) != 1)
			return true;
		switch (buf)
		{
		case '<':	// ��ǩ
			return Tag(sta_nor, ite);
		default:	// ����
			return Nor(sta_nor, ite);
		}
		return false;
	}
	// ��ǩ
	bool Tag(state_t eSta, iter_t ite)
	{
		if (eSta != sta_tag && 
			eSta != sta_nor && 
			eSta != sta_tco && 
			eSta != sta_ace) return false;
		char buf = '\0';
		if (m_mFile.Read(&buf, 1, sizeof(buf)) != 1)
			return false;
		switch (buf)
		{
		case '>':	// ��ǩ����
			return Tco(sta_tag, ite);
		case ' ':	// �ȴ�����
			return Wat(sta_tag, ite);
		case '/':	// ��ǩ���ݸ�ֵ���
			return Tce(sta_tag, ite);
		default:
			if (eSta == sta_nor || 
				eSta == sta_tco)
			{
				m_xData.Add(ExMem::Alloc<node_t>(&m_GC), ite);
				node_t* node = *ite;
				char ac[2] = {buf, 0};
				TCHAR tc[2] = {0};
				::OemToChar(ac, tc);
				node->nam += tc;
			}
			else
			if (eSta == sta_tag)
			{
				node_t* node = *ite;
				char ac[2] = {buf, 0};
				TCHAR tc[2] = {0};
				::OemToChar(ac, tc);
				node->nam += tc;
			}
			return Tag(sta_tag, ite);
		}
		return false;
	}
	// ��ǩ����
	bool Tco(state_t eSta, iter_t ite)
	{
		if (eSta != sta_tco && 
			eSta != sta_tag && 
			eSta != sta_wat) return false;
		if (ite == m_xData.Head()) return false;
		char buf = '\0';
		if (m_mFile.Read(&buf, 1, sizeof(buf)) != 1)
			return false;
		switch (buf)
		{
		case '<':
			return Tag(sta_tco, ite);
		default:
			{
				node_t* node = *ite;
				char ac[2] = {buf, 0};
				TCHAR tc[2] = {0};
				::OemToChar(ac, tc);
				node->val += tc;
			}
			return Tco(sta_tco, ite);
		}
		return false;
	}
	// ��ǩ�������
	bool Tce(state_t eSta, iter_t ite)
	{
		if (eSta != sta_tag) return false;
		return Nor(sta_tce, ite->Parent());
	}
	// �ȴ�����
	bool Wat(state_t eSta, iter_t ite)
	{
		if (eSta != sta_wat && 
			eSta != sta_tag) return false;
		char buf = '\0';
		if (m_mFile.Read(&buf, 1, sizeof(buf)) != 1)
			return false;
		switch (buf)
		{
		case '<':
		case ' ':
		case '\"':
		case '/':
			return Wat(sta_wat, ite);
		case '>':
			return Tco(sta_wat, ite);
		case '=':
			{
				node_t* node = *ite;
				if (node->tmp.Empty())
					return false;
				else
					node->att[node->tmp] = _T("");
			}
			return Avl(sta_wat, ite);
		default:
			{
				node_t* node = *ite;
				char ac[2] = {buf, 0};
				TCHAR tc[2] = {0};
				::OemToChar(ac, tc);
				node->tmp += tc;
			}
			return Wat(sta_wat, ite);
		}
		return false;
	}
	// ���Ը�ֵ
	bool Avl(state_t eSta, iter_t ite)
	{
		if (eSta != sta_avl && 
			eSta != sta_wat) return false;
		char buf = '\0';
		if (m_mFile.Read(&buf, 1, sizeof(buf)) != 1)
			return false;
		switch (buf)
		{
		case '\"':
			return Aco(sta_avl, ite);
		default:
			return Avl(sta_avl, ite);
		}
		return false;
	}
	// ��������
	bool Aco(state_t eSta, iter_t ite)
	{
		if (eSta != sta_aco && 
			eSta != sta_avl) return false;
		char buf = '\0';
		if (m_mFile.Read(&buf, 1, sizeof(buf)) != 1)
			return false;
		switch (buf)
		{
		case '\"':
			return Ace(sta_aco, ite);
		default:
			{
				node_t* node = *ite;
				char ac[2] = {buf, 0};
				TCHAR tc[2] = {0};
				::OemToChar(ac, tc);
				node->att[node->tmp] += tc;
			}
			return Aco(sta_aco, ite);
		}
		return false;
	}
	// �����������
	bool Ace(state_t eSta, iter_t ite)
	{
		if (eSta != sta_aco) return false;
		node_t* node = *ite;
		node->tmp.Clear();
		return Tag(sta_ace, ite);
	}

public:
	CGuiXML()
		: m_pFile(NULL)
	{ Clear(); }
	virtual ~CGuiXML()
	{}

public:
	void SetFile(IFileObject* pFile)
	{ m_pFile = pFile; }
	IFileObject* GetFile()
	{ return m_pFile; }

	void Clear()
	{
		m_xData.Clear();
		m_GC.Clear();
		m_xData.Add(ExMem::Alloc<node_t>(&m_GC)); // ���Ĭ�ϵĸ��ڵ�
		m_mFile.Close();
	}

	// ����
	bool Encode()
	{
		if (!m_pFile) return false;
		if (!m_pFile->Seek(0, IFileObject::begin) || 
			!m_mFile.Seek(0, IFileObject::begin))
			return false;
		return true;
	}
	// ����
	bool Decode()
	{
		if (!m_pFile) return false;
		if (!m_pFile->Seek(0, IFileObject::begin))
			return false;
		Clear();
		// ����ת��
		DWORD len = (DWORD)m_pFile->Size() + 1;
		char* buf = ExMem::Alloc<char>(&m_GC, len);
		ZeroMemory(buf, len);
		if (m_pFile->Read(buf, len - 1, sizeof(char)) != len - 1)
			return false;
		CString oem;
		if (!::OemToChar(buf, oem.GetCStr(len)))
			return false;
		if (m_mFile.Write(oem.GetCStr(), oem.GetLength()) != oem.GetLength())
			return false;
		// ��ʼ����
		return Nor(sta_nor, m_xData.Head());
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiXML_h__*/