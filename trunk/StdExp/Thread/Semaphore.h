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
// Semaphore - 信号量
//
// Author:	木头云
// Blog:	http://www.cnblogs.com/black-cat-ptr/
// E-Mail:	mark.lonr@tom.com
// Date:	2011-02-15
// Version:	1.0.0001.1000
//////////////////////////////////////////////////////////////////

#ifndef __Semaphore_h__
#define __Semaphore_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Thread/SyncObject.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CSemaphore : public ISyncObject
{
public:
	CSemaphore(LONG lInitialCount = 1, LONG lMaxCount = 1, LPCTSTR pstrName = NULL, LPSECURITY_ATTRIBUTES lpsaAttributes = NULL)
		: ISyncObject()
	{ Create(lInitialCount, lMaxCount, pstrName, lpsaAttributes); }
	~CSemaphore()
	{}

public:
	bool Create(LONG lInitialCount = 1, LONG lMaxCount = 1, LPCTSTR pstrName = NULL, LPSECURITY_ATTRIBUTES lpsaAttributes = NULL)
	{
		Close();
		m_hSync = CreateSemaphore(lpsaAttributes, lInitialCount, lMaxCount, pstrName);
		if (m_hSync == NULL) m_hSync = INVALID_HANDLE_VALUE;
		if (m_hSync == INVALID_HANDLE_VALUE) return false;
		return true;
	}

	bool Release(LONG lReleaseCount = 1, LPLONG lpPreviousCount = NULL)
	{
		if (IsClosed()) return false;
		return ReleaseSemaphore(m_hSync, lReleaseCount, lpPreviousCount);
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Semaphore_h__*/