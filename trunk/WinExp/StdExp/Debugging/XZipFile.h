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
// XZipFile - ZIP格式文件打包
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-12-15
// Version:	1.0.0001.1100
//////////////////////////////////////////////////////////////////

#ifndef __XZipFile_h__
#define __XZipFile_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Debugging/XZip/XZip.h"
#include "Memory/Memory.h"
#include "Container/String.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CXZip
{
protected:
	typedef HZIP zip_t;
	zip_t m_ZipFile;

public:
	CXZip(LPCTSTR sPath = NULL)
		: m_ZipFile(NULL)
	{
		if (sPath) Open(sPath);
	}
	virtual ~CXZip()
	{
		if (m_ZipFile) Close();
	}

public:
	BOOL Open(LPCTSTR sPath)
	{
		CString path(sPath);
		if (path.Empty()) return FALSE;
		m_ZipFile = CreateZip((LPTSTR)path, 0, ZIP_FILENAME);
		return (m_ZipFile != NULL);
	}
	void Close()
	{
		if (m_ZipFile) CloseZip(m_ZipFile);
		m_ZipFile = NULL;
	}
	BOOL AddFile(LPCTSTR sPath)
	{
		CString path(sPath);
		if (path.Empty()) return FALSE;
		WIN32_FIND_DATA fd = {0};
		HANDLE find = ::FindFirstFile(path, &fd);
		if (find == INVALID_HANDLE_VALUE) return FALSE;
		::FindClose(find);
		// Trim path off file name
		CString sFileName = path.Mid(path.RevFind(_T('\\'))->Index() + 1);
		// Start a new file in Zip
		return (ZR_OK == ZipAdd(m_ZipFile, sFileName, (LPTSTR)path, 0, ZIP_FILENAME));
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__XZipFile_h__*/