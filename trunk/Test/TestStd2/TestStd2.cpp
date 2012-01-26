// TestStd2.cpp : 定义控制台应用程序的入口点。
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
	const int TestMapS = 1009;
#else /*_DEBUG*/
	const int TestCont = 1000;
	const int TestLast = 10000;
	const int TestMapS = 10007;
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
			for(int n = 0; n < TestLast; ++n) { fst; }\
			sec; \
		} \
		tEnd = timeGetTime(); \
		timeEndPeriod(1); \
	} \
	ExCPrintf(_T("%dms\n"), (tEnd - tStart));
//#define TEST_CON(name, con, fst, sec)

	TEST_CON(\nStart for list...\t\t, list<int> int_list, int_list.push_back(n), int_list.clear())
	TEST_CON(Start for CList...\t\t, CList<int> int_list, int_list.AddTail(n), int_list.RemoveAll())
	TEST_CON(Start for CListT...\t\t, CListT<int> int_list, int_list.Add(n), int_list.Clear())

	TEST_CON(\nStart for vector...\t\t, vector<int> int_list, int_list.push_back(n), int_list.clear())
	TEST_CON(Start for CArray...\t\t, CArray<int> int_list, int_list.Add(n), int_list.RemoveAll())
	TEST_CON(Start for CArrayT...\t\t, CArrayT<int> int_list, int_list.Add(n), int_list.Clear())

	typedef map<int, int> map_t;
	typedef CMap<int, int, int, int> mapm_t;
	typedef CMapT<int, int, _MapPolicyT<TestMapS> > mapt_t;
	/*
		stl::map是红黑树;CMapT与CMap是哈希表
		这两个map测试添加及清空效率其实没有可比性
		以下数值仅供参考
	*/
	TEST_CON(\nStart for map...\t\t, map_t int_list, int_list[n] = n, int_list.clear())
	TEST_CON(Start for CMap...\t\t, mapm_t int_list; int_list.InitHashTable(TestMapS), int_list[n] = n, int_list.RemoveAll())
	TEST_CON(Start for CMapT...\t\t, mapt_t int_list, int_list[n] = n, int_list.Clear())

	// 随机访问测试
	int acc[TestLast] = {0};
	ExRandomize();
	for(int i = 0; i < TestLast; ++i)
		acc[i] = ExRandom(TestLast);
	map_t map1; mapm_t map2; mapt_t map3;
	map2.InitHashTable(TestMapS);
	for(int i = 0; i < TestLast; ++i)
	{
		map1[i] = i;
		map2[i] = i;
		map3[i] = i;
	}
	TEST_CON(\nRandom access map...\t\t, , map1.find(n))
	TEST_CON(Random access CMap...\t\t, , map2.PLookup(n))
	TEST_CON(Random access CMapT...\t\t, , map3.Locate(n))

/*	CString s(_T("我在马abcd路边捡EFGH到一分钱")), t;
	t = s.Left(4);
	s.Upper();
	s.Lower();
	s.TrimLeft(_T('我'));
	s.TrimLeft(_T("我在马a"));
	s.TrimRight(_T('钱'));
	s.TrimRight(_T("h到一分钱"));

	s = "我在马abcd路边捡EFGH到一分钱";
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

	CMapT<EXP::CString, DWORD> as3;
	as3.Add(_T("1"), 1);
	ExCPrintf(_T("%d\n"), as3[_T("1")]);
	as3[_T("2")] = 2;
	ExCPrintf(_T("%d%d\n"), as3[_T("1")], as3[_T("2")]);
	as3.Add(_T("3"), 3);
	ExCPrintf(_T("%d%d%d\n"), as3[_T("1")], as3[_T("2")], as3[_T("3")]);
	as3.Del(_T("3"));

	for(CMapT<EXP::CString, DWORD>::iterator_t ite = as3.Head(); ite != as3.Tail(); ++ite)
		ExCPrintf(_T("%s <=> %d\n"), (LPCTSTR)ite->Key(), ite->Val());*/

	_tsystem(_T("pause"));
	return 0;
}
