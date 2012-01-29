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
// ThreadCreator - 线程创建器
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-28
// Version:	1.1.0004.1407
//
// History:
//	- 1.0.0003.1706(2011-09-20) = 将handle_t的定义移除
//	- 1.1.0010.1407(2012-01-28)	= CThreadHeapCreator改为CThreadCreator,并移除_ThreadHeap定义
//								= 将EXP_THREAD_CREATOR与ExThread定义为CThreadCreator的别称
//////////////////////////////////////////////////////////////////

#ifndef __ThreadCreator_h__
#define __ThreadCreator_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CThreadCreator
{
public:
	typedef CThreadCreator creator_t;
	typedef DWORD (__stdcall* call_t)(LPVOID);

public:
	EXP_INLINE static HANDLE Create(_IN_ call_t lpStartAddr, 
									_IN_ LPVOID lpParam = NULL, 
									_IN_ DWORD dwFlag = 0, 
									_OT_ LPDWORD lpIDThread = NULL)
	{ return CreateThread(NULL, 0, lpStartAddr, lpParam, dwFlag, lpIDThread); }
	EXP_INLINE static BOOL Close(HANDLE hTrd)
	{ return CloseHandle(hTrd); }

	EXP_INLINE static DWORD Suspend(HANDLE hTrd)
	{ return SuspendThread(hTrd); }
	EXP_INLINE static DWORD Resume(HANDLE hTrd)
	{ return ResumeThread(hTrd); }

	EXP_INLINE static BOOL Terminate(HANDLE hTrd, DWORD dwExitCode = 0)
	{ return TerminateThread(hTrd, dwExitCode); }
};

#ifndef EXP_THREAD_CREATOR
#define EXP_THREAD_CREATOR CThreadCreator
#endif/*EXP_THREAD_CREATOR*/

typedef EXP_THREAD_CREATOR ExThread;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ThreadCreator_h__*/