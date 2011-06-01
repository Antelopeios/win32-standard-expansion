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
// MemFile - ÄÚ´æÎÄ¼þ´æ´¢
//
// Author:	Ä¾Í·ÔÆ
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-02-14
// Version:	1.0.0004.1500
//////////////////////////////////////////////////////////////////

#ifndef __MemFile_h__
#define __MemFile_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "File/FileObject.h"
#include "Container/Array.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename AllocT = EXP_MEMORY_ALLOC>
class CMemFileT : public IFileObject
{
public:
	typedef AllocT alloc_t;
	typedef struct _MemArrayPolicy : public _ArrayPolicyT<alloc_t>
	{
		static DWORD Expan(DWORD nSize)
		{ return (((nSize >> 16) + 1) << 16); /*+64KB*/ }
	} policy_t;
	typedef CArrayT<BYTE, policy_t> buff_t;
	typedef typename buff_t::iterator_t iterator_t;

protected:
	buff_t m_MemBuff;
	iterator_t m_Position;

public:
	CMemFileT(BYTE* pBuff = NULL, DWORD nSize = 1)
	{ Open(pBuff, nSize); }
	virtual ~CMemFileT()
	{ Close(); }

public:
	bool Open(BYTE* pBuff = NULL, DWORD nSize = 1)
	{
		if (nSize == 0) return false;
		if (!Close()) return false;
		if (pBuff)
			m_MemBuff.SetArray(pBuff, nSize);
		else
			m_MemBuff.SetSize(nSize);
		return true;
	}
	virtual bool Close()
	{
		m_MemBuff.Clear();
		m_Position = m_MemBuff.Head();
		return true;
	}

	virtual DWORD Read(LPVOID pBuff, DWORD nCount, DWORD nSize = sizeof(TCHAR))
	{
		if (!pBuff || nCount == 0 || nSize == 0) return 0;
		if (Eof()) return 0;

		DWORD len = nCount * nSize;
		len = (m_Position->Index() + len > m_MemBuff.GetCount()) ? (m_MemBuff.GetCount() - m_Position->Index()) : len;
		memcpy(pBuff, &(m_Position->Val()), len);
		m_Position += len;

		return (len / nSize);
	}
	virtual DWORD Write(LPCVOID pBuff, DWORD nCount, DWORD nSize = sizeof(TCHAR))
	{
		if (!pBuff || nCount == 0 || nSize == 0) return 0;
		if (Error()) return 0;

		DWORD len = nCount * nSize;
		m_MemBuff.Del(m_Position, len);
		if (m_Position->Index() + len > m_MemBuff.GetCount())
		{
			m_MemBuff.SetSizeExpan(m_Position->Index() + len);
			if (m_Position->Index() > m_MemBuff.GetCount())
			{	// Ìî³ä¿Õ°×¿Õ¼ä
				DWORD fill = m_Position->Index() - m_MemBuff.GetCount();
				BYTE* buff = alloc_t::Alloc<BYTE>(fill);
				m_MemBuff.AddArray(buff, fill);
				alloc_t::Free(buff);
			}
		}
		m_MemBuff.AddArray((BYTE*)pBuff, len, m_Position);
		m_Position += len;

		return (len / nSize);
	}

	virtual bool Seek(uint64_t nOffset, int iOrigin = current)
	{
		if (Error()) return false;
		long new_pos = m_Position->Index();

		if (iOrigin == begin)	new_pos = (long)nOffset;
		else
		if (iOrigin == current) new_pos += (long)nOffset;
		else
		if (iOrigin == end)		new_pos = m_MemBuff.GetCount() + (long)nOffset;
		else return false;
		if (new_pos < 0)		new_pos = 0;

		m_Position->nIndx = new_pos;
		return true;
	}
	virtual uint64_t Tell()
	{
		if (Error()) return -1;
		return m_Position->Index();
	}
	virtual uint64_t Size()
	{
		if (Error()) return -1;
		return m_MemBuff.GetCount();
	}
	virtual bool Flush()
	{
		if (Error()) return false;
		return true;
	}
	virtual bool Eof()
	{
		if (Error()) return true;
		return (m_Position->Index() >= m_MemBuff.GetCount());
	}
	virtual bool Error()
	{
		return m_MemBuff.IsNull();
	}

	operator LPCVOID()
	{
		return (LPCVOID)(LPBYTE)m_MemBuff;
	}
};

typedef CMemFileT<> CMemFile;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__MemFile_h__*/