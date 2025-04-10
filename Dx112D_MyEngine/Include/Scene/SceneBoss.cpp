#include "SceneBoss.h"
#include "../Object/PlayerObject.h"
#include "../Component/PlayerMovement.h"
#include "../Object/NueBossMonster.h"
#include "../Object/ObjectSpawnPoint.h"
#include "SceneAssetManager.h"
#include "../Asset/Material/Material.h"
#include "../Asset/Sound/SoundManager.h"
#include "../UI/UserWidget/MainWidget.h"
#include "SceneUIManager.h"
#include "../Object/TileMapObj.h"
#include "../Object/CameraObject.h"
#include "../Object/NPCObject.h"
#include "../Object/Portal.h"
#include "../PlayerManager.h"

CSceneBoss::CSceneBoss()
{
}

CSceneBoss::~CSceneBoss()
{

    GetAssetManager()->FindSound("SceneBossBGM")->Stop();
}

bool CSceneBoss::InitAsset()
{
    return true;
}

bool CSceneBoss::InitObject()
{
    mMapObj = CreateObj<CTileMapObj>("Map3");
    LoadMap();

    mMapObj->SetBackTexture("Map3Back",
        TEXT("Texture\\Map\\Map3BackGround.png"));

    mCameraObj = CreateObj<CCameraObject>("CameraObj");
    mCameraObj->SetTileMap(mMapObj);

    mPlayer = CreateObj<CPlayerObject>("Player");
    mPlayer->SetTileMap(mMapObj);
    mPlayer->GetData()->SetCurrentScene(ESceneType::SceneBoss);
    mPlayer->InitAfter();

    mCameraObj->SetTarget(mPlayer);

    CPortal* Portal =
        CreateObj<CPortal>("ToScene1");
    Portal->SetWorldPos(2050.f, 50.f);
    Portal->SetPortalTexture("BossPortal", TEXT("Texture\\Map\\MapObject\\Null.png"));
    Portal->SetPortalSize(500.f, 100.f);
    Portal->SetTileMap(mMapObj);
    Portal->SetTargetScene(ESceneType::Scene2);
    Portal->SetTargetPos(1000.f, 1350.f);
    return true;
}

bool CSceneBoss::InitWidget()
{
    mMainWidget = mUIManager->CreateWidget<CMainWidget>("Main");
    mUIManager->AddToViewport(mMainWidget);

    return true;
}

void CSceneBoss::Update(float DeltaTime)
{
    CScene::Update(DeltaTime);

    // 플레이어가 특정 좌표에 도달하고
    // 보스가 아직 생성되지 않았다면
    if (mPlayer->GetWorldPosition().y >= 1300.f && !mCreate)
    {
        // 보스 생성
        mBoss = CreateObj<CNueBossMonster>("NueBossMonster");
        mBoss->SetTileMap(mMapObj);
        mBoss->SetWorldPos(2050.f, 2500.f);
        mBoss->SetTarget(mPlayer);

        GetAssetManager()->FindSound("SceneBossBGM")->Play();

        // 컷씬을 준비한다.
        mCreate = true;
        mCutScene = true;
        mCutSceneTimer = 3.f;
    }

    // 만약 컷씬을 진행해야 한다면
    if (mCutScene)
    {
        // 카메라 타겟을 보스로 바꾼다.
        mCameraObj->SetTarget(mBoss);
        // 플레이어 움직임 제한.
        mPlayer->GetMovement()->SetEnable(false);

        
        // 컷씬 시간이 다됐다면 
        // 카메라 타겟을 다시 플레이어로 설정한다.
        mCutSceneTimer -= DeltaTime;
        if (mCutSceneTimer <= 0.f)
        {
            mCameraObj->SetTarget(mPlayer);
            mPlayer->GetMovement()->SetEnable(true);
            mMainWidget->SetEnableBossUI(true);
            mCutScene = false;
        }
    }

   
}

void CSceneBoss::LoadMap()
{
    TCHAR	FullPath[MAX_PATH] = {};

    lstrcpy(FullPath, gRootPath);
    lstrcat(FullPath, TEXT("Asset\\Data\\Scene3.tlm"));

    mMapObj->Load(FullPath);
}
