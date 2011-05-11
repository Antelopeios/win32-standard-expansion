// TestStd4.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

CMutex g_mutex;
CString g_str(_T("\n"));
volatile LONG g_inx, g_cnt;

typedef CNonLockT<CString> nl_string_t;
nl_string_t g_non;

CEvent g_start(true), g_end(false);

DWORD WINAPI Proc(LPVOID lpParam)
{
	int i = 0;
	g_start.Wait();
	if (lpParam)
	{	// 读
		while(i++ < 1000)
		{
			{
				ExLock(g_mutex, true);
				ExCPrintf(g_str);
				//CNonLockerT<nl_string_t> noner(g_non);
				//ExCPrintf(*noner);
			}
			Sleep(5);
		}
	}
	else
	{	// 写
		while(i++ < 1000)
		{
			{
				ExLock(g_mutex, false);
				g_str.Format(_T("%d\n"), g_inx++);
				//CString str;
				//str.Format(_T("%d\n"), EXP_THREAD_MODEL::Inc(&g_inx));
				//g_non.Set(str);
			}
			Sleep(5);
		}
		ExLock(g_mutex, true);
		ExCPrintf(g_str);
	}

	if (EXP_THREAD_MODEL::Dec(&g_cnt) == 0)
		g_end.Set();

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	_tsystem(_T("pause"));

	g_non.Set(_T("\n"));

	//HANDLE hdl = NULL;
	for (int i = 0; i < 100; ++i && ++g_cnt)
		//hdl = CThreadAdapterT<_ThreadPool>::Create(Proc, (LPVOID)0);
		ExThread::Close(ExThread::Create(Proc, (LPVOID)0));
	for (int i = 0; i < 100; ++i && ++g_cnt)
		//hdl = CThreadAdapterT<_ThreadPool>::Create(Proc, (LPVOID)1);
		ExThread::Close(ExThread::Create(Proc, (LPVOID)1));

	g_start.Set();
	unsigned int tStart = 0, tEnd = 0;
	timeBeginPeriod(1);
	tStart = timeGetTime();
	g_end.Wait();
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("\n%dms\n\n"), (tEnd - tStart));

	_tsystem(_T("pause"));
	//CThreadAdapterT<_ThreadPool>::GetCreator().Clear();

	return 0;
}