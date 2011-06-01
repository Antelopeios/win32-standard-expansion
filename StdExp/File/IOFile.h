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
// IOFile - 持久化文件存储
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-02-14
// Version:	1.0.0002.1500
//////////////////////////////////////////////////////////////////

#ifndef __IOFile_h__
#define __IOFile_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "File/FileObject.h"
#include "Container/String.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CIOFile : public IFileObject
{
public:
	enum OpenFlags
	{
		modeRead		 = (int)0x00000, 
		modeWrite		 = (int)0x00001, 
		modeReadWrite	 = (int)0x00002, 
		shareCompat		 = (int)0x00000, 
		shareExclusive	 = (int)0x00010, 
		shareDenyWrite	 = (int)0x00020, 
		shareDenyRead	 = (int)0x00030, 
		shareDenyNone	 = (int)0x00040, 
		modeNoInherit	 = (int)0x00080, 
		modeCreate		 = (int)0x01000, 
		modeNoTruncate	 = (int)0x02000, 
		typeText		 = (int)0x04000, // typeText and typeBinary are
		typeBinary		 = (int)0x08000, // used in derived classes only
		osNoBuffer		 = (int)0x10000, 
		osWriteThrough	 = (int)0x20000, 
		osRandomAccess	 = (int)0x40000, 
		osSequentialScan = (int)0x80000
	};

protected:
	HANDLE m_hFile;
	CString m_strFileName;

public:
	CIOFile()
		: m_hFile(INVALID_HANDLE_VALUE)
	{}
	CIOFile(LPCTSTR sPath, UINT nOpenFlags = modeRead | shareCompat)
		: m_hFile(INVALID_HANDLE_VALUE)
	{ Open(sPath, nOpenFlags); }
	virtual ~CIOFile()
	{ Close(); }

public:
	bool Open(LPCTSTR sPath, UINT nOpenFlags = modeRead | shareCompat)
	{
		if (!sPath) return false;
		if (!Close()) return false;

		nOpenFlags &= ~(UINT)typeBinary; // CIOFile 一直是二进制文本, CreateFile 并不需要这个标记
		m_hFile = INVALID_HANDLE_VALUE;
		if (FAILED(StringCchLength(sPath, MAX_PATH, NULL)) )
			return false;
		m_strFileName = sPath;

		// map read/write mode
		DWORD dwAccess = 0;
		switch (nOpenFlags & (modeRead | modeWrite | modeReadWrite))
		{
		case modeRead:
			dwAccess = GENERIC_READ;
			break;
		case modeWrite:
			dwAccess = GENERIC_WRITE;
			break;
		case modeReadWrite:
			dwAccess = GENERIC_READ | GENERIC_WRITE;
			break;
		default:
			ExAssert(false);  // 无效的共享模式
		}

		// map share mode
		DWORD dwShareMode = 0;
		switch (nOpenFlags & 0x70)    // map compatibility mode to exclusive
		{
		case shareCompat:
		case shareExclusive:
			dwShareMode = 0;
			break;
		case shareDenyWrite:
			dwShareMode = FILE_SHARE_READ;
			break;
		case shareDenyRead:
			dwShareMode = FILE_SHARE_WRITE;
			break;
		case shareDenyNone:
			dwShareMode = FILE_SHARE_WRITE | FILE_SHARE_READ;
			break;
		default:
			ExAssert(false);  // 无效的共享模式
		}

		// map modeNoInherit flag
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = (nOpenFlags & modeNoInherit) == 0;

		// map creation flags
		DWORD dwCreateFlag;
		if (nOpenFlags & modeCreate)
		{
			if (nOpenFlags & modeNoTruncate)
				dwCreateFlag = OPEN_ALWAYS;
			else
				dwCreateFlag = CREATE_ALWAYS;
		}
		else
			dwCreateFlag = OPEN_EXISTING;

		// special system-level access flags
		// Random access and sequential scan should be mutually exclusive
		ExAssert((nOpenFlags & (osRandomAccess | osSequentialScan)) != (osRandomAccess | osSequentialScan));
		DWORD dwFlags = FILE_ATTRIBUTE_NORMAL;
		if (nOpenFlags & osNoBuffer)
			dwFlags |= FILE_FLAG_NO_BUFFERING;
		if (nOpenFlags & osWriteThrough)
			dwFlags |= FILE_FLAG_WRITE_THROUGH;
		if (nOpenFlags & osRandomAccess)
			dwFlags |= FILE_FLAG_RANDOM_ACCESS;
		if (nOpenFlags & osSequentialScan)
			dwFlags |= FILE_FLAG_SEQUENTIAL_SCAN;

		// attempt file creation
		m_hFile = ::CreateFile(m_strFileName, dwAccess, dwShareMode, &sa,
			dwCreateFlag, dwFlags, NULL);
		if (m_hFile == INVALID_HANDLE_VALUE)
			return false;
		return true;
	}
	virtual bool Close()
	{
		bool ret = Error() ? true : CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
		m_strFileName.Clear();
		return ret;
	}

	virtual DWORD Read(LPVOID pBuff, DWORD nCount, DWORD nSize = sizeof(TCHAR))
	{
		if (!pBuff || nCount == 0 || nSize == 0) return 0;
		if (Eof()) return 0;

		DWORD len = nCount * nSize;
		::ReadFile(m_hFile, pBuff, len, &len, NULL);

		return (len / nSize);
	}
	virtual DWORD Write(LPCVOID pBuff, DWORD nCount, DWORD nSize = sizeof(TCHAR))
	{
		if (!pBuff || nCount == 0 || nSize == 0) return 0;
		if (Error()) return 0;

		DWORD len = nCount * nSize;
		::WriteFile(m_hFile, pBuff, len, &len, NULL);

		return (len / nSize);
	}

	virtual bool Seek(uint64_t nOffset, int iOrigin = current)
	{
		if (Error()) return false;
		if (iOrigin != begin && iOrigin != end && iOrigin != current)
			return false;

		LARGE_INTEGER liOff;
		liOff.QuadPart = nOffset;
		liOff.LowPart = ::SetFilePointer(m_hFile, liOff.LowPart, &liOff.HighPart, (DWORD)iOrigin);

		return (liOff.LowPart != (DWORD)-1);
	}
	virtual uint64_t Tell()
	{
		if (Error()) return -1;

		LARGE_INTEGER liPos;
		liPos.QuadPart = 0;
		liPos.LowPart = ::SetFilePointer(m_hFile, liPos.LowPart, &liPos.HighPart, FILE_CURRENT);
		if (liPos.LowPart == (DWORD)-1)
			return -1;

		return liPos.QuadPart;
	}
	virtual uint64_t Size()
	{
		if (Error()) return -1;

		ULARGE_INTEGER liSize;
		liSize.LowPart = ::GetFileSize(m_hFile, &liSize.HighPart);
		if (liSize.LowPart == INVALID_FILE_SIZE)
			return -1;

		return liSize.QuadPart;
	}
	virtual bool Flush()
	{
		if (Error()) return false;
		return ::FlushFileBuffers(m_hFile);
	}
	virtual bool Eof()
	{
		if (Error()) return true;

		uint64_t tell = Tell();
		if (tell == (uint64_t)-1) return true;
		uint64_t size = Size();
		if (size == (uint64_t)-1) return true;

		return (tell >= size);
	}
	virtual bool Error()
	{
		return (!m_hFile || m_hFile == INVALID_HANDLE_VALUE);
	}

	LPCTSTR GetFileName()
	{
		return (LPCTSTR)m_strFileName;
	}
	operator HANDLE()
	{
		return m_hFile;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__IOFile_h__*/