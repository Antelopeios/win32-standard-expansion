// Copyright 2011, Ä¾Í·ÔÆ
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
// CrashHandler - ±ÀÀ£²¶»ñ
//
// Author:	Ä¾Í·ÔÆ
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-12-13
// Version:	1.0.0000.1640
//
// Reference:
//	- Mike Carruth's CrashRpt(http://www.codeproject.com/KB/debug/crash_report.aspx)
//////////////////////////////////////////////////////////////////

#ifndef __CrashHandler_h__
#define __CrashHandler_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Debugging/Trace.h"
#include "Container/Container.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class PrintfPolicy_Crach
{
public:
	static void Output(PCTSTR sOutputString);
};

#undef	ExCrash
#define ExCrash EXP::ExPrintfT<EXP::PrintfPolicy_Crach>

//////////////////////////////////////////////////////////////////

interface ICrashHandler
{
	ICrashHandler();
	virtual ~ICrashHandler();

	virtual void OnCrash() = 0;

	const CString& GetCrashPath();
	const CString& GetCrashDmp();
	const CString& GetCrashLog();
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__CrashHandler_h__*/