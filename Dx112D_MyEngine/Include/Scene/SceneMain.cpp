#include "SceneMain.h"
#include "../Asset/Sound/SoundManager.h"
#include "../Object/PlayerObject.h"
#include "../Object/MageMonster.h"
#include "../Object/NearingMonster.h"
#include "../Object/ObjectSpawnPoint.h"
#include "../Component/StaticMeshComponent.h"
#include "SceneAssetManager.h"
#include "../Asset/Material/Material.h"
#include "../UI/UserWidget/MainWidget.h"
#include "SceneUIManager.h"
#include "../Object/TileMapObj.h"
#include "../Object/CameraObject.h"
#include "../Object/OguNPC.h"
#include "../Object/ShopNPC.h"
#include "../Object/Portal.h"
#include "../PlayerManager.h"


CSceneMain::CSceneMain()
{
}

CSceneMain::~CSceneMain()
{
    // 2번쨰 씬을 만들고 첫번쨰 씬을 지운다
    // 그다음 플레이어를 지운다.
    // 근데 지울 때 아이템 정보를 저장함
    GetAssetManager()->FindSound("Scene1BGM")->Stop();
}

bool CSceneMain::InitAsset()
{
    
    GetAssetManager()->FindSound("Scene1BGM")->Play();

    return true;
}

bool CSceneMain::InitObject()
{
    mMapObj = CreateObj<CTileMapObj>("Map1");

    mMapObj->SetBackTexture("Map1Back",
        TEXT("Texture\\Map\\Map1BackGround.png"));

    mCameraObj = CreateObj<CCameraObject>("CameraObj");
    mCameraObj->SetTileMap(mMapObj);

    CPlayerObject* Player =
        CreateObj<CPlayerObject>("Player");
    Player->SetTileMap(mMapObj);
    Player->GetData()->SetCurrentScene(ESceneType::Scene1);
    Player->InitAfter();

    CPortal* Portal = CreateObj<CPortal>("ToScene2");
    Portal->SetTileMap(mMapObj);
    Portal->SetWorldPos(1500.f, 3500.f);
    Portal->SetTargetScene(ESceneType::Scene2);
    Portal->SetTargetPos(250.f, 2596.f);

    mCameraObj->SetTarget(Player);
    
    COguNPC* OguNPC = CreateObj<COguNPC>("OguNPC");
    OguNPC->SetTileMap(mMapObj);
    OguNPC->SetWorldPos(700.f, 3796.f);

    CShopNPC* ShopNPC = CreateObj<CShopNPC>("ShopNPC");
    ShopNPC->SetTileMap(mMapObj);
    ShopNPC->SetWorldPos(700.f, 3496.f);

    return true;
}

bool CSceneMain::InitWidget()
{
    mMainWidget = mUIManager->CreateWidget<CMainWidget>("Main");
    mUIManager->AddToViewport(mMainWidget);

    return true;
}

void CSceneMain::LoadMapObjects()
{
   
}

void CSceneMain::LoadMap()
{
    TCHAR	FullPath[MAX_PATH] = {};

    lstrcpy(FullPath, gRootPath);
    lstrcat(FullPath, TEXT("Asset\\Data\\TileMap.tlm"));

    mMapObj->Load(FullPath);
}
