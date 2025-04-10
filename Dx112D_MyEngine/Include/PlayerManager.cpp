#include "PlayerManager.h"
#include "Object/PlayerObject.h"
#include "Object\ObjectData\PlayerData.h"

DEFINITION_SINGLE(CPlayerManager);

CPlayerManager::CPlayerManager()
{

}

CPlayerManager::~CPlayerManager()
{
    SAFE_DELETE(mPlayerData);
}

bool CPlayerManager::Init()
{
    mPlayerData = new CPlayerData;

    return true;
}

bool CPlayerManager::Save()
{
    return mPlayerData->SaveToFile("PlayerData01.sav");
}

bool CPlayerManager::Load()
{
    if (mPlayerData->LoadFromFile("PlayerData01.sav"))
        return true;

    else
        Save();

    return true;
}
