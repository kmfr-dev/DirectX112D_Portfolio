#include "BackGroundImgObj.h"
#include "../Scene/Scene.h"
#include "../Component/CameraComponent.h"
#include "../Component/SpriteComponent.h"
#include "../Animation/Animation2D.h"
#include "../Share/Log.h"
#include "../Scene/SceneAssetManager.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Texture/Texture.h"
#include "../Asset/Texture/TextureManager.h"
#include "../Device.h"

CBackGroundImgObj::CBackGroundImgObj()
{
}

CBackGroundImgObj::CBackGroundImgObj(const CBackGroundImgObj& Obj)
{
}

CBackGroundImgObj::CBackGroundImgObj(CBackGroundImgObj&& Obj)
{
}

CBackGroundImgObj::~CBackGroundImgObj()
{
}

bool CBackGroundImgObj::Init()
{
    FResolution RS = CDevice::GetInst()->GetResolution();

    mRoot = CreateComponent<CSpriteComponent>("Player");
 
    mCamera = CreateComponent<CCameraComponent>();

    mRoot->SetTexture("StartOguImg", TEXT("Texture\\StartScene\\StartOgu.png"));

    mRoot->SetPivot(0.5f, 0.5f);

    mRoot->SetWorldPos(330.f, -70.f, 0.f);
    mRoot->SetWorldScale(300.f, 300.f, 1.f);

    SetRootComponent(mRoot);

    mCamera->SetProjectionType(ECameraProjectionType::Ortho);

    mRoot->AddChild(mCamera);
    

    mAnimation = mRoot->CreateAnimation2D<CAnimation2D>();

    mAnimation->AddSequence("StartOguIdle", 1.f, 1.f, true, true);

	return true;
}

void CBackGroundImgObj::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);
}

void CBackGroundImgObj::Render()
{
	CSceneObject::Render();
}
