#pragma once
#include "SceneObject.h"


class CCameraObject :
    public CSceneObject
{
    friend class CScene;

protected:
    CCameraObject();
    CCameraObject(const CCameraObject& Obj);
    CCameraObject(CCameraObject&& Obj);
    virtual ~CCameraObject();

protected:
    CSceneObject*                           mTarget;
    CSharedPtr<class CTileMapObj>           mTileMap;
    CSharedPtr<class CCameraComponent>      mCamera;

    bool            mFirst = true;
public:
    virtual bool Init();
    virtual void PreUpdate(float DeltaTime) override;
    virtual void Update(float DeltaTime);
    virtual void Render();


public:
    void    SetTarget(CSceneObject* Target) { mTarget = Target; }
    void    SetTileMap(CSharedPtr<class CTileMapObj> Map);

private:
    void    FollowTarget();
};

