#include "StoreWidget.h"
#include "../Common/Image.h"
#include "../Common/ResourceInfo.h"
#include "../Common/Button.h"
#include "../Common/TextBlock.h"
#include "StoreItemInfo.h"

#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneAssetManager.h"
#include "../../Asset/Texture/Texture.h"
#include "../../Device.h"


CStoreWidget::CStoreWidget()
{
}

CStoreWidget::~CStoreWidget()
{
}

bool CStoreWidget::Init()
{
    CWindowWidget::Init();

    FResolution RS = CDevice::GetInst()->GetResolution();

    SetSize((float)RS.Width, (float)RS.Height);

    mStoreBack = mScene->GetUIManager()->CreateWidget<CImage>("StoreBack");
    mStoreBack->SetTexture("StoreBack", TEXT("Texture\\UI\\BasicBox.png"));
    mStoreBack->SetSize((float)RS.Width, (float)RS.Height);
    AddWidget(mStoreBack);

    mStoreText = mScene->GetUIManager()->CreateWidget <CTextBlock>("StoreText");
    mStoreText->SetText(TEXT("상점"));
    mStoreText->SetFontSize(40.f);
    mStoreText->SetSize(100.f, 40.f);
    mStoreText->SetAlignH(ETextAlignH::Center);
    mStoreText->SetTextColor(0, 0, 0, 255);
    mStoreText->SetPos((float)RS.Width * 0.5f - 100.f, (float)RS.Height - 120.f);

    AddWidget(mStoreText);

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
    mGoldInfo->SetPos(RS.Width - 350.f, RS.Height - 90.f);

    AddWidget(mGoldInfo);

    // 슬롯목록 오른쪽에 표시될 아이템 정보
    mItemInfo = mScene->GetUIManager()->CreateWidget<CStoreItemInfo>("StroeItemInfo");
    mItemInfo->SetPos(RS.Width - 600.f, 50.f);
    mItemInfo->SetEnable(true);
    AddWidget(mItemInfo);

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

            Slot->SetSlotType(ESlotType::Store);
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

void CStoreWidget::Update(float DeltaTime)
{
    CWindowWidget::Update(DeltaTime);
}

void CStoreWidget::Render()
{
    CWindowWidget::Render();
}


void CStoreWidget::SetGoldInfo(int Gold)
{
    std::wstring GoldInfo = ToWString(std::to_string(Gold).c_str());
    mGoldInfo->SetInfoText(GoldInfo.c_str());
}

void CStoreWidget::SetItemList(const std::vector<FItemData*>& ItemList)
{
    for (int i = 0; i < mSlotList.size(); ++i)
    {
        if (i < ItemList.size())
        {
            mSlotList[i]->SetItemData(ItemList[i]);
            StoreChange(ItemList[i], i);
        }
        else
        {
            mSlotList[i]->SetItemData(nullptr);
            StoreChange(nullptr, i);
        }
    }

   /* mSelectedItem = nullptr;
    mItemInfo->SetEnable(false);*/

    // 만약 아이템 리스트가 있으면
    // 기본적으로 첫번째 슬롯으로 선택
    if (!ItemList.empty())
        SelectSlot(0);
}

void CStoreWidget::StoreChange(const FItemData* Item, int Index)
{
    if (!Item)
        mSlotList[Index]->SetTextureIcon(nullptr);

    else
        mSlotList[Index]->SetTextureIcon(Item->Icon);
}

void CStoreWidget::SelectSlot(int Index)
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

    // nullptr이면 자동으로 정보창 숨김
    if (mItemInfo)
    {
        mItemInfo->SetItemInfo(Item);
        mItemInfo->SetEnable(true);
    }
}
