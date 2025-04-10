#pragma once
#include "MonsterObject.h"

class CNueBossMonster :
    public CMonsterObject
{
    friend class CScene;

protected:
    CNueBossMonster();
    CNueBossMonster(const CNueBossMonster& Obj);
    CNueBossMonster(CNueBossMonster&& Obj);
    virtual ~CNueBossMonster();

protected:
    CSharedPtr<class CColliderSphere2D> mAttackRange;

    float       mSpeed = 700.f;
    float       mSpPatternCount = 0.f;
    bool        mSpPattern = false;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void AIDeath() override;
    virtual float Damage(float Attack, CSceneObject* Obj);

public:
    void        SetSpeed(float Speed) { mSpeed = Speed; }
    void        SetHP(int HP) { mHP = HP; }
    void        SetSpPattern(bool SpPattern) { mSpPattern = SpPattern; }
    void        PlusPatternCount()
    {
        ++mSpPatternCount;
        if (mSpPatternCount == 5)
        {
            mSpPatternCount = 0;
            mSpPattern = true;
        }
    }

public:
    float       GetSpeed() { return mSpeed; }
    int         GetHP() { return mHP; }
    int         GetMaxHP() { return mMaxHP; };
public:
    bool        IsUseSpecialPattern() { return mSpPattern; }

protected:
    void        AttackNotify();
    void        AttackEnd();

protected:
    virtual void CollisionMonster(const FVector3D& HitPoint,
        class CColliderBase* Dest);
    virtual void CollisionMonsterEnd(class CColliderBase* Dest);

    virtual void CollisionAttackDetect(const FVector3D& HitPoint,
        class CColliderBase* Dest);

    virtual void CollisionAttackDetectEnd(class CColliderBase* Dest);

    virtual void CollisionMonsterDetect(const FVector3D& HitPoint,
        class CColliderBase* Dest);
    virtual void CollisionMonsterDetectEnd(class CColliderBase* Dest);
};

