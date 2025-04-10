#pragma once

#include "MonsterObject.h"

class CNearingMonster :
    public CMonsterObject
{
    friend class CScene;

protected:
    CNearingMonster();
    CNearingMonster(const CNearingMonster& Obj);
    CNearingMonster(CNearingMonster&& Obj);
    virtual ~CNearingMonster();

protected:
    CSharedPtr<class CColliderSphere2D> mAttackRange;

public:
    virtual bool  Init();
    virtual void  Update(float DeltaTime);
    virtual float Damage(float Attack, CSceneObject* Obj) override;
    virtual void  AIDeath() override;

protected:
    virtual void CollisionMonster(const FVector3D& HitPoint,
        class CColliderBase* Dest) override;
    virtual void CollisionMonsterEnd(class CColliderBase* Dest) override;

    virtual void CollisionAttackDetect(const FVector3D& HitPoint,
        class CColliderBase* Dest);

    virtual void CollisionAttackDetectEnd(class CColliderBase* Dest);

    virtual void CollisionMonsterDetect(const FVector3D& HitPoint,
        class CColliderBase* Dest);
    virtual void CollisionMonsterDetectEnd(class CColliderBase* Dest);
};

