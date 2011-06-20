// TestStd5.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//////////////////////////////////////////////////////////////////

class CTest1 : public IBaseObject
{
	EXP_DECLARE_DYNCREATE_CLS(CTest1, IBaseObject)

protected:
	int i;
};

EXP_IMPLEMENT_DYNCREATE_CLS(CTest1, IBaseObject)

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
	CTest2* pt2 = ExDynCast<CTest2>(ExDynCreate(_T("CTest2"), &gc));
	CTest1* pt1 = ExDynCast<CTest1>(pt2);
	CTest3<CTest1>* pt3_1 = ExDynCast<CTest3<CTest1> >(ExDynCreate(_T("CTest3<TestT>"), &gc)); // Fail, ��template��֧����ʱ���Ǻ�����
	return 0;
}