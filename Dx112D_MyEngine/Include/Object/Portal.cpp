#include "Portal.h"
#include "../Component/ColliderAABB2D.h"
#include "../Component/SpriteComponent.h"
#include "../Component/InventoryComponent.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneMain.h"
#include "../Scene/Scene2.h"
#include "../Scene/SceneBoss.h"

#include "PlayerObject.h"
#include "../PlayerManager.h"

CPortal::CPortal()
{
}
CPortal::CPortal(const CPortal& Obj)
{
}
CPortal::CPortal(CPortal&& Obj)
{
}
CPortal::~CPortal()
{
}

bool CPortal::Init()
{
    CSceneObject::Init();

    mRoot = CreateComponent<CSpriteComponent>();
    mRoot->SetWorldScale(100.f, 100.f);
    mRoot->SetTexture("Portal",TEXT("Texture\\Map\\MapObject\\TestWall.png"));
    mRoot->SetPivot(0.5f, 0.5f);
    SetRootComponent(mRoot);

    mBody = CreateComponent<CColliderAABB2D>();
    mBody->SetCollisionProfile("Portal");
    mBody->SetWorldScale(100.f, 100.f);
    mBody->SetBoxSize(100.f, 100.f);
    mBody->SetCollisionBeginFunc<CPortal>(this,
        &CPortal::CollisionPortal);
    mRoot->AddChild(mBody);


    return true;
}

void CPortal::Update(float DeltaTime)
{
    CSceneObject::Update(DeltaTime);
}

void CPortal::Render()
{
    CSceneObject::Render();
}

void CPortal::SetPortalTexture(const std::string& Name, 
    const TCHAR* FileName)
{
    mRoot->SetTexture(Name, FileName);
}

void CPortal::SetPortalSize(const FVector2D& Size)
{
    mBody->SetBoxSize(Size.x, Size.y);
}

void CPortal::SetPortalSize(float x, float y)
{
    mBody->SetBoxSize(x, y);
}

void CPortal::CollisionPortal(
    const FVector3D& HitPoint, CColliderBase* Dest)
{
    // 포탈에 부딪힌 대상이 플레이어가 아니면 충돌 수행X
    if (!Dest->GetProfile() ||
        Dest->GetProfile()->Channel != ECollisionChannel::Player)
        return;
   
    // 다음 씬에서 플레이가 스폰될 위치 저장
    CPlayerManager::GetInst()->GetPlayerData()->
        SetNextSpawnPos(mTargetPos);
    CPlayerManager::GetInst()->GetPlayerData()->
        SetGameType(EGameType::MoveNextScene);

    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(Dest->GetOwner());
    if (!Player)
        return;

    CPlayerManager::GetInst()->GetPlayerData()->
        SetInventoryItems(Player->GetInventory()->GetItemList());
    CPlayerManager::GetInst()->Save();

    switch (mTargetScene)
    {
    case ESceneType::Scene1:
        CSceneManager::GetInst()->CreateLoadScene<CSceneMain>();
        break;
    case ESceneType::Scene2:
        CSceneManager::GetInst()->CreateLoadScene<CScene2>();
        break;
    case ESceneType::SceneBoss:
        CSceneManager::GetInst()->CreateLoadScene<CSceneBoss>();
        break;
    }
}