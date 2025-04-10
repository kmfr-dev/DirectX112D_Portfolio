#include "StoreComponent.h"
#include "InventoryComponent.h"

#include "../Scene/Scene.h"
#include "../Scene/SceneUIManager.h"

#include "../Asset/Texture/Texture.h"

#include "../UI/UserWidget/MainWidget.h"
#include "../UI/UserWidget/StoreWidget.h"

#include "../Object/PlayerObject.h"

CStoreComponent::CStoreComponent()
{
}

CStoreComponent::~CStoreComponent()
{
}

bool CStoreComponent::Init()
{
    CComponent::Init();
    
    return true;
}

void CStoreComponent::SetItems(const std::vector<FItemData*>& Items)
{
    mStoreItems = Items;
}

void CStoreComponent::BuyItem(FItemData* Item)
{
    // 구매할 아이템이 없으면 return
    if (!Item)
        return;

    CPlayerObject* Player = mScene->FindObjectFromType<CPlayerObject>();

    // 만약 플레이어가 없고 플레이어의 골드가 
    // 구매할 아이템의 판매금액보다 낮으면 판매X
    if (!Player || Player->GetGold() < Item->BuyPrice)
        return;
    
    // 플레이어의 골드를 차감한다.
    Player->AddGold(-Item->BuyPrice);

    // 플레이어의 인벤토리에 추가한다.
    // 아이템 매니저에서 깊은복사해서 넘어온 아이템이기 때문에
    // 플레이어의 인벤토리에  바로 넣어준다.
    Player->GetInventory()->AddItem(Item);

    CStoreWidget* StoreUI = mScene->GetMainWidget()->GetStoreUI();

   // 플레이어의 골드 상태를 갱신한다.
    if (StoreUI)
        StoreUI->SetGoldInfo(Player->GetGold());
}

void CStoreComponent::OpenStore(CPlayerObject* Player)
{
    // ���� ���� ������ �����ִٸ� return
    if (mStoreOpened)
        return;

    // ������������ ���� UI�� �����´�.
    CStoreWidget* StoreUI = mScene->GetMainWidget()->GetStoreUI();

    if (!StoreUI)
        return;
    // ���� UI�� Ȱ��ȭ
    StoreUI->SetEnable(true);
    StoreUI->SetGoldInfo(Player->GetGold());
    StoreUI->SetItemList(mStoreItems);

    // StoreUI�� Store������Ʈ�� �˰��Ѵ�. 
    if (!StoreUI->GetStoreComponent())
        StoreUI->SetStoreComponent(this);

    mStoreOpened = true;
}

void CStoreComponent::CloseStore()
{
    CStoreWidget* StoreUI = mScene->GetMainWidget()->GetStoreUI();

    // ���� UI ��Ȱ��ȭ
    if (StoreUI)
        StoreUI->SetEnable(false);

    // ������ �ݾ��ش�.
    mStoreOpened = false;
}
