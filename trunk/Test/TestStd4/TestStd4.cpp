// TestStd4.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

CShrMutex g_mutex;
CString g_str(_T("\n"));
DWORD g_inx;

DWORD WINAPI Proc(LPVOID lpParam)
{
	int i = 0;
	if (lpParam)
	{	// ��
		while(i++ < 1000)
		{
			{
				ExLock(g_mutex, true, CShrMutex);
				ExCPrintf(g_str);
			}
			Sleep(5);
		}
	}
	else
	{	// д
		while(i++ < 1000)
		{
			{
				ExLock(g_mutex, false, CShrMutex);
				g_str.Format(_T("%d\n"), g_inx++);
			}
			Sleep(5);
		}
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	_tsystem(_T("pause"));

	HANDLE hdl = NULL;
	hdl = CThreadAdapterT<_ThreadPool>::Create(Proc, (LPVOID)0);
	hdl = CThreadAdapterT<_ThreadPool>::Create(Proc, (LPVOID)500);
	hdl = ExThread::Create(Proc, (LPVOID)0);
	hdl = ExThread::Create(Proc, (LPVOID)500);

	_tsystem(_T("pause"));
	return 0;
}