#pragma once

#include "SceneObject.h"

class CMonsterObject :
    public CSceneObject
{
    friend class CScene;

protected:
    CMonsterObject();
    CMonsterObject(const CMonsterObject& Obj);
    CMonsterObject(CMonsterObject&& Obj);
    virtual ~CMonsterObject();

protected:
    CSharedPtr<class CSpriteComponent>        mRoot;
    CSharedPtr<class CColliderSphere2D>       mBody;

    // 몬스터의 인지 반경 충돌체
    CSharedPtr<class CColliderSphere2D>       mDetect;
    // 몬스터의 공격 반경 충돌체
    CSharedPtr<class CColliderSphere2D>       mAttackRange;

    // 몬스터의 HP정보를 표시해줄 Widget
    CSharedPtr<class CWidgetComponent>        mHPWidget;
    // 이동 컴포넌트
    CSharedPtr<class CMonsterMovement>        mMovement;
    // 상태 머신
    CSharedPtr<class CStateMachineComponent>  mStateMachine;

    class CAnimation2D* mAnimation = nullptr;

    // 몬스터의 타겟
    CSharedPtr<CSceneObject>        mTarget;

    // 현재 몬스터가 바라보는 방향.
    EObjectDir          mMonsterDir = EObjectDir::B;
    // 몬스터의 타입 (ex: 보스, 메이지, 근접)
    EMonsterType        mMonsterType = EMonsterType::None;

    // 현재 몬스터 공격 반경에 들어와있는지 체크하는 변수.
    bool            mIsInAttackRange = false;
    // 현재 몬스터가 죽었는지 판단하는 변수
    bool            mDead = false; 
    // 몬스터가 공격중 인지 확인하는 변수
    bool            mAttacking = false;
    // 몬스터가 추격중 인지 확인하는 변수.
    bool            mTracing = false;

    bool            mIsHit = false;
    float           mHitTimer = 3.f;
    // 체력, 최대체력
    int             mHP;
    int             mMaxHP;

public:
    virtual class CColliderBase* GetCollider() const;
    class CMonsterMovement* GetMovement() const;

    CSceneObject* GetTarget()
    {
        if (mTarget)
            return mTarget;

        return nullptr;
    }

    EMonsterType GetMonsterType() const
    {
        return mMonsterType;
    }

    EObjectDir GetMonsterDir() const
    {
        return mMonsterDir;
    }

    FVector2D FindRandomTargetTile() const;

public:
    void    SetTarget(class CSceneObject* Target)
    {
        mTarget = Target;
    }

    void    SetMonsterType(EMonsterType Type)
    {
        mMonsterType = Type;
    }

    void    SetAttacking(bool Attacking) 
    { 
        mAttacking = Attacking;
    }

    void    SetTracing(bool Tracing) 
    { 
        mTracing = Tracing; 
    }

public:
    bool    IsDead() { return mHP <= 0; }
    bool    IsAttacking() { return mAttacking; }
    bool    IsTracing() { return mTracing; }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
   
    virtual void ConstrainToMap();
    virtual void ChangeAnimation(const std::string Name);

public:
    virtual float Damage(float Attack, CSceneObject* Obj);
    virtual void  AIDeath();

protected:
    virtual void CollisionMonster(const FVector3D& HitPoint,
        class CColliderBase* Dest);
    virtual void CollisionMonsterEnd(class CColliderBase* Dest);

};

