// TestStd6.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//////////////////////////////////////////////////////////////////

class CCrash : public ICrashHandler
{
public:
	void OnCrash()
	{
		ExMPrintf(_T("�ð�,�ҹ���...=.=\n����ת��������: %s\n������־������: %s\n������Ϣ���������:\n%s"), 
			(LPCTSTR)GetCrashDmp(), (LPCTSTR)GetCrashLog(), (LPCTSTR)GetCrashZip());
	}
} g_Crash;

//////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
	int c = 10;
	for(int i = 0; i <= c; ++i)
	{
		ExCrash(_T("c = %d, i = %d\n"), c, i);
		ExCPrintf(_T("%d / %d = %d\n"), c, (c - i), c / (c - i));
	}
	return 0;
}
