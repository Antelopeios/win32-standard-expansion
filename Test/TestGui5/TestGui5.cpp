// TestGui5.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestGui5.h"

#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

class CApp : public IApp
{
public:
	CApp(BOOL bUIApp = TRUE)
		: IApp(bUIApp)
	{}

protected:
	DWORD OnInit()
	{
		DWORD ret = IApp::OnInit();

		CIOFile file(_T("TestGui5\\test.xml"));
		if (file.Error() || file.Size() == 0) return 0;
		CGuiSkin::Load(&file);
		
	/*	unsigned int tStart = 0, tEnd = 0;

		CIOFile file(_T("TestGui5\\test.xml"));
		if (file.Error() || file.Size() == 0) return 0;

		CGuiXML xml;
		xml.SetFile(&file);

		//timeBeginPeriod(1);
		//tStart = timeGetTime();
		if (!xml.Decode()) return 0;
		//tEnd = timeGetTime();
		//timeEndPeriod(1);
		//ExCPrintf(_T("%dms\n"), (tEnd - tStart));

		//CGuiXML::iterator_t ite;
		//xml.GetNode(_T("skin"), ite = xml.GetRoot());		// ��λskin���
		//xml.AddNode(_T("test"), ite);						// ���test���
		//xml.AddAttr(_T("key"), _T("val_test"), ite);		// ���test����key����
		//xml.GetNode(_T("font"), ite = xml.GetRoot());		// ��λfont���
		//xml.GetNode(_T("font"), ite);						// ��λ�ڶ���font���(���ݷ���ֵ�ж��Ƿ�ɹ�)
		//CString face = xml.GetAttr(_T("face"), ite);
		//xml.DelNode(ite);									// ɾ���ڶ���font���
		//xml.GetNode(_T("detail"), ite = xml.GetRoot());		// ��λdetail���
		//xml.AddAttr(_T("author"), _T("DarkCat"), ite);		// �޸�/���detail����author����
		//xml.DelAttr(_T("name"), ite);						// ɾ��detail����name����

		file.Open(_T("TestGui5\\test_encode.xml"), CIOFile::modeWrite | CIOFile::modeCreate);
		if (!xml.Encode()) return 0;*/

		return ret;
	}
} g_App;