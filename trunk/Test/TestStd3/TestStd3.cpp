// TestStd3.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	{
		CIOFile file(_T("test.txt"), CIOFile::modeCreate | CIOFile::modeReadWrite);
		CString str = _T("Test File!");
		DWORD len = str.GetLength();
		file.Write(str, len);
		file.Seek(0, CIOFile::begin);
		str.Clear();
		file.Read(str.GetCStr(len), len);
		ExCPrintf(_T("IOFile:%s\n"), (LPCTSTR)str);
	}
	DeleteFile(_T("test.txt"));
	{
		CMemFile file;
		CString str = _T("Test File!");
		DWORD len = str.GetLength();
		file.Write(str, len);
		file.Seek(0, CMemFile::begin);
		str.Clear();
		file.Read(str.GetCStr(len), len);
		ExCPrintf(_T("MemFile:%s\n"), (LPCTSTR)str);
	}
	_tsystem(_T("pause"));
	return 0;
}