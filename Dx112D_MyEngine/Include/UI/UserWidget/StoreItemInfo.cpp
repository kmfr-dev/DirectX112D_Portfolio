#include "StoreItemInfo.h"
#include "../Common/Image.h"
#include "../Common/TextBlock.h"
#include "../Common/Button.h"
#include "../UserWidget/Slot.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"

CStoreItemInfo::CStoreItemInfo()
{
}

CStoreItemInfo::~CStoreItemInfo()
{
}

bool CStoreItemInfo::Init()
{
	mBack = mScene->GetUIManager()->CreateWidget<CImage>("StoreItemInfoBack");
	mBack->SetTexture("ItemInfoBack", TEXT("Texture\\UI\\BasicBox.png"));
	mBack->SetSize(500.f, 750.f);

	mSlot = mScene->GetUIManager()->CreateWidget<CSlot>("StoreInfoSlot");
	mSlot->SetSize(200.f, 200.f);
	mSlot->SetPos(150.f, 500.f);
	mSlot->SetIconSize(160.f, 160.f);
	mSlot->SetIconPos(20.f, 20.f);

	mItemInfo = mScene->GetUIManager()->CreateWidget<CTextBlock>("ItemInfo");
	mItemInfo->SetFontSize(20.f);
	mItemInfo->SetTextColor(0, 0, 0, 255);
	mItemInfo->SetText(TEXT(""));
	mItemInfo->SetSize(300.f, 200.f);
	mItemInfo->SetPos(100.f, 340.f);
	mItemInfo->SetAlignH(ETextAlignH::Center);

	mBuyBtn = mScene->GetUIManager()->CreateWidget<CButton>("ItemUseBtn");
	mBuyBtn->SetTexture(EButtonState::Normal, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mBuyBtn->SetTexture(EButtonState::Hovered, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mBuyBtn->SetTexture(EButtonState::Click, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mBuyBtn->SetSize(250.f, 50.f);
	mBuyBtn->SetPos(130.f, 75.f);

	CSharedPtr<CTextBlock> Text = mScene->GetUIManager()->CreateWidget<CTextBlock>("BuyBtn");
	Text->SetText(TEXT("아이템 구매 (F키)"));
	Text->SetTextColor(0, 0, 0, 255);
	Text->SetAlignH(ETextAlignH::Center);
	Text->SetFontSize(20.f);
	Text->SetPos(0.f, 5.f);
	mBuyBtn->SetChild(Text);

	AddWidget(mBack);
	AddWidget(mSlot);
	AddWidget(mItemInfo);
	AddWidget(mBuyBtn);
	
	mBuyBtn->SetEnable(false);

	return true;
}

void CStoreItemInfo::Update(float DeltaTime)
{
	CUserWidget::Update(DeltaTime);
}

void CStoreItemInfo::Render()
{
	CUserWidget::Render();
}

void CStoreItemInfo::SetItemInfo(FItemData* ItemData)
{
	mBuyBtn->SetEnable(false);

	// 아이템이 없으면
	// 구매버튼 비활성화 및 
	// 텍스트 설정, 슬롯 텍스처를 비운다.
	if (!ItemData)
	{
		mItemInfo->SetText(TEXT("아이템 정보 없음..."));
		mSlot->SetTextureIcon(nullptr);
		mSlot->SetEnableInfoTag(false);
		mBuyBtn->SetEnable(false);
		
		return;
	}

	// 아이템이 있으면
	else
	{
		// 아이템 정보 설정 및 구매버튼 활성화
		mItemInfo->SetText(ToWString(ItemData->Desc).c_str());
		mSlot->SetInfoText(ToWString(ItemData->Name).c_str());
		mSlot->SetInfoTextBoxSize(196.f, 40.f);
		mSlot->SetTextPos(2.f, 2.f);
		mSlot->SetTextureIcon(ItemData->TextureName);
		mSlot->SetEnableInfoTag(true);
		mBuyBtn->SetEnable(true);
	}
}
