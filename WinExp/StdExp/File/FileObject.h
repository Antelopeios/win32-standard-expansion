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
// FileObject - 文件对象基类
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-12-14
// Version:	1.0.0006.1108
//
// History:
//	- 1.0.0003.0106(2011-04-04)	+ 添加CFileSeeker类,用于自动还原文件索引
//	- 1.0.0004.1722(2011-09-08)	+ 添加IFileObject::SetSize()接口,方便调整文件大小
//	- 1.0.0005.1522(2011-12-13)	+ 添加IFileObject::operator=()操作符重载,支持文件内容直接拷贝
//								+ 添加IFileObject::Clear()接口,支持直接清空文件
//	- 1.0.0006.1108(2011-12-14)	+ File下的派生类支持拷贝构造
//								+ 添加IFileObject::CopyTo()接口
//////////////////////////////////////////////////////////////////

#ifndef __FileObject_h__
#define __FileObject_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

interface IFileObject
{
public:
	enum SeekPosition
	{
		begin	= FILE_BEGIN, 
		current = FILE_CURRENT, 
		end		= FILE_END
	};

	class CFileSeeker
	{
	protected:
		IFileObject* m_pFile;
		uint64_t	 m_nTell;

	public:
		CFileSeeker(IFileObject* pFile)
			: m_pFile(pFile)
			, m_nTell(-1)
		{
			if (m_pFile)
			{
				m_nTell = m_pFile->Tell();
				if (m_nTell == (uint64_t)-1)
					m_pFile = NULL;
			}
		}
		~CFileSeeker()
		{
			if (m_pFile)
				m_pFile->Seek(m_nTell, IFileObject::begin);
		}
	};

public:
	IFileObject() {}
	virtual ~IFileObject() {}

public:
	virtual BOOL Close() = 0;

	virtual DWORD Read(LPVOID pBuff, DWORD nCount, DWORD nSize = sizeof(TCHAR)) = 0;
	virtual DWORD Write(LPCVOID pBuff, DWORD nCount, DWORD nSize = sizeof(TCHAR)) = 0;
	virtual BOOL Clear() = 0;

	virtual BOOL Seek(int64_t nOffset, int iOrigin = current) = 0;
	virtual uint64_t Tell() = 0;
	virtual uint64_t Size() = 0;
	virtual BOOL SetSize(uint64_t nSize) = 0;
	virtual BOOL Flush() = 0;
	virtual BOOL Eof() = 0;
	virtual BOOL Error() = 0;

	virtual BOOL PutC(TCHAR cChar)
	{
		return (Write(&cChar, 1) == 1);
	}
	virtual TCHAR GetC()
	{
		if (Error()) return _TEOF;
		TCHAR c;
		if (Read(&c, 1) == 1)
			return c;
		else
			return _TEOF;
	}
	virtual BOOL PutS(LPCTSTR sString)
	{
		DWORD len = _tcslen(sString);
		return (Write(sString, len) == len);
	}
	virtual LPTSTR GetS(LPTSTR sString, int nCount)
	{
		if (Error()) return NULL;
		if (Read(sString, nCount) == 0)
			return NULL;
		else
			return sString;
	}

	BOOL CopyTo(IFileObject& rFile)
	{
		if (Error()) return FALSE;
		if (!rFile.Clear()) return FALSE;
		CFileSeeker seeker(this);
		if (!Seek(0, begin)) return FALSE;
		BYTE buff[1 << 20] = {0}; // 1M的缓存区
		while(Read(buff, _countof(buff), sizeof(BYTE)) != 0)
			if (rFile.Write(buff, _countof(buff), sizeof(BYTE)) != _countof(buff)) break;
		return TRUE;
	}
	IFileObject& operator=(IFileObject& rFile)
	{
		rFile.CopyTo(*this);
		return (*this);
	}

	virtual LPCTSTR GetFileName() { return NULL; }
};

typedef IFileObject::CFileSeeker CFileSeeker;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__FileObject_h__*/