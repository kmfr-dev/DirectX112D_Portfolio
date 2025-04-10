#pragma once
#include "SceneObject.h"
class CBackGroundObj :
    public CSceneObject
{
    friend class CScene;

protected:
    CBackGroundObj();
    CBackGroundObj(const CBackGroundObj& Obj);
    CBackGroundObj(CBackGroundObj&& Obj);
    virtual ~CBackGroundObj();

protected:
    CSharedPtr<class CSpriteComponent>  mRoot;
    CSharedPtr<class CCameraComponent>      mCamera;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();
};

