// TestStd2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <list>
#include <vector>
#include <map>
using namespace std;

#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef	_DEBUG
	const int TestCont = 100;
	const int TestLast = 1000;
#else /*_DEBUG*/
	const int TestCont = 1000;
	const int TestLast = 10000;
#endif/*_DEBUG*/
	unsigned int tStart = 0, tEnd = 0;

#define TEST_CON(name, init, fst, sec) \
	ExCPrintf(_T(#name)); \
	{ \
		init; \
		timeBeginPeriod(1); \
		tStart = timeGetTime(); \
		for(int i = 0; i < TestCont; ++i) \
		{ \
			for(int j = 0; j < TestLast; ++j) { fst; }\
			sec; \
		} \
		tEnd = timeGetTime(); \
		timeEndPeriod(1); \
	} \
	ExCPrintf(_T("%dms\n"), (tEnd - tStart));
//#define TEST_CON(name, con, fst, sec)

	TEST_CON(\nStart for list...\t\t, list<int> int_list, int_list.push_back(i), int_list.clear())
	TEST_CON(Start for CListT...\t\t, CListT<int> int_list, int_list.Add(i), int_list.Clear())

	TEST_CON(\nStart for vector...\t\t, vector<int> int_list, int_list.push_back(i), int_list.clear())
	TEST_CON(Start for CArrayT...\t\t, CArrayT<int> int_list, int_list.Add(i), int_list.Clear())

	/*
		stl::map�Ǻ����;CMapT�ǹ�ϣ��
		������map������Ӽ����Ч����ʵû�пɱ���
		������ֵ�����ο�
	*/
	typedef map<int, int> map_t;
	TEST_CON(\nStart for map...\t\t, map_t int_list, int_list[i] = i, int_list.clear())
	typedef CMapT<int, int> mapt_t;
	TEST_CON(\nStart for CMapT...\t\t, mapt_t int_list(10007), int_list[i] = i, int_list.Clear())

/*	CString s(_T("������abcd·�߼�EFGH��һ��Ǯ")), t;
	t = s.Left(4);
	s.Upper();
	s.Lower();
	s.TrimLeft(_T('��'));
	s.TrimLeft(_T("������a"));
	s.TrimRight(_T('Ǯ'));
	s.TrimRight(_T("h��һ��Ǯ"));

	s = "������abcd·�߼�EFGH��һ��Ǯ";
	CStringT<char> c(s);

	CArrayT<TCHAR> str(_T("1234")), str2(_T("123"));
	TCHAR* a = NULL, b[MAX_PATH] = {0};*/
	//if (str/* == *//*a*//*b*//*str2*//*_T("123")*/)
	//	int i = 0;

/*	CArrayT<CString> as1;
	as1.Add(_T("123"), as1.Head());
	as1.Add(_T("321"), as1.Head());
	as1.Add(_T("312"), as1.Head());

	for(DWORD i = 0; i < as1.GetCount(); ++i)
		ExCPrintf(_T("%s\n"), (LPCTSTR)as1[i]);

	ExCPrintf(_T("\n"));

	for(CArrayT<CString>::iterator_t ite = as1.Head(); ite != as1.Tail(); ++ite)
		ExCPrintf(_T("%s\n"), (LPCTSTR)(*ite));

	ExCPrintf(_T("\n"));

	/////////////////////////////////

	CListT<CString> as2;
	as2.Add(_T("123"), as2.Head());
	as2.Add(_T("321"), as2.Head());
	as2.Add(_T("312"), as2.Head());

	for(CListT<CString>::iterator_t ite = as2.Head(); ite != as2.Tail(); ++ite)
		ExCPrintf(_T("%s\n"), (LPCTSTR)(*ite));

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
		ExCPrintf(_T("%s <=> %d\n"), (LPCTSTR)ite->Key(), ite->Val());*/

	_tsystem(_T("pause"));
	return 0;
}
