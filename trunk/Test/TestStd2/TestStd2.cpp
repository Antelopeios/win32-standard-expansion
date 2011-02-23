// TestStd2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CArrayT<CString> as1;
	as1.Add(_T("123"), as1.Head());
	as1.Add(_T("321"), as1.Head());
	as1.Add(_T("312"), as1.Head());

	for(DWORD i = 0; i < as1.GetCount(); ++i)
		ExCPrintf(_T("%s\n"), (LPCTSTR)as1[i]);

	ExCPrintf(_T("\n"));

	for(CArrayT<CString>::iterator_t ite = as1.Head(); ite != as1.Tail(); ++ite)
		ExCPrintf(_T("%s\n"), (LPCTSTR)ite->Val());

	ExCPrintf(_T("\n"));

	/////////////////////////////////

	CListT<CString> as2;
	as2.Add(_T("123"), as2.Head());
	as2.Add(_T("321"), as2.Head());
	as2.Add(_T("312"), as2.Head());

	for(CListT<CString>::iterator_t ite = as2.Head(); ite != as2.Tail(); ++ite)
		ExCPrintf(_T("%s\n"), (LPCTSTR)ite->Val());

	ExCPrintf(_T("\n"));

	/////////////////////////////////

	CMapT<CString, DWORD> as3;
	as3.Add(_T("1"), 1);
	ExCPrintf(_T("%d\n"), as3[_T("1")]);
	as3[_T("2")] = 2;
	ExCPrintf(_T("%d%d\n"), as3[_T("1")], as3[_T("2")]);
	as3.Add(_T("3"), 3);
	ExCPrintf(_T("%d%d%d\n"), as3[_T("1")], as3[_T("2")], as3[_T("3")]);
	as3.Del(_T("3"));

	for(CMapT<CString, DWORD>::iterator_t ite = as3.Head(); ite != as3.Tail(); ++ite)
		ExCPrintf(_T("%s <=> %d\n"), (LPCTSTR)ite->Key(), ite->Val());

	_tsystem(_T("pause"));
	return 0;
}
