#include "ThreadBase.h"

CThreadBase::CThreadBase()
{
}

CThreadBase::~CThreadBase()
{
	if (mThread)
	{
		WaitForSingleObject(mThread, INFINITE);

		// Thread 강제 종료.
		// TerminateThread(mThread, 0);

		CloseHandle(mThread);
	}
}

void CThreadBase::Init(bool Pause)
{
	unsigned int InitFlag = 0;

	if (Pause)
		InitFlag = CREATE_SUSPENDED;
	
	mThread = (HANDLE)_beginthreadex(nullptr, 0,
		CThreadBase::ThreadFunc, (void*)this,
		InitFlag, nullptr);
}

void CThreadBase::Suspend()
{
	SuspendThread(mThread);
}

void CThreadBase::Resume()
{
	ResumeThread(mThread);
}

unsigned int __stdcall CThreadBase::ThreadFunc(void* Arg)
{
	CThreadBase* Thread = (CThreadBase*)Arg;

	Thread->Run();

	return 0;
}
