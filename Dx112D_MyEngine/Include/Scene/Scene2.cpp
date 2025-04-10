#include "Scene2.h"
#include "../Object/PlayerObject.h"
#include "../Object/MageMonster.h"
#include "../Object/NearingMonster.h"
#include "../Object/ObjectSpawnPoint.h"
#include "SceneAssetManager.h"
#include "../Asset/Sound/SoundManager.h"
#include "../Asset/Material/Material.h"
#include "../UI/UserWidget/MainWidget.h"
#include "SceneUIManager.h"
#include "../Object/TileMapObj.h"
#include "../Object/CameraObject.h"
#include "../Object/NPCObject.h"
#include "../Object/Portal.h"
#include "../PlayerManager.h"

CScene2::CScene2()
{
}

CScene2::~CScene2()
{
    GetAssetManager()->FindSound("Scene2BGM")->Stop();
}

bool CScene2::InitAsset()
{
    GetAssetManager()->FindSound("Scene2BGM")->Play();
    return true;
}

bool CScene2::InitObject()
{
    mMapObj = CreateObj<CTileMapObj>("Map2");
    LoadMap();

    mMapObj->SetBackTexture("Map2Back",
        TEXT("Texture\\Map\\Map2BackGround.png"));

    mCameraObj = CreateObj<CCameraObject>("CameraObj");
    mCameraObj->SetTileMap(mMapObj);

    CPlayerObject* Player =
        CreateObj<CPlayerObject>("Player");
    Player->SetTileMap(mMapObj);
    Player->GetData()->SetCurrentScene(ESceneType::Scene2);
    Player->InitAfter();

    mCameraObj->SetTarget(Player);

    CPortal* Portal =
        CreateObj<CPortal>("ToScene1");
    Portal->SetWorldPos(100.f, 2596.f);
    Portal->SetTileMap(mMapObj);
    Portal->SetTargetScene(ESceneType::Scene1);
    Portal->SetTargetPos(1350.f, 3500.f);

    Portal =
        CreateObj<CPortal>("ToSceneBoss");
    Portal->SetWorldPos(1000.f, 1500.f);
    Portal->SetTileMap(mMapObj);
    Portal->SetTargetScene(ESceneType::SceneBoss);
    Portal->SetTargetPos(2050.f, 200.f);

    CObjectSpawnPoint* MonsterPoint =
        CreateObj<CObjectSpawnPoint>("NearingMonster1");

    MonsterPoint->SetSpawnType(EObjectSpawnType::NearingMonster);
    MonsterPoint->SetSpawnLoopType(EObjectSpawnLoopType::Once);
    MonsterPoint->SetSpawnCountType(EObjectSpawnCountType::Loop);
    MonsterPoint->SetSpawnTime(3.f);
    MonsterPoint->SetImmediateSpawn(true);
    MonsterPoint->SetWorldPos(400.f, 300.f);

    MonsterPoint =
        CreateObj<CObjectSpawnPoint>("NearingMonster2");
    MonsterPoint->SetSpawnType(EObjectSpawnType::NearingMonster);
    MonsterPoint->SetSpawnLoopType(EObjectSpawnLoopType::Once);
    MonsterPoint->SetSpawnCountType(EObjectSpawnCountType::Loop);
    MonsterPoint->SetSpawnTime(3.f);
    MonsterPoint->SetImmediateSpawn(true);
    MonsterPoint->SetWorldPos(800.f, 300.f);

    MonsterPoint =
        CreateObj<CObjectSpawnPoint>("MageMonster1");
    MonsterPoint->SetSpawnType(EObjectSpawnType::MageMonster);
    MonsterPoint->SetSpawnLoopType(EObjectSpawnLoopType::Once);
    MonsterPoint->SetSpawnCountType(EObjectSpawnCountType::Loop);
    MonsterPoint->SetSpawnTime(3.f);
    MonsterPoint->SetImmediateSpawn(true);
    MonsterPoint->SetWorldPos(400.f, 1200.f);
    MonsterPoint =
        CreateObj<CObjectSpawnPoint>("MageMonster2");

    MonsterPoint->SetSpawnType(EObjectSpawnType::MageMonster);
    MonsterPoint->SetSpawnLoopType(EObjectSpawnLoopType::Once);
    MonsterPoint->SetSpawnCountType(EObjectSpawnCountType::Loop);
    MonsterPoint->SetSpawnTime(3.f);
    MonsterPoint->SetImmediateSpawn(true);
    MonsterPoint->SetWorldPos(800.f, 1200.f);

    return true;
}

bool CScene2::InitWidget()
{
    mMainWidget = mUIManager->CreateWidget<CMainWidget>("Main");
    mUIManager->AddToViewport(mMainWidget);

    return true;
}

void CScene2::LoadMap()
{
    TCHAR	FullPath[MAX_PATH] = {};

    lstrcpy(FullPath, gRootPath);
    lstrcat(FullPath, TEXT("Asset\\Data\\Scene2.tlm"));

    mMapObj->Load(FullPath);
}
