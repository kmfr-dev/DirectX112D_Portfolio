#pragma once
#include "SceneObject.h"
class CBackGroundImgObj :
    public CSceneObject
{
    friend class CScene;

protected:
    CBackGroundImgObj();
    CBackGroundImgObj(const CBackGroundImgObj& Obj);
    CBackGroundImgObj(CBackGroundImgObj&& Obj);
    virtual ~CBackGroundImgObj();

protected:
    CSharedPtr<class CSpriteComponent>      mRoot;
    CSharedPtr<class CCameraComponent>      mCamera;
    class CAnimation2D* mAnimation = nullptr;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();
};

