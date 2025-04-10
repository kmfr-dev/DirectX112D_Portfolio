#include "Inventory.h"
#include "../../Share/Log.h"
#include "../Common/Image.h"
#include "../Common/Button.h"
#include "../Common/TextBlock.h"
#include "../Common/TitleBar.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneAssetManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Object/PlayerObject.h"

#include "../../Asset/Texture/Texture.h"
#include "../Common/ResourceInfo.h"
#include "../Common/InvenItemInfo.h"
#include "../Common/CategoryButtons.h"
#include "../../Component/InventoryComponent.h"
#include "../../Device.h"


CInventory::CInventory()
{
}

CInventory::~CInventory()
{
}

bool CInventory::Init()
{
    CWindowWidget::Init();

    FResolution RS = CDevice::GetInst()->GetResolution();

    SetSize((float)RS.Width, (float)RS.Height);

    mInvenBack = mScene->GetUIManager()->CreateWidget<CImage>("InvenBack");
    mInvenBack->SetTexture("InvenBack", TEXT("Texture\\UI\\BasicBox.png"));
    mInvenBack->SetSize((float)RS.Width, (float)RS.Height);
    AddWidget(mInvenBack);

    // 플레이어 골드 정보
    mGoldInfo = mScene->GetUIManager()->CreateWidget<CResourceInfo>("GoldInfo");
    mGoldInfo->SetBackTexture("InfoBack", TEXT("Texture\\UI\\ItemInfoBack.png"));
    mGoldInfo->SetBackSize(200.f, 50.f);
    
    mGoldInfo->SetIconTexture("GoldSymbol", TEXT("Texture\\UI\\GoldSymbol.png"));
    mGoldInfo->SetIconSize(35.f, 35.f);
    mGoldInfo->SetIconPos(10.f, 7.f);
    
    mGoldInfo->SetInfoText(TEXT(""));
    mGoldInfo->SetInfoTextBoxSize(200.f, 50.f);

    mGoldInfo->SetText(TEXT("Gold"));
    mGoldInfo->SetTextBoxSize(200.f, 50.f);
    mGoldInfo->SetPos(RS.Width - 350.f, RS.Height -90.f);

    AddWidget(mGoldInfo);
    
    // 슬롯 오른쪽에 표시될 아이템 정보
    mItemInfo = mScene->GetUIManager()->CreateWidget<CInvenItemInfo>("ItemInfo");
    mItemInfo->SetPos(RS.Width - 600.f, 50.f);
    AddWidget(mItemInfo);

    // 카테고리 버튼들(4개)
    mCategoryBtns = mScene->GetUIManager()->CreateWidget<CCategoryButtons>("Categorys");
    mCategoryBtns->SetPos(450.f, RS.Height - 150.f);
    mCategoryBtns->mConsumable->SetEventCallback(EButtonEventState::Click, this,
        &CInventory::ChangeConsumCategory);
    mCategoryBtns->mHat->SetEventCallback(EButtonEventState::Click, this,
        &CInventory::ChangeHatCategory);
    mCategoryBtns->mQuest->SetEventCallback(EButtonEventState::Click, this,
        &CInventory::ChangeQuestCategory);
    mCategoryBtns->mGrass->SetEventCallback(EButtonEventState::Click, this,
        &CInventory::ChangeGrassCategory);
    
    AddWidget(mCategoryBtns);



    // 아이템 슬롯 목록
    mSlotList.resize(9);

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            std::string SlotName = "Slot" +
                std::to_string(i * 3 + j);

            CSlot* Slot =
                mScene->GetUIManager()->CreateWidget<CSlot>(SlotName);

            Slot->SetSlotType(ESlotType::Inventory);
            Slot->SetSize(160.f, 160.f);
            Slot->SetIconSize(120.f, 120.f);
            Slot->SetPos(j * 270.f + 150.f, (2 - i) * 200.f + 150.f);
            Slot->SetIconPos(20.f, 20.f);

            Slot->SetIndex(i * 3 + j);

            int Index = i * 3 + j;
            Slot->SetClickCallback([this, Index]()
                {
                    SelectSlot(Index);
                });

            mSlotList[i * 3 + j] = Slot;

            AddWidget(Slot);
        }
    }

    return true;
}

void CInventory::Update(float DeltaTime)
{
    CWindowWidget::Update(DeltaTime);
}

void CInventory::Render()
{
    CWindowWidget::Render();
}

void CInventory::SetEnable(bool Enable)
{
    CObject::SetEnable(Enable);

    if (Enable)
    {
        RefreshCategory();

        // 셀렉션 초기화
        for (int i = 0; i < mSlotList.size(); ++i)
        {
            if (mSlotList[i]->GetItemData())
            {
                SelectSlot(i);
                break;
            }
        }
    }
    else
    {
        mSelectedSlotIndex = -1;
        for (auto& Slot : mSlotList)
            Slot->SetSelected(false);

        if (mItemInfo)
            mItemInfo->SetItemInfo(nullptr);
    }
}

void CInventory::SetCategory(EItemCategory Category)
{
    // 현재 카테고리를 설정해주고 카테고리를 새로고침 해준다.
    mCurrentCategory = Category;
    RefreshCategory();   

    // 먼저 모든 슬롯 셀렉션 해제
    for (auto& Slot : mSlotList)
        Slot->SetSelected(false);


    // 셀렉션 초기화
    for (int i = 0; i < mSlotList.size(); ++i)
    {
        FItemData* Data = mSlotList[i]->GetItemData();
        // 만약 아이템이 있고, 아이템의 카테고리가 현재카테고리와 같을 경우
        if (Data && Data->Category == mCurrentCategory)
        {
            SelectSlot(i); 
            return;
        }
    }

    if (mItemInfo)
        mItemInfo->SetItemInfo(nullptr);
}

void CInventory::SetGoldInfo(int Gold)
{
    std::wstring GoldInfo = ToWString(std::to_string(Gold).c_str());
    mGoldInfo->SetInfoText(GoldInfo.c_str());
}

void CInventory::RefreshCategory()
{
    // 인벤토리 컴포넌트를 얻어온다.
    CInventoryComponent* InvenCom = CSceneManager::GetInst()->GetCurrentScene()->
        FindObjectFromType<CPlayerObject>()->GetInventory();

    if (!InvenCom) 
        return;

    // 인벤토리 컴포넌트에서 아이템 목록을 얻어온다.
    const std::vector<FItemData*>& ItemList = InvenCom->GetItemList(); 

    int SlotIndex = 0;
    // 아이템 목록 전체 반복
    for (FItemData* Item : ItemList)
    {
        if (!Item) 
            continue;

        // 현재 아이템의 카테고리가 
        // 인벤토리 위젯의 현재 카테고리와 같은경우
        if (Item->Category == mCurrentCategory)
        {
            // 슬롯 인덱스가 전체 슬롯갯수 보다 작을경우에만
            if (SlotIndex < mSlotList.size())
            {
                // 차례대로 슬롯에 아이템과 아이콘을 설정해준다.
                mSlotList[SlotIndex]->SetTextureIcon(Item->Icon);
                mSlotList[SlotIndex]->SetItemData(Item);
                ++SlotIndex;
            }
        }
    }

    // 나머지 슬롯 비우기
    for (; SlotIndex < mSlotList.size(); ++SlotIndex)
    {
        mSlotList[SlotIndex]->SetTextureIcon(nullptr);
        mSlotList[SlotIndex]->SetItemData(nullptr);
    }
}

void CInventory::InventoryChange(const FItemData* Item, int Index)
{
    if (!Item)
        mSlotList[Index]->SetTextureIcon(nullptr);

    else
        mSlotList[Index]->SetTextureIcon(Item->Icon);
}

void CInventory::ChangeConsumCategory()
{
    SetCategory(EItemCategory::Consumable);
}

void CInventory::ChangeHatCategory()
{
    SetCategory(EItemCategory::Hat);
}

void CInventory::ChangeQuestCategory()
{
    SetCategory(EItemCategory::Quest);
}

void CInventory::ChangeGrassCategory()
{
    SetCategory(EItemCategory::Grass);
}

void CInventory::SelectSlot(int Index)
{
    if (Index < 0 || Index >= mSlotList.size())
        return;

    // 이전에 선택했던 슬롯이 존재한다면.
    if (mSelectedSlotIndex >= 0 && mSelectedSlotIndex < mSlotList.size())
    {
        mSlotList[mSelectedSlotIndex]->SetSelected(false);
    }

    mSelectedSlotIndex = Index;
    mSlotList[Index]->SetSelected(true);

    // 해당 슬롯에서 아이템을 직접 가져옴
    FItemData* Item = mSlotList[Index]->GetItemData(); 

    if (Item)
        mSelectedItem = Item;
    else
        mSelectedItem = nullptr;

    // nullptr이면 자동으로 정보창 숨김
    if (mItemInfo)
        mItemInfo->SetItemInfo(Item); 
}
