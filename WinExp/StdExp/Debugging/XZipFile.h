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
// XZipFile - ZIP格式文件打包
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-02-13
// Version:	1.0.0002.1628
//
// History:
//	- 1.0.0002.1628(2012-02-13)	+ 添加zip解压缩相关算法
//////////////////////////////////////////////////////////////////

#ifndef __XZipFile_h__
#define __XZipFile_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Debugging/XZip/XZip.h"
#include "Debugging/XZip/XUnzip.h"
#include "Memory/Memory.h"
#include "Container/String.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CXZip
{
public:
	typedef HZIP zip_t;

protected:
	zip_t m_ZipFile;

public:
	CXZip()
		: m_ZipFile(NULL)
	{
	}
	CXZip(LPCTSTR sPath)
	{
		this->CXZip::CXZip();
		Open(sPath);
	}
	CXZip(BYTE* buff, DWORD size)
	{
		this->CXZip::CXZip();
		Open(buff, size);
	}
	virtual ~CXZip()
	{
		Close();
	}

public:
	EXP_INLINE BOOL IsClosed()
	{
		return (m_ZipFile == NULL);
	}
	BOOL Open(LPCTSTR sPath)
	{
		CString path(sPath);
		if (path.Empty()) return FALSE;

		Close();

		WIN32_FIND_DATA fd = {0};
		HANDLE find = ::FindFirstFile(path, &fd);
		if (find == INVALID_HANDLE_VALUE)
		{	// 创建zip文件
			m_ZipFile = CreateZip((LPTSTR)path, 0, ZIP_FILENAME);
		}
		else
		{	// 打开zip文件
			::FindClose(find);
			m_ZipFile = OpenZip((LPTSTR)path, 0, ZIP_FILENAME);
		}
		return (!IsClosed());
	}
	BOOL Open(BYTE* buff, DWORD size)
	{
		Close();
		m_ZipFile = OpenZip(buff, size, ZIP_MEMORY);
		return (!IsClosed());
	}
	void Close()
	{
		if (IsClosed()) return;
		CloseZip(m_ZipFile);
		m_ZipFile = NULL;
	}

	BOOL ZipFile(LPCTSTR sPath)
	{
		if (IsClosed()) return FALSE;
		CString path(sPath);
		if (path.Empty()) return FALSE;

		WIN32_FIND_DATA fd = {0};
		HANDLE find = ::FindFirstFile(path, &fd);
		if (find == INVALID_HANDLE_VALUE) return FALSE;
		::FindClose(find);

		CString sFileName = path.Mid(path.RevFind(_T('\\'))->Index() + 1);
		return (ZR_OK == ZipAdd(m_ZipFile, sFileName, (LPTSTR)path, 0, ZIP_FILENAME));
	}
	BOOL UnzipFile(LPCTSTR sPath)
	{
		if (IsClosed()) return FALSE;
		CString path(sPath);
		if (path.Empty()) return FALSE;
		if (path.LastItem() != _T('\\'))
			path.PushLast(_T('\\'));

	#ifdef _UNICODE
		ZIPENTRYW ze;
	#else
		ZIPENTRY ze; 
	#endif
		ZRESULT zr = GetZipItem(m_ZipFile, -1, &ze);
		if (zr != ZR_OK) return FALSE;

		BOOL ret = FALSE;
		int i = 0, count = ze.index;
		for(; i < count; ++i)
		{
			ZRESULT zr = GetZipItem(m_ZipFile, i, &ze);
			if (zr != ZR_OK) break;
			zr = UnzipItem(m_ZipFile, ze.index, (void*)(LPCTSTR)(path + ze.name), 0, ZIP_FILENAME);
			if (zr != ZR_OK) break;
		}
		if (i == count) ret = TRUE;
		return ret;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__XZipFile_h__*/