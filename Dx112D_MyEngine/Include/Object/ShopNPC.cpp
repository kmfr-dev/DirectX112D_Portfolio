#include "ShopNPC.h"
#include "../Component/SpriteComponent.h"
#include "../Component/ColliderSphere2D.h"
#include "../Animation/Animation2D.h"
#include "../Component/StoreComponent.h"

#include "../Scene/Scene.h"
#include "../Scene/SceneAssetManager.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Texture/TextureManager.h"
#include "../Asset/Texture/Texture.h"

#include "../ItemManager.h"

CShopNPC::CShopNPC()
{
}

CShopNPC::CShopNPC(const CShopNPC& Obj)
{
}

CShopNPC::CShopNPC(CShopNPC&& Obj)
{
}

CShopNPC::~CShopNPC()
{
 
}

bool CShopNPC::Init()
{
    CNPCObject::Init();

    mNPCName = "ShopNPC";
    mNPCID = 2;
    mCurrentScriptID = 1;

    mRoot->SetWorldScale(200.f, 200.f, 1.f);
    mBody->SetRadius(40.f);

    mAnimation->AddSequence("ShopNPC", 1.f, 1.f, true, false);

    mQuestMark->SetEnable(false);

    mStoreComponent = CreateComponent<CStoreComponent>("StoreComponent");
   
    mItemList.push_back(CItemManager::GetInst()->FindItem("레몬"));
    mItemList.push_back(CItemManager::GetInst()->FindItem("신비한 과일"));
    mItemList.push_back(CItemManager::GetInst()->FindItem("슬픈 과일"));
    mItemList.push_back(CItemManager::GetInst()->FindItem("졸린 과일"));
    mItemList.push_back(CItemManager::GetInst()->FindItem("기운이 솟는 호박"));
    mItemList.push_back(CItemManager::GetInst()->FindItem("영문을 모르겠는 과일"));
    mItemList.push_back(CItemManager::GetInst()->FindItem("메론"));
    mItemList.push_back(CItemManager::GetInst()->FindItem("?"));
    mItemList.push_back(CItemManager::GetInst()->FindItem("큐브 젤리"));

    mStoreComponent->SetItems(mItemList);
    

    return true;
}

void CShopNPC::Update(float DeltaTime)
{
    CNPCObject::Update(DeltaTime);
}

void CShopNPC::Render()
{
    CNPCObject::Render();
}