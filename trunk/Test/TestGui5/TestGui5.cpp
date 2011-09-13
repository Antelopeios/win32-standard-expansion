// TestGui5.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CIOFile file(_T("test.xml"));
	if (file.Error() || file.Size() == 0) return 0;

	CGuiXML xml;
	xml.SetFile(&file);
	if (!xml.Decode()) return 0;

	file.Open(_T("test_encode.xml"), CIOFile::modeWrite | CIOFile::modeCreate);
	if (!xml.Encode()) return 0;

	return 0;
}

