#pragma once

#include "SceneObject.h"

enum class EObjectSpawnType : unsigned char
{
    None,
    Player,
    Bullet,
    GravityBullet,
    TalonR,
    TornadoBullet,
    MageMonster,
    NearingMonster
};

enum class EObjectSpawnLoopType : unsigned char
{
    Once,   // 생성된 오브젝트가 없을 경우에만 생성
    Loop    // 생성된 오브젝트 상관없이 계속 생성
};

enum class EObjectSpawnCountType : unsigned char
{
    OnceDestroy,    // 생성되고 스폰포인트 바로 제거
    Loop,           // 무한으로 생성
    CountDestroy,   // 일정 수량만큼 생성하고 스폰포인트 제거
};

class CObjectSpawnPoint :
    public CSceneObject
{
    friend class CScene;

protected:
    CObjectSpawnPoint();
    CObjectSpawnPoint(const CObjectSpawnPoint& Obj);
    CObjectSpawnPoint(CObjectSpawnPoint&& Obj);
    virtual ~CObjectSpawnPoint();

protected:
    CSharedPtr<class CSceneComponent>  mRoot;
    EObjectSpawnType    mSpawnType = EObjectSpawnType::None;
    EObjectSpawnLoopType    mSpawnLoopType = 
        EObjectSpawnLoopType::Once;
    EObjectSpawnCountType   mSpawnCountType =
        EObjectSpawnCountType::Loop;
    // 실제 스폰된 오브젝트
    CSceneObject* mSpawnObject = nullptr;
    float       mSpawnTime = 0.f;
    float       mTime = 0.f;

    int         mDestroySpawnCount = 0;

    bool        mImmediateSpawn = true;

public:

    void SetSpawnType(EObjectSpawnType Type)
    {
        mSpawnType = Type;
    }

    void SetSpawnLoopType(EObjectSpawnLoopType Type)
    {
        mSpawnLoopType = Type;
    }

    void SetSpawnCountType(EObjectSpawnCountType Type)
    {
        mSpawnCountType = Type;
    }

    void SetSpawnTime(float Time)
    {
        mSpawnTime = Time;
    }

    void SetDestroySpawnCount(int Count)
    {
        mDestroySpawnCount = Count;
    }

    void SetImmediateSpawn(bool Spawn)
    {
        mImmediateSpawn = Spawn;
    }

    void ClearObject()
    {
        mSpawnObject = nullptr;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

private:
    // 시간이 다 되었을 경우 true, 아니면 false
    bool ComputeSpawnTime(float DeltaTime);
    void ComputeSpawnCountType();
    void Spawn();
};

