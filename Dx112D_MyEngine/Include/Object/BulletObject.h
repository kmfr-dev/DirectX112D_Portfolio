#pragma once

#include "SceneObject.h"

class CBulletObject :
    public CSceneObject
{
    friend class CScene;

protected:
    CBulletObject();
    CBulletObject(const CBulletObject& Obj);
    CBulletObject(CBulletObject&& Obj);
    virtual ~CBulletObject();

protected:
    CSharedPtr<class CSpriteComponent>  mRoot;
    CSharedPtr<class CColliderSphere2D>       mBody;
    CSharedPtr<class CMovementComponent>    mMovement;

    FVector3D mDirection = FVector3D::Zero;
    float   mSpeed = 400.f;

public:
    virtual void SetSpeed(float Speed)
    {
        mSpeed = Speed;
    }
    virtual void SetDirection(const FVector3D& Dir)
    {
        mDirection = Dir;
        mDirection.Normalize();
    }
    virtual void SetBulletCollisionProfile(const std::string& Name);

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);



private:
    void CollisionBullet(const FVector3D& HitPoint,
        class CColliderBase* Dest);
};

