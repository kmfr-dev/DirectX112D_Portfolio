#pragma once
#include "MonsterObject.h"
class CMageMonster :
    public CMonsterObject
{
    friend class CScene;

protected:
    CMageMonster();
    CMageMonster(const CMageMonster& Obj);
    CMageMonster(CMageMonster&& Obj);
    virtual ~CMageMonster();

protected:
    CSharedPtr<class CColliderSphere2D> mAttackRange;
   
public:
    virtual bool  Init();
    virtual void  Update(float DeltaTime);
    virtual float Damage(float Attack, CSceneObject* Obj) override;
    virtual void  AIDeath() override;

protected:
    virtual void CollisionAttackDetect(const FVector3D& HitPoint,
        class CColliderBase* Dest);

    virtual void CollisionAttackDetectEnd(class CColliderBase* Dest);

    virtual void CollisionMonsterDetect(const FVector3D& HitPoint,
        class CColliderBase* Dest);
    virtual void CollisionMonsterDetectEnd(class CColliderBase* Dest);

};