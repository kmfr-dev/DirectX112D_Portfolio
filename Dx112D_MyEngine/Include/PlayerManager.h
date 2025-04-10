#pragma once

#include "GameInfo.h"

class CPlayerManager
{
private:
	class CPlayerData* mPlayerData = nullptr;

public:
	bool	Init();
	bool	Save();
	bool	Load();

public:
	class CPlayerData* GetPlayerData() const { return mPlayerData; }
	void SetData(class CPlayerData* Data) { mPlayerData = Data; }


	DECLARE_SINGLE(CPlayerManager)
};

