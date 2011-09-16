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

	CGuiXML::iterator_t ite;
	xml.GetNode(_T("skin"), ite = xml.GetRoot());		// ��λskin���
	xml.AddNode(_T("test"), ite);						// ���test���
	xml.AddAttr(_T("key"), _T("val_test"), ite);		// ���test����key����
	xml.GetNode(_T("font"), ite = xml.GetRoot());		// ��λfont���
	xml.GetNode(_T("font"), ite);						// ��λ�ڶ���font���(���ݷ���ֵ�ж��Ƿ�ɹ�)
	xml.DelNode(ite);									// ɾ���ڶ���font���
	xml.GetNode(_T("detail"), ite = xml.GetRoot());		// ��λdetail���
	xml.AddAttr(_T("author"), _T("DarkCat"), ite);		// �޸�/���detail����author����
	xml.DelAttr(_T("name"), ite);						// ɾ��detail����name����

	file.Open(_T("test_encode.xml"), CIOFile::modeWrite | CIOFile::modeCreate);
	if (!xml.Encode()) return 0;

	return 0;
}