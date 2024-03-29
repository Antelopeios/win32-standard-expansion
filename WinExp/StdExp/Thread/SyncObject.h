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
// SyncObject - 同步对象基类
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-12-08
// Version:	1.0.0004.0946
//
// History:
//	- 1.0.0004.0946(2011-12-08)	+ 添加一个新的静态Wait()接口,支持等待一个外部同步对象
//////////////////////////////////////////////////////////////////

#ifndef __SyncObject_h__
#define __SyncObject_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Algorithm/NonCopyable.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

interface ISyncObject : INonCopyable
{
protected:
	HANDLE m_hSync;

public:
	ISyncObject()
		: m_hSync(INVALID_HANDLE_VALUE)
	{}
	virtual ~ISyncObject()
	{ Close(); }

public:
	BOOL IsClosed() const
	{ return (!m_hSync || m_hSync == INVALID_HANDLE_VALUE); }
	BOOL Close()
	{
		BOOL ret = TRUE;
		if(!IsClosed())
			ret = CloseHandle(m_hSync);
		m_hSync = INVALID_HANDLE_VALUE;
		return ret;
	}

	DWORD Wait(DWORD dwWaitTime = INFINITE)
	{
		if (IsClosed()) return WAIT_FAILED;
		return Wait(m_hSync, dwWaitTime);
	}
	static DWORD Wait(HANDLE hSync, DWORD dwWaitTime/* = INFINITE*/)
	{
		if (hSync == NULL) return WAIT_FAILED;
		return WaitForSingleObject(hSync, dwWaitTime);
	}
	static DWORD Wait(HANDLE* Syncs, DWORD nCount, BOOL bWaitAll, DWORD dwWaitTime = INFINITE)
	{
		if (Syncs == NULL || nCount == 0) return WAIT_FAILED;
		return WaitForMultipleObjects(nCount, Syncs, bWaitAll, dwWaitTime);
	}
	static DWORD Wait(ISyncObject* Syncs, DWORD nCount, BOOL bWaitAll, DWORD dwWaitTime = INFINITE)
	{
		HANDLE syncs[MAXIMUM_WAIT_OBJECTS];
		DWORD limit = min(MAXIMUM_WAIT_OBJECTS, nCount);
		for(DWORD i = 0; i < limit; ++i) syncs[i] = Syncs[i].GetHandle();
		return Wait(syncs, limit, bWaitAll, dwWaitTime);
	}
	template <DWORD SizeT>
	static DWORD Wait(HANDLE (&Syncs)[SizeT], BOOL bWaitAll, DWORD dwWaitTime = INFINITE)
	{ return Wait(Syncs, SizeT, bWaitAll, dwWaitTime); }
	template <DWORD SizeT>
	static DWORD Wait(ISyncObject (&Syncs)[SizeT], BOOL bWaitAll, DWORD dwWaitTime = INFINITE)
	{ return Wait(Syncs, SizeT, bWaitAll, dwWaitTime); }

	HANDLE GetHandle() const { return m_hSync; }
	operator HANDLE() { return GetHandle(); }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__SyncObject_h__*/