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
// LogSystem - 循环日志
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-12-13
// Version:	1.1.0002.1606
//////////////////////////////////////////////////////////////////

#ifndef __LogSystem_h__
#define __LogSystem_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "File/FileObject.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

#ifndef DEF_MAX_CACLIMIT
#define DEF_MAX_CACLIMIT	(1024 * 1024)
#endif
#ifndef DEF_MAX_BUFLIMIT
#define DEF_MAX_BUFLIMIT	MAX_PATH
#endif
#ifndef DEF_MAX_RECLIMIT
#define DEF_MAX_RECLIMIT	2000
#endif

//////////////////////////////////////////////////////////////////

template<const DWORD CacLimit = DEF_MAX_CACLIMIT, 
		 const DWORD BufLimit = DEF_MAX_BUFLIMIT>
class TBuffCache
{
	template<const DWORD CacLimit = DEF_MAX_CACLIMIT, 
			 const DWORD BufLimit = DEF_MAX_BUFLIMIT>
	class CIteratorT;
	template<const DWORD CacLimit, 
			 const DWORD BufLimit>
	friend class CIteratorT;

private:
	struct Buf
	{
		char buffer[BufLimit];
		Buf() { ZeroMemory(buffer, sizeof(buffer)); }
	};

	Buf m_BuffCache[CacLimit / BufLimit];

public:
	template<const DWORD CacLimit, 
			 const DWORD BufLimit>
	class CIteratorT
	{
	private:
		DWORD m_Index;
		DWORD m_Start;
		bool m_BeEnd;

	public:
		explicit CIteratorT(DWORD dwStart = 0)
			: m_Index(dwStart)
			, m_BeEnd(false)
		{ SetStart(dwStart); SetBegin(); }

	public:
		DWORD GetItemLimit() const
		{ return (CacLimit / BufLimit); }

		void SetBegin() { m_Index = m_Start; m_BeEnd = false; }
		void SetEnd()	{ m_Index = m_Start; m_BeEnd = true; }
		bool IsBegin()	{ return (m_BeEnd == false && m_Index == m_Start); }
		bool IsEnd()	{ return (m_BeEnd == true); }

		DWORD GetStart()
		{ return m_Start; }
		void SetStart(DWORD dwStart)
		{
			m_Start = dwStart % GetItemLimit();
			if (m_BeEnd == true && m_Index != m_Start)
				m_BeEnd = false;
		}

		operator DWORD()
		{
			return m_Index;
		}

		// ++i
		CIteratorT& operator++() 
		{ 
			if (IsEnd()) return *this;
			++m_Index;
			if (m_Index >= GetItemLimit())
				m_Index = 0;
			if (m_Index == GetStart())
				SetEnd();
			return *this;
		}
		// i++ 
		const CIteratorT operator++(int)
		{ 
			CIteratorT tmp = *this;
			++(*this);
			return tmp;
		}

		// --i
		CIteratorT& operator--() 
		{ 
			if (IsBegin()) return *this;
			if (m_Index == 0)
				m_Index = GetItemLimit() - 1;
			else
				--m_Index;
			if (m_Index == GetStart())
				SetBegin();
			return *this; 
		}
		// i--
		const CIteratorT operator--(int)
		{ 
			CIteratorT tmp = *this;
			--(*this);
			return tmp;
		}
	};

	typedef CIteratorT<CacLimit, BufLimit> CIterator;

private:
	CIterator m_BuffIte;

public:
	char* GetBuff(DWORD dwInx)
	{
		CIterator ite(dwInx);
		return m_BuffCache[ite].buffer;
	}

	char* GetCurrBuff()
	{ return GetBuff(m_BuffIte); }
	void SetCurrBuff()
	{
		if (m_BuffIte.IsEnd())
			m_BuffIte.SetStart(m_BuffIte.GetStart() + 1);
		++m_BuffIte;
	}

	DWORD GetStart()
	{ return m_BuffIte.GetStart(); }

	void Clear()
	{ ZeroMemory(m_BuffCache, CacLimit); }
};

//////////////////////////////////////////////////////////////////

template<const DWORD RecLimit = DEF_MAX_RECLIMIT>
class TLog
{
public:
	typedef TBuffCache<RecLimit * DEF_MAX_BUFLIMIT, DEF_MAX_BUFLIMIT> BuffCache;

private:
	BuffCache m_Cache;
	BOOL	  m_bCoutTime;

public:
	TLog()
		: m_bCoutTime(TRUE)
	{}

public:
	BOOL SetCoutTime(BOOL bCoutTime)
	{
		BOOL is_cout_time = m_bCoutTime;
		m_bCoutTime = bCoutTime;
		return is_cout_time;
	}

	void Cout(LPCTSTR sCout)
	{
		if (!sCout) return;

		// 格式化数据
		::CharToOem(sCout, m_Cache.GetCurrBuff());
		BOOL bn = (m_Cache.GetCurrBuff()[0] == '\n');

		// 将"\n"改为"\r\n"
		CStringT<char> str_cout(bn ? m_Cache.GetCurrBuff() + 1 : m_Cache.GetCurrBuff()), 
			str_old("\n"), str_new("\r\n");
		CStringT<char>::iterator_t ite = str_cout.Head();
		while((ite = str_cout.Find(ite, str_cout.Tail(), '\n')) != str_cout.Tail())
		{
			str_cout.Del(ite);
			str_cout.AddString("\r\n", ite);
			ite += 2;
		}

		// 写入时间戳
		if (m_bCoutTime)
		{
			SYSTEMTIME sys;
			GetLocalTime(&sys);
			sprintf_s(m_Cache.GetCurrBuff(), DEF_MAX_BUFLIMIT, 
				bn ? "\r\n[%02d:%02d:%02d.%03d]\t%s" : "[%02d:%02d:%02d.%03d]\t%s", 
				sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, (LPCSTR)str_cout);
			m_Cache.SetCurrBuff();
		}
		else
		{
			sprintf_s(m_Cache.GetCurrBuff(), DEF_MAX_BUFLIMIT, bn ? "\r\n%s" : "%s", (LPCSTR)str_cout);
			m_Cache.SetCurrBuff();
		}
	}

	void ToFile(IFileObject* pFile)
	{
		if (!pFile) return;
		for(BuffCache::CIterator ite(m_Cache.GetStart()); !ite.IsEnd(); ++ite)
		{
			char* cout = m_Cache.GetBuff(ite);
			if (!cout) return;
			pFile->Write(cout, strlen(cout), sizeof(char));
		}
	}

	void Clear()
	{
		m_Cache.Clear();
		m_LogTM = 0;
	}
};

typedef TLog<> CLog;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__LogSystem_h__*/
