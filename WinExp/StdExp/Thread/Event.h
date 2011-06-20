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
// Event - 事件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-02-15
// Version:	1.1.0003.1000
//////////////////////////////////////////////////////////////////

#ifndef __Event_h__
#define __Event_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Thread/SyncObject.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CEvent : public ISyncObject
{
public:
	CEvent()
		: ISyncObject()
	{}
	CEvent(bool bManualReset, bool bInitialState = false)
		: ISyncObject()
	{ Create(bManualReset, bInitialState); }
	~CEvent()
	{}

public:
	bool Create(bool bManualReset = false, bool bInitialState = false)
	{
		Close();
		m_hSync = CreateEvent(0, bManualReset, bInitialState, NULL);
		if (m_hSync == NULL) m_hSync = INVALID_HANDLE_VALUE;
		if (m_hSync == INVALID_HANDLE_VALUE) return false;
		return true;
	}

	bool Set()
	{
		if (IsClosed()) return false;
		return SetEvent(m_hSync);
	}
	bool Reset()
	{
		if (IsClosed()) return false;
		return ResetEvent(m_hSync);
	}
	bool IsSetted()
	{ return (Wait(0) != WAIT_TIMEOUT); }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Event_h__*/