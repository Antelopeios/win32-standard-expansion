// TestGui5.cpp : 定义控制台应用程序的入口点。
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
	xml.GetNode(_T("skin"), ite = xml.GetRoot());		// 定位skin结点
	xml.AddNode(_T("test"), ite);						// 添加test结点
	xml.AddAttr(_T("key"), _T("val_test"), ite);		// 添加test结点的key属性
	xml.GetNode(_T("font"), ite = xml.GetRoot());		// 定位font结点
	xml.GetNode(_T("font"), ite);						// 定位第二个font结点(根据返回值判断是否成功)
	xml.DelNode(ite);									// 删除第二个font结点
	xml.GetNode(_T("detail"), ite = xml.GetRoot());		// 定位detail结点
	xml.AddAttr(_T("author"), _T("DarkCat"), ite);		// 修改/添加detail结点的author属性
	xml.DelAttr(_T("name"), ite);						// 删除detail结点的name属性

	file.Open(_T("test_encode.xml"), CIOFile::modeWrite | CIOFile::modeCreate);
	if (!xml.Encode()) return 0;

	return 0;
}