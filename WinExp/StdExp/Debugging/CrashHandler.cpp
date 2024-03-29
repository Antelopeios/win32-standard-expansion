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
// CrashHandler - 崩溃捕获
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-12-15
// Version:	1.0.0001.1145
//
// Reference:
//	- Mike Carruth's CrashRpt(http://www.codeproject.com/KB/debug/crash_report.aspx)
//
// History:
//	- 1.0.0001.1145(2011-12-15)	+ 添加ICrashHandler::GetCrashZip()接口,支持直接将崩溃信息打包为zip文件
//////////////////////////////////////////////////////////////////

#include "Common/Common.h"
#include "CrashHandler.h"
#include "File/File.h"
#include "Debugging/LogSystem.h"
#include "Debugging/XZipFile.h"

#include <DbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")

EXP_BEG

//////////////////////////////////////////////////////////////////

LPTOP_LEVEL_EXCEPTION_FILTER g_OldCrashFilter = NULL;
PEXCEPTION_POINTERS g_pExInfo = NULL;

struct _ModuleListEntry
{
	MINIDUMP_MODULE_CALLBACK item;
	_ModuleListEntry* next;
} g_ModHead, *g_ModTail = &g_ModHead;

CLog g_Log;

ICrashHandler* g_pCrashHandler = NULL;

//////////////////////////////////////////////////////////////////

void PrintfPolicy_Crach::Output(PCTSTR sOutputString)
{
	g_Log.Cout(sOutputString);
}

//////////////////////////////////////////////////////////////////

LONG WINAPI UnhandledExceptionFilter(PEXCEPTION_POINTERS pExInfo)
{
	g_pExInfo = pExInfo;
	if (g_pCrashHandler)
	{
		g_pCrashHandler->GetCrashDmp();
		g_pCrashHandler->GetCrashLog();
		g_pCrashHandler->OnCrash();
	}
	return EXCEPTION_EXECUTE_HANDLER;
}

BOOL CALLBACK MiniDumpCallback(PVOID, CONST PMINIDUMP_CALLBACK_INPUT CallbackInput, PMINIDUMP_CALLBACK_OUTPUT)
{
	if (ModuleCallback == CallbackInput->CallbackType)
	{
		g_ModTail->next = ExMem::Alloc<_ModuleListEntry>();
		g_ModTail = g_ModTail->next;
		g_ModTail->item = CallbackInput->Module;
		g_ModTail->item.FullPath = _wcsdup(CallbackInput->Module.FullPath);
		g_ModTail->next = NULL;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////

ICrashHandler::ICrashHandler()
{
	g_pCrashHandler = this;
	g_OldCrashFilter = SetUnhandledExceptionFilter(UnhandledExceptionFilter);
}

ICrashHandler::~ICrashHandler()
{
	_ModuleListEntry* node = g_ModHead.next;
	while(node)
	{
		_ModuleListEntry* temp = node;
		node = node->next;
		ExMem::Free(temp);
	}
	SetUnhandledExceptionFilter(g_OldCrashFilter);
}

LPCTSTR ICrashHandler::GetCrashPath()
{
	TCHAR app_path[MAX_PATH];
	::GetModuleFileName(NULL, app_path, MAX_PATH);
	_tcsrchr(app_path, _T('.'))[0] = _T('\0');

	TCHAR tmp_path[MAX_PATH]; size_t r_s = 0;
	_tgetenv_s(&r_s, tmp_path, _T("TEMP"));

	static TCHAR path[MAX_PATH];
	_tprintf_s(path, _T("%s\\%s"), tmp_path, _tcsrchr(app_path, _T('\\')) + 1);
	return path;
}

LPCTSTR ICrashHandler::GetCrashDmp()
{
	static TCHAR path[MAX_PATH];
	_tprintf_s(path, _T("%s.dmp"), GetCrashPath());

	CIOFile file;
	if (!file.Open(path, CIOFile::modeCreate | CIOFile::modeWrite)) return _T("");

	MINIDUMP_EXCEPTION_INFORMATION info;
	info.ThreadId = ::GetCurrentThreadId();
	info.ExceptionPointers = g_pExInfo;
	info.ClientPointers = FALSE;

	MINIDUMP_CALLBACK_INFORMATION mini_dump;
	mini_dump.CallbackRoutine = MiniDumpCallback;
	mini_dump.CallbackParam = 0;

	MiniDumpWriteDump(
		GetCurrentProcess(),
		GetCurrentProcessId(),
		file,
		static_cast<MINIDUMP_TYPE>(
			MiniDumpWithFullMemory | 
			MiniDumpWithDataSegs | 
			MiniDumpWithHandleData | 
			MiniDumpWithProcessThreadData | 
			MiniDumpScanMemory | 
			MiniDumpWithIndirectlyReferencedMemory),
		g_pExInfo ? &info : NULL,
		NULL,
		&mini_dump);

	return path;
}

LPCTSTR ICrashHandler::GetCrashLog()
{
	static TCHAR path[MAX_PATH];
	_tprintf_s(path, _T("%s.log"), GetCrashPath());

	CIOFile file;
	if (!file.Open(path, CIOFile::modeCreate | CIOFile::modeWrite)) return _T("");

	BOOL cout_time_old = g_Log.SetCoutTime(FALSE);

	_ModuleListEntry* node = g_ModHead.next;
	while(node)
	{
		ExCrash(_T("\nFullPath: %s\n"), node->item.FullPath);
		ExCrash(_T("BaseAddress: 0x%08X\n"), node->item.BaseOfImage);
		ExCrash(_T("Size: 0x%08X\n"), node->item.SizeOfImage);

		WIN32_FIND_DATA fd = {0};
		HANDLE file = ::FindFirstFile(node->item.FullPath, &fd);
		if (INVALID_HANDLE_VALUE != file)
		{
			::FindClose(file);
			FILETIME ft = {0};
			::FileTimeToLocalFileTime(&(fd.ftLastWriteTime), &ft);
			SYSTEMTIME st = {0};
			::FileTimeToSystemTime(&ft, &st);
			ExCrash(_T("TimeStamp: %02u/%02u/%04u %02u:%02u:%02u\n"), 
				st.wMonth, 
				st.wDay, 
				st.wYear, 
				st.wHour, 
				st.wMinute, 
				st.wSecond);
		}

		ExCrash(_T("FileVersion: %d.%d.%d.%d\n"), 
			HIWORD(node->item.VersionInfo.dwFileVersionMS),
			LOWORD(node->item.VersionInfo.dwFileVersionMS),
			HIWORD(node->item.VersionInfo.dwFileVersionLS),
			LOWORD(node->item.VersionInfo.dwFileVersionLS));
		ExCrash(_T("ProductVersion: %d.%d.%d.%d\n"), 
			HIWORD(node->item.VersionInfo.dwProductVersionMS),
			LOWORD(node->item.VersionInfo.dwProductVersionMS),
			HIWORD(node->item.VersionInfo.dwProductVersionLS),
			LOWORD(node->item.VersionInfo.dwProductVersionLS));

		node = node->next;
	}

	g_Log.SetCoutTime(cout_time_old);

	g_Log.ToFile(&file);
	return path;
}

LPCTSTR ICrashHandler::GetCrashZip(LPCTSTR sPath/* = NULL*/)
{
	static TCHAR path[MAX_PATH];
	if (sPath)
		_tcscpy_s(path, sPath);
	else
		path[0] = 0;
	if (_tcslen(path) == 0)
	{
		// 得到压缩包文件名
		TCHAR app_path[MAX_PATH];
		GetModuleFileName(NULL, app_path, MAX_PATH);
		_tcsrchr(app_path, _T('.'))[0] = _T('\0');
		SYSTEMTIME sys = {0};
		GetLocalTime(&sys);
		_tprintf_s(path, _T("%s_%4d.%02d.%02d_%02d.%02d.%02d_%03d.zip"), (LPCTSTR)app_path, 
			sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	}
	// 生成压缩包并压缩文件
	CXZip zip(path);
	zip.ZipFile(GetCrashDmp());
	zip.ZipFile(GetCrashLog());
	return path;
}

//////////////////////////////////////////////////////////////////

EXP_END
