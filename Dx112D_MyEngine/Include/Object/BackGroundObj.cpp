#include "BackGroundObj.h"
#include "../Device.h"
#include "../Scene/Scene.h"
#include "../Component/CameraComponent.h"
#include "../Component/SpriteComponent.h"
#include "../Animation/Animation2D.h"
#include "../Share/Log.h"
#include "../Scene/SceneAssetManager.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Texture/Texture.h"
#include "../Asset/Texture/TextureManager.h"
#include "../Render/RenderManager.h"

CBackGroundObj::CBackGroundObj()
{
}

CBackGroundObj::CBackGroundObj(const CBackGroundObj& Obj)
{
}

CBackGroundObj::CBackGroundObj(CBackGroundObj&& Obj)
{
}

CBackGroundObj::~CBackGroundObj()
{
}

bool CBackGroundObj::Init()
{
    mRoot = CreateComponent<CSpriteComponent>("StartBg");

    CRenderManager::GetInst()->ChangeLayer(mRoot, "BackGround");

    mCamera = CreateComponent<CCameraComponent>();
    
    mRoot->SetTexture("StargBackGround", TEXT("Texture\\StartScene\\StartBG.png"));
   
    mRoot->SetPivot(0.5f, 0.5f);

    mRoot->SetWorldPos(-7.f, -7.f, 0.f);

    FResolution RS = CDevice::GetInst()->GetResolution();
    mRoot->SetWorldScale((float)RS.Width + 7.f,(float)RS.Height + 7.f, 1.f);

    SetRootComponent(mRoot);

    mCamera->SetProjectionType(ECameraProjectionType::Ortho);

    mRoot->AddChild(mCamera);

    return true;
}

void CBackGroundObj::Update(float DeltaTime)
{
    CSceneObject::Update(DeltaTime);
}

void CBackGroundObj::Render()
{
    CSceneObject::Render();
}
