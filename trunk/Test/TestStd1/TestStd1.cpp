// TestStd.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	typedef BYTE A;

#ifdef	_DEBUG
	const int TestCont = 100;
	const int TestLast = 100;
	const int TestSMin = 1;
	const int TestSMax = 10000;
#else /*_DEBUG*/
	const int TestCont = 1000;
	const int TestLast = 1000;
	const int TestSMin = 1;
	const int TestSMax = 100000;
	//const int TestCont = 10000;
	//const int TestLast = 1000;
	//const int TestSMin = 1;
	//const int TestSMax = TestSMin/*100000*/;
#endif/*_DEBUG*/
	unsigned int tStart = 0, tEnd = 0;
	A*	  Test[TestLast] = {0};
	DWORD Size[TestLast] = {0};
	int nMax = TestSMax - TestSMin + 1;

	/////////////////////////////////

	ExCPrintf(_T("循环次数:%d, 分配次数:%d, 分配大小:%dByte-%.1fKB\n"), 
		TestCont, TestLast, TestSMin * sizeof(A), (float)(TestSMax * sizeof(A)) / 1024.0f);
	//ExCPrintf(_T("内存池大小:%.1fMB, 内存池上限:%.1fMB\n\n"), 
	//	(float)CMemAdapterT<_MemPool>::GetAlloc().GetPoolSize() / (float)(1024 * 1024), 
	//	(float)CMemAdapterT<_MemPool>::GetAlloc().GetMaxSize() / (float)(1024 * 1024));

	_tsystem(_T("pause"));

	ExRandomize();
	for(int j = 0; j < TestLast; j++)
		Size[j] = /*TestSMax*/ExRandom(nMax) + TestSMin;

	/////////////////////////////////

#ifdef	EXP_USING_NEW

	ExCPrintf(_T("\nStart for new/delete...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		for(int j = 0; j < TestLast; j++)
			Test[j] = dbnew A[Size[j]];
		for(int j = 0; j < TestLast; j++)
			delete [] (Test[TestLast - 1 - j]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n"), (tEnd - tStart));

	ExCPrintf(_T("Start for gcnew...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		CGC gc;
		for(int j = 0; j < TestLast; j++)
			Test[j] = gcnew(gc, A)[Size[j]];
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n\n"), (tEnd - tStart));

#else /*EXP_USING_NEW*/

	ExCPrintf(_T("\nStart for new/delete...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		for(int j = 0; j < TestLast; j++)
			Test[j] = new A[Size[j]];
		for(int j = 0; j < TestLast; j++)
			delete [] (Test[TestLast - 1 - j]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n"), (tEnd - tStart));

#endif/*EXP_USING_NEW*/

	ExCPrintf(_T("Start for malloc...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		for(int j = 0; j < TestLast; j++)
			Test[j] = (A*)malloc(Size[j] * sizeof(A));
		for(int j = 0; j < TestLast; j++)
			free(Test[TestLast - 1 - j]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n"), (tEnd - tStart));

	ExCPrintf(_T("Start for HeapAlloc...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		for(int j = 0; j < TestLast; j++)
			Test[j] = (A*)CMemHeapAlloc::Alloc(Size[j] * sizeof(A));
		for(int j = 0; j < TestLast; j++)
			CMemHeapAlloc::Free(Test[TestLast - 1 - j]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n\n"), (tEnd - tStart));

	/////////////////////////////////

	ExCPrintf(_T("Start for MemHeap...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		for(int j = 0; j < TestLast; j++)
			Test[j] = CMemAdapterT<_MemHeap>::Alloc<A>(Size[j]);
		for(int j = 0; j < TestLast; j++)
			CMemAdapterT<_MemHeap>::Free(Test[TestLast - 1 - j]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n"), (tEnd - tStart));

	//CMemAdapterT<_MemPool>::GetAlloc().Clear();
	//CMemAdapterT<_MemPool>::GetAlloc().SetPoolSize();

	ExCPrintf(_T("Start for MemPool...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		for(int j = 0; j < TestLast; j++)
			Test[j] = CMemAdapterT<_MemPool>::Alloc<A>(Size[j]);
		for(int j = 0; j < TestLast; j++)
			CMemAdapterT<_MemPool>::Free(Test[TestLast - 1 - j]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n\n"), (tEnd - tStart));

	/////////////////////////////////

	typedef CGCT<_GCPolicyT<CMemAdapterT<_MemHeap> > > CHeapGC;
	typedef CGCT<_GCPolicyT<CMemAdapterT<_MemPool> > > CPoolGC;

	ExCPrintf(_T("Start for GCHeap...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		CHeapGC gc;
		for(int j = 0; j < TestLast; j++)
			CGCAllocT<CHeapGC>::Alloc<A>(&gc, Size[j]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n"), (tEnd - tStart));

	//CMemAdapterT<_MemPool>::GetAlloc().Clear();
	//CMemAdapterT<_MemPool>::GetAlloc().SetPoolSize();

	ExCPrintf(_T("Start for GCPool...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		CPoolGC gc;
		for(int j = 0; j < TestLast; j++)
			CGCAllocT<CPoolGC>::Alloc<A>(&gc, Size[j]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n\n"), (tEnd - tStart));

	/////////////////////////////////

	CObjPoolT<A> pool;

	ExCPrintf(_T("Start for ObjPool...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		for(int j = 0; j < TestLast; j++)
			Test[j] = (A*)pool.Alloc(TestSMax * sizeof(A));
		for(int j = 0; j < TestLast; j++)
			pool.Free(Test[TestLast - 1 - j]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n"), (tEnd - tStart));

	CBlockPool blk_pool(TestSMax * sizeof(A));

	ExCPrintf(_T("Start for BlkPool...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		for(int j = 0; j < TestLast; j++)
			Test[j] = (A*)blk_pool.Alloc();
		for(int j = 0; j < TestLast; j++)
			blk_pool.Free(Test[TestLast - 1 - j]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n\n"), (tEnd - tStart));

	/////////////////////////////////
	// ReAlloc
	ExCPrintf(_T("/////////////////////////////////\n\n"));
	/////////////////////////////////

#ifdef	EXP_USING_NEW

	ExCPrintf(_T("Start for renew...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		Test[0] = renew(NULL, A)[Size[0]];
		for(int j = 1; j < TestLast; j++)
			Test[j] = renew(Test[j - 1], A)[Size[j]];
		delete (Test[TestLast - 1]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n"), (tEnd - tStart));

	ExCPrintf(_T("Start for gcrenew...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		CGC gc;
		Test[0] = gcrenew(gc, NULL, A)[Size[0]];
		for(int j = 1; j < TestLast; j++)
			Test[j] = gcrenew(gc, Test[j - 1], A)[Size[j]];
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n\n"), (tEnd - tStart));

#endif/*EXP_USING_NEW*/

	ExCPrintf(_T("Start for realloc...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		Test[0] = (A*)realloc(NULL, Size[0] * sizeof(A));
		for(int j = 1; j < TestLast; j++)
			Test[j] = (A*)realloc(Test[j - 1], Size[j] * sizeof(A));
		free(Test[TestLast - 1]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n"), (tEnd - tStart));

	ExCPrintf(_T("Start for HeapReAlloc...\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		Test[0] = (A*)CMemHeapAlloc::ReAlloc(NULL, Size[0] * sizeof(A));
		for(int j = 1; j < TestLast; j++)
			Test[j] = (A*)CMemHeapAlloc::ReAlloc(Test[j - 1], Size[j] * sizeof(A));
		CMemHeapAlloc::Free(Test[TestLast - 1]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n\n"), (tEnd - tStart));

	/////////////////////////////////

	ExCPrintf(_T("Start for MemHeap...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		Test[0] = CMemAdapterT<_MemHeap>::ReAlloc<A>(NULL, Size[0]);
		for(int j = 1; j < TestLast; j++)
			Test[j] = CMemAdapterT<_MemHeap>::ReAlloc<A>(Test[j - 1], Size[j]);
		CMemAdapterT<_MemHeap>::Free(Test[TestLast - 1]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n"), (tEnd - tStart));

	//CMemAdapterT<_MemPool>::GetAlloc().Clear();
	//CMemAdapterT<_MemPool>::GetAlloc().SetPoolSize();

	ExCPrintf(_T("Start for MemPool...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		Test[0] = CMemAdapterT<_MemPool>::ReAlloc<A>(NULL, Size[0]);
		for(int j = 1; j < TestLast; j++)
			Test[j] = CMemAdapterT<_MemPool>::ReAlloc<A>(Test[j - 1], Size[j]);
		CMemAdapterT<_MemPool>::Free(Test[TestLast - 1]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n\n"), (tEnd - tStart));

	/////////////////////////////////

	ExCPrintf(_T("Start for GCHeap...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		CHeapGC gc;
		Test[0] = CGCAllocT<CHeapGC>::ReAlloc<A>(&gc, NULL, Size[0]);
		for(int j = 1; j < TestLast; j++)
			Test[j] = CGCAllocT<CHeapGC>::ReAlloc<A>(&gc, Test[j - 1], Size[j]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n"), (tEnd - tStart));

	//CMemAdapterT<_MemPool>::GetAlloc().Clear();
	//CMemAdapterT<_MemPool>::GetAlloc().SetPoolSize();

	ExCPrintf(_T("Start for GCPool...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		CPoolGC gc;
		Test[0] = CGCAllocT<CPoolGC>::ReAlloc<A>(&gc, NULL, Size[0]);
		for(int j = 1; j < TestLast; j++)
			Test[j] = CGCAllocT<CPoolGC>::ReAlloc<A>(&gc, Test[j - 1], Size[j]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n\n"), (tEnd - tStart));

	/////////////////////////////////

	_tsystem(_T("pause"));
	return 0;
}
