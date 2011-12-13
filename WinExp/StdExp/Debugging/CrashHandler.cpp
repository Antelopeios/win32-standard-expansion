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
//////////////////////////////////////////////////////////////////

#include "Common/Common.h"
#include "CrashHandler.h"
#include "File/File.h"
#include "Memory/Memory.h"
#include "Container/Container.h"
#include "Debugging/LogSystem.h"

#include <DbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")

EXP_BEG

//////////////////////////////////////////////////////////////////

ICrashHandler* g_pCrashHandler = NULL;
PEXCEPTION_POINTERS g_pExInfo = NULL;

struct _ModuleListEntry
{
	MINIDUMP_MODULE_CALLBACK item;
	struct _ModuleListEntry *next;
} g_ModHead, *g_ModTail;

CLog g_Log;

//////////////////////////////////////////////////////////////////

LONG WINAPI UnhandledExceptionFilter(PEXCEPTION_POINTERS pExInfo)
{
	g_pExInfo = pExInfo;
	if (g_pCrashHandler) g_pCrashHandler->OnCrash();
	return EXCEPTION_EXECUTE_HANDLER;
}

BOOL CALLBACK MiniDumpCallback(PVOID, CONST PMINIDUMP_CALLBACK_INPUT CallbackInput, PMINIDUMP_CALLBACK_OUTPUT)
{
	if (ModuleCallback == CallbackInput->CallbackType)
	{
		g_ModTail->next = ExMem::Alloc(_ModuleListEntry);
		g_ModTail = g_ModTail->next;
		g_ModTail->item = CallbackInput->Module;
		g_ModTail->item.FullPath = _wcsdup(CallbackInput->Module.FullPath);
		g_ModTail->next = NULL;
	}
	return TRUE;
}

CString GetCrashPath()
{
	TCHAR app_path[MAX_PATH];
	::GetModuleFileName(NULL, app_path, MAX_PATH);
	_tcsrchr(app_path, _T('.'))[0] = _T('\0');

	TCHAR tmp_path[MAX_PATH]; size_t r_s = 0;
	_tgetenv_s(&r_s, tmp_path, _T("TEMP"));

	CString path;
	path.Format(_T("%s\\%s"), tmp_path, app_path);
	return path;
}

//////////////////////////////////////////////////////////////////

CString ICrashHandler::GetCrashFile()
{
	CString path;
	path.Format(_T("%s.dmp"), (LPCTSTR)GetCrashPath());

	CIOFile file;
	if (!file.Open(path, CIOFile::modeCreate | CIOFile::modeWrite)) return NULL;

	MINIDUMP_EXCEPTION_INFORMATION info;
	info.ThreadId = GetCurrentThreadId();
	info.ExceptionPointers = g_pExInfo;
	info.ClientPointers = FALSE;

	MINIDUMP_CALLBACK_INFORMATION mini_dump;
	mini_dump.CallbackRoutine = MiniDumpCallback;
	mini_dump.CallbackParam = 0;

	MiniDumpWriteDump(
		GetCurrentProcess(),
		GetCurrentProcessId(),
		file,
		static_cast<MINIDUMP_TYPE>(DUMP_TYPE),
		g_pExInfo ? &info : NULL,
		NULL,
		&mini_dump);

	return path;
}

CString ICrashHandler::GetCrashLog()
{
	CString path;
	path.Format(_T("%s.log"), (LPCTSTR)GetCrashPath());

	_ModuleListEntry* node = g_ModHead->next;
	while (node)
	{
		char temp[MAX_PATH];
		::CharToOemW(node->item.FullPath, temp);
		sprintf_s(m_Cache.GetCurrBuff(), MAX_BUF_LIMIT, "\r\nFullPath: %s\r\n", temp);
		m_Cache.SetCurrBuff();

		sprintf_s(m_Cache.GetCurrBuff(), MAX_BUF_LIMIT, "BaseAddress: 0x%08X\r\n", node->item.BaseOfImage);
		m_Cache.SetCurrBuff();

		sprintf_s(m_Cache.GetCurrBuff(), MAX_BUF_LIMIT, "Size: 0x%08X\r\n", node->item.SizeOfImage);
		m_Cache.SetCurrBuff();

		FILETIME    ft = CUtility::getLastWriteFileTime(node->item.FullPath);
		SYSTEMTIME  st = {0};
		FileTimeToSystemTime(&ft, &st);
		sprintf_s(m_Cache.GetCurrBuff(), MAX_BUF_LIMIT, "TimeStamp: %02u/%02u/%04u %02u:%02u:%02u\r\n", 
			st.wMonth, 
			st.wDay, 
			st.wYear, 
			st.wHour, 
			st.wMinute, 
			st.wSecond);
		m_Cache.SetCurrBuff();

		sprintf_s(m_Cache.GetCurrBuff(), MAX_BUF_LIMIT, "FileVersion: %d.%d.%d.%d\r\n", 
			HIWORD(node->item.VersionInfo.dwFileVersionMS),
			LOWORD(node->item.VersionInfo.dwFileVersionMS),
			HIWORD(node->item.VersionInfo.dwFileVersionLS),
			LOWORD(node->item.VersionInfo.dwFileVersionLS));
		m_Cache.SetCurrBuff();

		sprintf_s(m_Cache.GetCurrBuff(), MAX_BUF_LIMIT, "ProductVersion: %d.%d.%d.%d\r\n", 
			HIWORD(node->item.VersionInfo.dwProductVersionMS),
			LOWORD(node->item.VersionInfo.dwProductVersionMS),
			HIWORD(node->item.VersionInfo.dwProductVersionLS),
			LOWORD(node->item.VersionInfo.dwProductVersionLS));
		m_Cache.SetCurrBuff();

		node = node->next;
	}
}

//////////////////////////////////////////////////////////////////

EXP_END
