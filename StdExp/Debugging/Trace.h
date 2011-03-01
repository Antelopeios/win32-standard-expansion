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
// Trace - 跟踪
//
// Author:	木头云
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-03-01
// Version:	1.0.0003.2359
//
// History:
//	- 1.0.0003.2359(2011-03-01)	= ExPrintfT()内部的内存分配不使用ExMem(),直接采用CMemHeapAlloc完成
//////////////////////////////////////////////////////////////////

#ifndef __Trace_h__
#define __Trace_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/MemHeap.h"
#include "Thread/Lock.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename PolicyT>
inline void __cdecl ExPrintfT(LPCTSTR lpFormat, ...)
{
	if (!lpFormat) return;

	ExLockThis();

	va_list args;
	va_start(args, lpFormat);

	size_t len = _vsctprintf(lpFormat, args) + 1;
	TCHAR* buf = (TCHAR*)CMemHeapAlloc::Alloc(len * sizeof(TCHAR));
	if (!buf) return;
	HRESULT r = StringCchVPrintf(buf, len, lpFormat, args);

	va_end(args);

	if( r == S_OK ) PolicyT::Output(buf);
	if (buf) CMemHeapAlloc::Free(buf);
}

//////////////////////////////////////////////////////////////////

class PrintfPolicy_Debug
{
public:
	static void Output(PCTSTR sOutputString)
	{ OutputDebugString(sOutputString); }
};

class PrintfPolicy_Console
{
private:
	PrintfPolicy_Console()
	{ AllocConsole(); }
public:
	~PrintfPolicy_Console()
	{ FreeConsole(); }
public:
	static void RunConsole()
	{ static PrintfPolicy_Console run; }
	static void Output(PCTSTR sOutputString)
	{
		HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
		if( INVALID_HANDLE_VALUE == hdl || NULL == hdl )
			RunConsole();
		_tcprintf_s(sOutputString);
	}
};

class PrintfPolicy_Message
{
public:
	static void Output(PCTSTR sOutputString)
	{ ExMB(sOutputString, MB_ICONINFORMATION); }
};

//////////////////////////////////////////////////////////////////

#undef	ExDPrintf
#define ExDPrintf	ExPrintfT<PrintfPolicy_Debug>
#undef	ExCPrintf
#define ExCPrintf	ExPrintfT<PrintfPolicy_Console>
#undef	ExMPrintf
#define ExMPrintf	ExPrintfT<PrintfPolicy_Message>

#undef	ExTrace
#ifdef	_DEBUG
#define ExTrace		ExDPrintf
#else /*_DEBUG*/
#define ExTrace		__noop
#endif/*_DEBUG*/

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Trace_h__*/