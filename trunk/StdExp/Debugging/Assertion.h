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
// Assertion - 断言
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-03-01
// Version:	1.2.0005.2345
//
// History:
//	- 1.2.0005.2345(2011-03-01)	= 调整ExDebugBreak定义为ExDebugBreak()
//////////////////////////////////////////////////////////////////

#ifndef __Assertion_h__
#define __Assertion_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

#ifdef _X86_
#define ExDebugBreak()	_asm { int 3 }
#else
#define ExDebugBreak()	__noop
#endif

EXP_INLINE void ExMBA(PCSTR csMsg, UINT uType = MB_ICONERROR)
{
	char c_title[MAX_PATH];
	GetModuleFileNameA(NULL, c_title, _countof(c_title));
	MessageBoxA(NULL, csMsg, c_title, MB_OK | uType);
}

EXP_INLINE void ExMBW(PCWSTR wsMsg, UINT uType = MB_ICONERROR)
{
	WCHAR wc_title[MAX_PATH];
	GetModuleFileNameW(NULL, wc_title, _countof(wc_title));
	MessageBoxW(NULL, wsMsg, wc_title, MB_OK | uType);
}

#ifdef  _UNICODE
#define ExMB	ExMBW
#else /*_UNICODE*/
#define ExMB	ExMBA
#endif/*_UNICODE*/

EXP_INLINE void ExFail(PSTR csMsg)
{
	ExMBA(csMsg);
	ExDebugBreak();
}

EXP_INLINE void ExAssertFail(PCSTR csFile, int iLine, PCSTR csExpr)
{
	char c_str[MAX_PATH];
	StringCchPrintfA(c_str, _countof(c_str), "文件: %s\n行号: %d\n语句: %s", csFile, iLine, csExpr);
	ExFail(c_str);
}

#ifdef _DEBUG
#define ExAssert(x)		if (!(x)) ExAssertFail(__FILE__, __LINE__, #x)
#else
#define ExAssert(x)		__noop
#endif

#ifdef _DEBUG
#define ExVerify(x)		ExAssert(x)
#else
#define ExVerify(x)		(x)
#endif

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Assertion_h__*/
