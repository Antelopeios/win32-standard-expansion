// TestStd6.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//////////////////////////////////////////////////////////////////

class CCrash : public ICrashHandler
{
public:
	void OnCrash()
	{
		ExMPrintf(_T("好吧,我挂了...=.=\n崩溃转储在这里: %s\n崩溃日志在这里: %s\n崩溃信息打包到这里:\n%s"), 
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
