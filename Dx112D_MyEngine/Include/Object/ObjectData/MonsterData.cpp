#include "MonsterData.h"

CMonsterData::CMonsterData()
{
}
CMonsterData::CMonsterData(const CMonsterData& Data)
{
}
CMonsterData::CMonsterData(CMonsterData&& Data)
{
}
CMonsterData::~CMonsterData()
{
}

bool CMonsterData::LoadFromFile(const char* FileName)
{
    return true;
}

bool CMonsterData::SaveToFile(const char* FileName)
{
    return true;
}
