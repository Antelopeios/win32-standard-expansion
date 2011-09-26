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
// ThreadHeap - ϵͳ�̶߳���ӿ�
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-09-20
// Version:	1.0.0003.1706
//
// History:
//	- 1.0.0003.1706(2011-09-20) = ��handle_t�Ķ����Ƴ�
//////////////////////////////////////////////////////////////////

#ifndef __ThreadHeap_h__
#define __ThreadHeap_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CThreadHeapCreator
{
public:
	enum { IsPoolCreator = 0 };
	typedef CThreadHeapCreator creator_t;
	typedef DWORD (__stdcall* call_t)(LPVOID);

public:
	static HANDLE Create(_IN_ call_t lpStartAddr, 
					     _IN_ LPVOID lpParam = NULL, 
					     _IN_ DWORD dwFlag = 0, 
					     _OT_ LPDWORD lpIDThread = NULL)
	{ return CreateThread(NULL, 0, lpStartAddr, lpParam, dwFlag, lpIDThread); }
	static bool Close(HANDLE hTrd)
	{ return CloseHandle(hTrd); }

	static DWORD Suspend(HANDLE hTrd)
	{ return SuspendThread(hTrd); }
	static DWORD Resume(HANDLE hTrd)
	{ return ResumeThread(hTrd); }

	static bool Terminate(HANDLE hTrd, DWORD dwExitCode = 0)
	{ return TerminateThread(hTrd, dwExitCode); }
};

typedef CThreadHeapCreator _ThreadHeap;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ThreadHeap_h__*/