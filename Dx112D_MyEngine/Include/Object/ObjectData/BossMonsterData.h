#pragma once
#include "MonsterData.h"

class CBossMonsterData :
    public CMonsterData
{
public:
	CBossMonsterData();
	CBossMonsterData(const CBossMonsterData& Data);
	CBossMonsterData(CBossMonsterData&& Data);
	virtual ~CBossMonsterData();

public:
	std::string		PatternName;
public:
	virtual bool LoadFromFile(const char* FileName) override;
	virtual bool SaveToFile(const char* FileName) override;
};

