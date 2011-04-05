// TestStd5.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//////////////////////////////////////////////////////////////////

class CTest1 : public CBaseObject
{
	EXP_DECLARE_DYNCREATE_CLS(CTest1, CBaseObject)

protected:
	int i;
};

EXP_IMPLEMENT_DYNCREATE_CLS(CTest1, CBaseObject)

//////////////////////////////////////////////////////////////////

class CTest2 : public CTest1
{
	EXP_DECLARE_DYNCREATE_CLS(CTest2, CTest1)

protected:
	char c;
};

EXP_IMPLEMENT_DYNCREATE_CLS(CTest2, CTest1)

//////////////////////////////////////////////////////////////////

template <typename TestT>
class CTest3 : public TestT
{
	EXP_DECLARE_DYNCREATE_CLS(CTest3, TestT)

protected:
	CString s;
};

EXP_IMPLEMENT_DYNCREATE_CLS(CTest3<TestT>, TestT, template <typename TestT>)

//////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
	CGC gc;
	CTest2* pt2 = ExDynCreate<CTest2>(_T("CTest2"), &gc);
	CTest1* pt1 = ExDynCast<CTest1>(pt2);
	CTest3<CTest1>* pt3_1 = ExDynCreate<CTest3<CTest1> >(_T("CTest3<TestT>"), &gc); // Fail, 对template的支持暂时不是很理想
	return 0;
}
