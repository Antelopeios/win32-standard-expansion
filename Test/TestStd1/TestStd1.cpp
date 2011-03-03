// TestStd.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

int _tmain(int argc, _TCHAR* argv[])
{
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
#endif/*_DEBUG*/
	unsigned int tStart = 0, tEnd = 0;
	void* Test[TestLast];

	/////////////////////////////////

	ExCPrintf(_T("循环次数:%d, 分配次数:%d, 分配大小:%dB-%.1fKB\n"), 
		TestCont, TestLast, TestSMin, (float)TestSMax / 1024.0f);
	ExCPrintf(_T("内存池大小:%.1fMB, 内存池上限:%.1fMB\n\n"), 
		(float)CMemAdapterT<_MemPool>::GetAlloc().GetPoolSize() / (float)(1024 * 1024), 
		(float)CMemAdapterT<_MemPool>::GetAlloc().GetMaxSize() / (float)(1024 * 1024));

	_tsystem(_T("pause"));

	ExRandomize();

	/////////////////////////////////

	ExCPrintf(_T("\nStart for new/delete...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		for(int j = 0; j < TestLast; j++)
			Test[j] = new BYTE[ExRandom(TestSMax) + TestSMin];
		for(int j = 0; j < TestLast; j++)
			delete [] (Test[TestLast - 1 - j]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n"), (tEnd - tStart));

	ExCPrintf(_T("Start for malloc...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		for(int j = 0; j < TestLast; j++)
			Test[j] = malloc(ExRandom(TestSMax) + TestSMin);
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
			Test[j] = CMemHeapAlloc::Alloc(ExRandom(TestSMax) + TestSMin);
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
			Test[j] = CMemAdapterT<_MemHeap>::Alloc(ExRandom(TestSMax) + TestSMin);
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
			Test[j] = CMemAdapterT<_MemPool>::Alloc(ExRandom(TestSMax) + TestSMin);
		for(int j = 0; j < TestLast; j++)
			CMemAdapterT<_MemPool>::Free(Test[j]);
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
			CGCAllocT<CHeapGC>::Alloc<BYTE>(gc, ExRandom(TestSMax) + TestSMin);
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
			CGCAllocT<CPoolGC>::Alloc<BYTE>(gc, ExRandom(TestSMax) + TestSMin);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n\n"), (tEnd - tStart));

	/////////////////////////////////

	CObjPoolT<BYTE[TestSMax], _ObjPoolPolicyT<CMemAdapterT<_MemPool> > > pool;

	ExCPrintf(_T("Start for ObjPool...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		for(int j = 0; j < TestLast; j++)
			Test[j] = pool.Alloc();
		for(int j = 0; j < TestLast; j++)
			pool.Free(Test[TestLast - 1 - j]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n\n"), (tEnd - tStart));

	/////////////////////////////////

	_tsystem(_T("pause"));
	return 0;
}
