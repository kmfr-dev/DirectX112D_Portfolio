#include "SceneStart.h"
#include "../UI/UserWidget/StartWidget.h"
#include "SceneUIManager.h"
#include "../Object/BackGroundObj.h"
#include "../Object/BackGroundImgObj.h"
#include "../Scene/SceneAssetManager.h"

CSceneStart::CSceneStart()
{
}

CSceneStart::~CSceneStart()
{
}

bool CSceneStart::InitAsset()
{
    return true;
}

bool CSceneStart::InitObject()
{
    CBackGroundObj* Background = CreateObj<CBackGroundObj>("StartBgObj");
    CBackGroundImgObj* Ogu = CreateObj<CBackGroundImgObj>("StartOguObj");
    

    return true;
}

bool CSceneStart::InitWidget()
{
    CStartWidget* Widget = mUIManager->CreateWidget<CStartWidget>("Start");

    mUIManager->AddToViewport(Widget);

    return true;
}
