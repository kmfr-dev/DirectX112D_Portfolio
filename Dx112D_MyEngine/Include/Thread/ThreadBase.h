#pragma once

#include "../GameInfo.h"
#include <process.h>

class CThreadBase abstract
{
	friend class CThreadManager;

protected:
	CThreadBase();
	virtual ~CThreadBase();

protected:
	HANDLE	mThread = 0;

public:
	void Init(bool Pause = false);
	virtual void Run() = 0;

public:
	void Suspend();
	void Resume();

private:
	static unsigned int __stdcall ThreadFunc(void* Arg);
};

