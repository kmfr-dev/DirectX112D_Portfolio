#pragma once

#include "../GameInfo.h"

class CTimer
{
	friend class CGameManager;

private:
	static LARGE_INTEGER	mSecond;
	static LARGE_INTEGER	mTime;
	static float	mDeltaTime;
	//TEST
	//static float mTestTime;
	//TEST

	static float	mFPS;
	static float	mFPSTime;
	static int		mFPSTick;

private:
	static void Init();
	static float Update();

public:
	static float GetDeltaTime();
	static float GetFPS();
};

