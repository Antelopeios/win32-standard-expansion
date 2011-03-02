// TestStd.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	const int TestCont = 100;
	const int TestLast = 100;
	const int TestSize = 10000;
#else /*_DEBUG*/
	const int TestCont = 1000;
	const int TestLast = 1000;
	const int TestSize = 100000;
#endif/*_DEBUG*/
	unsigned int tStart = 0, tEnd = 0;
	void* Test[TestLast];

	//for(int j = 0; j < TestLast; j++)
	//	Test[j] = ExMemAlloc<CEvent, _MemPool>(TestSize);
	//for(int j = 0; j < TestLast; j++)
	//	ExMemFree<_MemPool>(Test[TestLast - 1 - j]);

	/////////////////////////////////

	ExCPrintf(_T("循环次数:%d, 分配次数:%d, 分配大小:%.1fKB\n"), 
		TestCont, TestLast, (float)TestSize / 1024.0f);
	ExCPrintf(_T("内存池大小:%.1fMB, 内存池上限:%.1fMB\n\n"), 
		(float)CMemAdapterT<_MemPool>::GetAlloc().GetPoolSize() / (float)(1024 * 1024), 
		(float)CMemAdapterT<_MemPool>::GetAlloc().GetMaxSize() / (float)(1024 * 1024));

	_tsystem(_T("pause"));

	/////////////////////////////////

	ExCPrintf(_T("\nStart for new/delete...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		for(int j = 0; j < TestLast; j++)
			Test[j] = new BYTE[TestSize];
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
			Test[j] = malloc(TestSize);
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
			Test[j] = CMemHeapAlloc::Alloc(TestSize);
		for(int j = 0; j < TestLast; j++)
			CMemHeapAlloc::Free(Test[TestLast - 1 - j]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n\n"), (tEnd - tStart));

	/////////////////////////////////

	ExCPrintf(_T("Start for _MemHeap...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		for(int j = 0; j < TestLast; j++)
			Test[j] = CMemAdapterT<_MemHeap>::Alloc(TestSize);
		for(int j = 0; j < TestLast; j++)
			CMemAdapterT<_MemHeap>::Free(Test[TestLast - 1 - j]);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n"), (tEnd - tStart));

	//CMemAdapterT<_MemPool>::GetAlloc().Clear();
	//CMemAdapterT<_MemPool>::GetAlloc().SetPoolSize();

	ExCPrintf(_T("Start for _MemPool...\t\t"));
	timeBeginPeriod(1);
	tStart = timeGetTime();
	for(int i = 0; i < TestCont; i++)
	{
		for(int j = 0; j < TestLast; j++)
			Test[j] = CMemAdapterT<_MemPool>::Alloc(TestSize);
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
			CGCAllocT<CHeapGC>::Alloc<BYTE>(gc, TestSize);
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
			CGCAllocT<CPoolGC>::Alloc<BYTE>(gc, TestSize);
	}
	tEnd = timeGetTime();
	timeEndPeriod(1);
	ExCPrintf(_T("%dms\n\n"), (tEnd - tStart));

	/////////////////////////////////

	CObjPoolT<BYTE[TestSize]> pool;

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
