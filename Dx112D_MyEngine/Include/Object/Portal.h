#pragma once
#include "SceneObject.h"

class CPortal :
    public CSceneObject
{
    friend class CScene;
protected:
    CPortal();
    CPortal(const CPortal& Obj);
    CPortal(CPortal&& Obj);
    virtual ~CPortal();

protected:
    CSharedPtr<class CSpriteComponent>       mRoot;
    CSharedPtr<class CColliderAABB2D>        mBody;

    // 어떤 씬으로 이동할지 저장.
    ESceneType mTargetScene = ESceneType::End;
    // 플레이어를 어디로 이동시킬지 저장.
    FVector3D mTargetPos = FVector3D(-1.f, -1.f, -1.f);

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();

    void SetPortalTexture(const std::string& Name,
        const TCHAR* FileName);

    virtual void SetPortalSize(const FVector2D& Size);
    virtual void SetPortalSize(float x, float y);

    virtual void SetTargetScene(ESceneType TargetScene)
    {
        mTargetScene = TargetScene;
    }

    virtual void SetTargetPos(const FVector3D& Pos)
    {
        mTargetPos = Pos;
    }
    
    virtual void SetTargetPos(float x, float y, float z)
    {
        mTargetPos.x = x;
        mTargetPos.y = y;
        mTargetPos.z = z;
    }
    virtual void SetTargetPos(const FVector2D& Pos)
    {
        mTargetPos.x = Pos.x;
        mTargetPos.y = Pos.y;
        mTargetPos.z = 1.f;
    }
    virtual void SetTargetPos(float x, float y)
    {
        mTargetPos.x = x;
        mTargetPos.y = y;
        mTargetPos.z = 1.f;
    }

private:
    void CollisionPortal(const FVector3D& HitPoint,
        class CColliderBase* Dest);
};

