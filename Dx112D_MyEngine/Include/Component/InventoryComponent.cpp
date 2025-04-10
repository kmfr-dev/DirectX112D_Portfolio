#include "InventoryComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneAssetManager.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Texture/Texture.h"
#include "../Asset/Texture/TextureManager.h"
#include "../Scene/SceneUIManager.h"
#include "../UI/UserWidget/Inventory.h"
#include "../Object/PlayerObject.h"
#include "../ItemManager.h"
#include "../PlayerManager.h"

CInventoryComponent::CInventoryComponent()
{
}

CInventoryComponent::CInventoryComponent(
    const CInventoryComponent& Com) :
    CComponent(Com)
{
}

CInventoryComponent::CInventoryComponent(CInventoryComponent&& Com) :
    CComponent(Com)
{
}

CInventoryComponent::~CInventoryComponent()
{
    CPlayerManager::GetInst()->GetPlayerData()
        ->SetInventoryItems(mItemList);

    for (int i = 0; i < mItemList.size(); ++i)
    {
        SAFE_DELETE(mItemList[i]);
    }
}


void CInventoryComponent::AddItem(FItemData* NewItem)
{
    for (int i = 0; i < mItemList.size(); ++i)
    {
        
        if (!mItemList[i])
        {
            mItemList[i] = NewItem;

            if (mChangeFunction)
                mChangeFunction(NewItem, i);
           
            return;
        }

        // 기존 아이템의 이름과 추가할 아이템의 이름이 같고
        // 카테고리도 같고, 만약 소모품이라면 동일한 아이템을 추가
        // 하려고할 것이다. 그러므로 추가할 아이템을 지우고
        // 기존 아이템의 갯수를 늘려준다.
        if(mItemList[i]->Name == NewItem->Name &&
            mItemList[i]->Category == NewItem->Category &&
            mItemList[i]->Category == EItemCategory::Consumable)
        {
            ++mItemList[i]->Count;
            SAFE_DELETE(NewItem);

            if (mChangeFunction)
                mChangeFunction(mItemList[i], i);
            
            return;
        }
    }
}

void CInventoryComponent::RemoveItem(int Index)
{
    if (!mItemList[Index])
        return;

    SAFE_DELETE(mItemList[Index]);

    if (mChangeFunction)
        mChangeFunction(nullptr, Index);
}

void CInventoryComponent::UseItem(FItemData* Item)
{
    // 만약 아이템이 없거나 아이템을 사용할 수 없거나
    // 아이템 갯수가 0보다 작거나 같다면 return
    if (!Item || !Item->Useable || Item->Count <= 0)
        return;

    // 여기까지 오면 아이템을 사용해야 한다.

    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(mOwnerObject);
    if (!Player)
        return;

    // 1. 아이템 효과 적용.
    switch (Item->StatInfo[0].Type)
    {
    case EItemStatType::HP:
        Player->AddHP(Item->StatInfo[0].Value);
        break;
    case EItemStatType::Attack:
        break;
    }

    // 2. 사용했으니 아이템 수량을 감소시킨다.
    --Item->Count;

    // 3. 아이템 갯수가 0이되면 제거한다.
    if (Item->Count <= 0)
    {
        // 아이템 목록 전체 반복
        for (int i = 0; i < mItemList.size(); ++i)
        {
            // 아이템 목록중에 사용한 아이템을 찾았다면
            if (mItemList[i] == Item)
            {
                RemoveItem(i);
            }
        }
    }

    // 4. 아이템 갯수가 0이 아니라면
    else
    {
        // 아이템 갯수만 갱신
        for (int i = 0; i < mItemList.size(); ++i)
        {
            if (mItemList[i] == Item)
            {
                if (mChangeFunction)
                    mChangeFunction(Item, i);
                break;
            }
        }
    }

    CPlayerManager::GetInst()->GetPlayerData()->
        SetInventoryItems(mItemList);
}

bool CInventoryComponent::Init()
{
    if (!CComponent::Init())
        return false;

    CInventory* Inventory =
        dynamic_cast<CInventory*>(mScene->GetUIManager()->FindWindowWidget("Inventory"));

    if (Inventory)
    {
        SetChangeFunction<CInventory>(Inventory,
            &CInventory::InventoryChange);/*
        Inventory->SetCategory()*/
    }

    mItemList.resize(9, nullptr);
 
    return true;
}

bool CInventoryComponent::Init(const char* FileName)
{
    if (!CComponent::Init(FileName))
        return false;

    return true;
}

void CInventoryComponent::PreUpdate(float DeltaTime)
{
    CComponent::PreUpdate(DeltaTime);
}

void CInventoryComponent::Update(float DeltaTime)
{
    CComponent::Update(DeltaTime);
}

void CInventoryComponent::PostUpdate(float DeltaTime)
{
    CComponent::PostUpdate(DeltaTime);
}

CInventoryComponent* CInventoryComponent::Clone()
{
    return new CInventoryComponent(*this);
}

void CInventoryComponent::EndFrame()
{
    CComponent::EndFrame();
}

