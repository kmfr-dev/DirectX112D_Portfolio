#include "ObjectSpawnPoint.h"
#include "../Component/SceneComponent.h"
#include "PlayerObject.h"
#include "BulletObject.h"
#include "MageMonster.h"
#include "NearingMonster.h"
#include "../Scene/Scene.h"

CObjectSpawnPoint::CObjectSpawnPoint()
{
}

CObjectSpawnPoint::CObjectSpawnPoint(const CObjectSpawnPoint& Obj)  :
    CSceneObject(Obj)
{
}

CObjectSpawnPoint::CObjectSpawnPoint(CObjectSpawnPoint&& Obj) :
    CSceneObject(Obj)
{
}

CObjectSpawnPoint::~CObjectSpawnPoint()
{
}

bool CObjectSpawnPoint::Init()
{
    mRoot = CreateComponent<CSceneComponent>();

    SetRootComponent(mRoot);

    return true;
}

void CObjectSpawnPoint::Update(float DeltaTime)
{
    CSceneObject::Update(DeltaTime);

    if (mImmediateSpawn)
    {
        mImmediateSpawn = false;
        Spawn();
    }

    else
    {
        switch (mSpawnLoopType)
        {
        // 생성된 오브젝트가 없을 경우에만 생성
        case EObjectSpawnLoopType::Once:
            if (!mSpawnObject)
            {
                // 하나가 생성되고 시간이 지났다면
                if (ComputeSpawnTime(DeltaTime))
                {
                    mTime = 0.f;
                }
            }
            break;
        // 생성된 오브젝트 상관없이 계속 생성
        case EObjectSpawnLoopType::Loop:
            ComputeSpawnTime(DeltaTime);
            break;
        }
    }
}

bool CObjectSpawnPoint::ComputeSpawnTime(float DeltaTime)
{
    if (mSpawnTime == 0.f)
    {
        Spawn();

        return true;
    }

    mTime += DeltaTime;

    // 스폰해야 할 시간이 되었을 경우
    if (mTime >= mSpawnTime)
    {
        mTime -= mSpawnTime;

        Spawn();

        return true;
    }

    return false;
}

void CObjectSpawnPoint::ComputeSpawnCountType()
{
    switch (mSpawnCountType)
    {
    case EObjectSpawnCountType::OnceDestroy:
        Destroy();
        break;
    case EObjectSpawnCountType::CountDestroy:
        --mDestroySpawnCount;

        if (mDestroySpawnCount <= 0)
            Destroy();
        break;
    }
}

void CObjectSpawnPoint::Spawn()
{
    switch (mSpawnType)
    {
    case EObjectSpawnType::Player:
        mSpawnObject = mScene->CreateObj<CPlayerObject>("Player");
        break;
    case EObjectSpawnType::Bullet:
        mSpawnObject = mScene->CreateObj<CBulletObject>("Bullet");
        break;
    case EObjectSpawnType::MageMonster:
        mSpawnObject = mScene->CreateObj<CMageMonster>("MageMonster");
        break;
    case EObjectSpawnType::NearingMonster:
        mSpawnObject = mScene->CreateObj<CNearingMonster>("NearingMonster");
        break;

    }
    mSpawnObject->SetTileMap(mScene->GetMapObj());
    mSpawnObject->SetWorldRotation(GetWorldRotation());
    mSpawnObject->SetWorldPos(GetWorldPosition());
    mSpawnObject->SetSpawnPoint(this);

    ComputeSpawnCountType();
}
