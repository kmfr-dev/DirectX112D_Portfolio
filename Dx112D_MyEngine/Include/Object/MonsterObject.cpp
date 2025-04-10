#include "MonsterObject.h"
#include "../Component/StateMachineComponent.h"
#include "../Scene/Scene.h"
#include "../Component/ColliderAABB2D.h"
#include "../Component/ColliderOBB2D.h"
#include "../Component/ColliderSphere2D.h"
#include "../Share/Log.h"
#include "../Component/SpriteComponent.h"
#include "../Component/MonsterMovement.h"
#include "../Component/WidgetComponent.h"

#include "../Scene/SceneUIManager.h"
#include "TileMapObj.h"
#include "PlayerObject.h"



CMonsterObject::CMonsterObject()
{
}
CMonsterObject::CMonsterObject(const CMonsterObject& Obj)   :
    CSceneObject(Obj)
{
}
CMonsterObject::CMonsterObject(CMonsterObject&& Obj) :
    CSceneObject(Obj)
{
}
CMonsterObject::~CMonsterObject()
{
}

CColliderBase* CMonsterObject::GetCollider() const
{
    return mBody;
}

CMonsterMovement* CMonsterObject::GetMovement() const
{
    return mMovement;
}


FVector2D CMonsterObject::FindRandomTargetTile() const
{
    CTileMapComponent* TileMap = mMap->GetTileMap();

    while (true)
    {
        int x = rand() % TileMap->GetTileCountX();
        int y = rand() % TileMap->GetTileCountY();

        int index = y * TileMap->GetTileCountX() + x;
        const CTile* tile = TileMap->GetTile(index);

        // 타일 중앙 좌표
        if (tile && tile->GetType() == ETileType::Normal)
            return tile->GetCenter(); 

        return FVector2D(0, 0);
    }
}

bool CMonsterObject::Init()
{
    mRoot = CreateComponent<CSpriteComponent>();

    SetRootComponent(mRoot);

    mBody = CreateComponent<CColliderSphere2D>();

    mBody->SetCollisionProfile("Monster");

    mBody->SetCollisionBeginFunc<CMonsterObject>(this,
        &CMonsterObject::CollisionMonster);
    mBody->SetCollisionEndFunc<CMonsterObject>(this,
        &CMonsterObject::CollisionMonsterEnd);

    mRoot->AddChild(mBody);

    mDetect = CreateComponent<CColliderSphere2D>();
    mDetect->SetCollisionProfile("MonsterDetect");

    mRoot->AddChild(mDetect);

    mHPWidget = CreateComponent<CWidgetComponent>("MonsterHPWidget");
    mRoot->AddChild(mHPWidget);

    mStateMachine = CreateComponent<CStateMachineComponent>();
    mMovement = CreateComponent<CMonsterMovement>();

    

    mObjectType = EObjectType::Monster;

    return true;
}

void CMonsterObject::Update(float DeltaTime)
{
    CSceneObject::Update(DeltaTime);

    
   // ConstrainToMap();

}

void CMonsterObject::ConstrainToMap()
{
    FVector2D Pos;
    Pos.x = mRootComponent->GetWorldPosition().x;
    Pos.y = mRootComponent->GetWorldPosition().y;

    // X축 제한 (왼쪽 & 오른쪽 벽)
    // 최대 X 경계를 벗어나면 고정
    if (Pos.x > mMap->GetPlayerMaxBounds().x)
        Pos.x = mMap->GetPlayerMaxBounds().x;

    // 최소 X 경계를 벗어나면 고정
    else if (Pos.x < mMap->GetPlayerMinBounds().x)
        Pos.x = mMap->GetPlayerMinBounds().x;

    // Y축 제한 (위 & 아래 벽)
    // 최대 Y 경계를 벗어나면 고정
    if (Pos.y > mMap->GetPlayerMaxBounds().y)
        Pos.y = mMap->GetPlayerMaxBounds().y;
    // 최소 Y 경계를 벗어나면 고정
    else if (Pos.y < mMap->GetPlayerMinBounds().y)
        Pos.y = mMap->GetPlayerMinBounds().y;

    mRootComponent->SetWorldPos(Pos);
}

void CMonsterObject::ChangeAnimation(const std::string Name)
{
    mAnimation->ChangeAnimation(Name);
}


float CMonsterObject::Damage(float Attack,
    CSceneObject* Obj)
{
    float Dmg = CSceneObject::Damage(Attack, Obj);

    mHP -= (int)Dmg;


    CLog::Debug("Monster Name : ", mName);
    if (mHP <= 0)
    {
        mDead = true;
    }

    return Dmg;
}

void CMonsterObject::CollisionMonster(
    const FVector3D& HitPoint, CColliderBase* Dest)
{
    /*FCollisionProfile* DestProfile = 
        dynamic_cast<FCollisionProfile*>(Dest->GetProfile());

    if (DestProfile->Channel == ECollisionChannel::PlayerAttack)
    {
        if (!mHPWidget)
            return;

        mHPWidget->SetEnable(true);
        mHitTimer = 3.f;
    }*/
}

void CMonsterObject::CollisionMonsterEnd(
    CColliderBase* Dest)
{
    /*if (mTarget)
        mStateMachine->ChangeStateMonster(EMonsterAIState::Trace, mMonsterDir);*/
}


void CMonsterObject::AIDeath()
{
    
}
