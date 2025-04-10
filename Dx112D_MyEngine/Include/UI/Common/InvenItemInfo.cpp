#include "InvenItemInfo.h"

#include "../Common/Image.h"
#include "../Common/TextBlock.h"
#include "../Common/Button.h"
#include "../UserWidget/Slot.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"

CInvenItemInfo::CInvenItemInfo()
{
}
CInvenItemInfo::~CInvenItemInfo()
{
}

bool CInvenItemInfo::Init()
{
	mBack = mScene->GetUIManager()->CreateWidget<CImage>("InvenItemInfoBack");
	mBack->SetTexture("ItemInfoBack", TEXT("Texture\\UI\\BasicBox.png"));
	mBack->SetSize(500.f, 750.f);

	mSlot = mScene->GetUIManager()->CreateWidget<CSlot>("ItemInfoSlot");
	mSlot->SetSize(200.f, 200.f);
	mSlot->SetPos(150.f, 500.f);
	mSlot->SetIconSize(160.f, 160.f);
	mSlot->SetIconPos(20.f, 20.f);

	mItemInfo = mScene->GetUIManager()->CreateWidget<CTextBlock>("ItemInfo");
	mItemInfo->SetFontSize(18.f);
	mItemInfo->SetTextColor(0, 0, 0, 255);
	mItemInfo->SetText(TEXT("아이템 이름"));
	mItemInfo->SetSize(300.f, 200.f);
	mItemInfo->SetPos(110.f, 355.f);
	mItemInfo->SetAlignH(ETextAlignH::Center);
	

	
	
	mQuick1 = mScene->GetUIManager()->CreateWidget<CButton>("SetQuick1");
	mQuick1->SetTexture(EButtonState::Normal, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mQuick1->SetTexture(EButtonState::Hovered, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mQuick1->SetTexture(EButtonState::Click, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mQuick1->SetSize(250.f, 50.f);
	mQuick1->SetPos(130.f, 350.f);
	
	CSharedPtr<CTextBlock>    Text =
		mScene->GetUIManager()->CreateWidget<CTextBlock>("Quick1Text");

	Text->SetText(TEXT("퀵슬롯에 등록 (1 키)"));
	Text->SetTextColor(0, 0, 0, 255);
	Text->SetAlignH(ETextAlignH::Center);
	Text->SetFontSize(20.f);
	Text->SetPos(0.f, 5.f);
	mQuick1->SetChild(Text);

	mQuick2 = mScene->GetUIManager()->CreateWidget<CButton>("SetQuick2");
	mQuick2->SetTexture(EButtonState::Normal, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mQuick2->SetTexture(EButtonState::Hovered, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mQuick2->SetTexture(EButtonState::Click, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mQuick2->SetSize(250.f, 50.f);
	mQuick2->SetPos(130.f, 275.f);

	Text = mScene->GetUIManager()->CreateWidget<CTextBlock>("TestBtn");
	Text->SetText(TEXT("퀵슬롯에 등록 (2 키)"));
	Text->SetTextColor(0, 0, 0, 255);
	Text->SetAlignH(ETextAlignH::Center);
	Text->SetFontSize(20.f);
	Text->SetPos(0.f, 5.f);
	mQuick2->SetChild(Text);

	mQuick3 = mScene->GetUIManager()->CreateWidget<CButton>("SetQuick3");
	mQuick3->SetTexture(EButtonState::Normal, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mQuick3->SetTexture(EButtonState::Hovered, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mQuick3->SetTexture(EButtonState::Click, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mQuick3->SetSize(250.f, 50.f);
	mQuick3->SetPos(130.f, 200.f);

	Text = mScene->GetUIManager()->CreateWidget<CTextBlock>("TestBtn");
	Text->SetText(TEXT("퀵슬롯에 등록 (3 키)"));
	Text->SetTextColor(0, 0, 0, 255);
	Text->SetAlignH(ETextAlignH::Center);
	Text->SetFontSize(20.f);
	Text->SetPos(0.f, 5.f);
	mQuick3->SetChild(Text);

	mQuick4 = mScene->GetUIManager()->CreateWidget<CButton>("SetQuick4");
	mQuick4->SetTexture(EButtonState::Normal, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mQuick4->SetTexture(EButtonState::Hovered, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mQuick4->SetTexture(EButtonState::Click, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mQuick4->SetSize(250.f, 50.f);
	mQuick4->SetPos(130.f, 125.f);

	Text = mScene->GetUIManager()->CreateWidget<CTextBlock>("TestBtn");
	Text->SetText(TEXT("퀵슬롯에 등록 (4 키)"));
	Text->SetTextColor(0, 0, 0, 255);
	Text->SetAlignH(ETextAlignH::Center);
	Text->SetFontSize(20.f);
	Text->SetPos(0.f, 5.f);
	mQuick4->SetChild(Text);

	mItemUse = mScene->GetUIManager()->CreateWidget<CButton>("ItemUseBtn");
	mItemUse->SetTexture(EButtonState::Normal, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mItemUse->SetTexture(EButtonState::Hovered, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mItemUse->SetTexture(EButtonState::Click, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
	mItemUse->SetSize(250.f, 50.f);
	mItemUse->SetPos(130.f, 50.f);

	Text = mScene->GetUIManager()->CreateWidget<CTextBlock>("TestBtn");
	Text->SetText(TEXT("아이템 사용 (E 키)"));
	Text->SetTextColor(0, 0, 0, 255);
	Text->SetAlignH(ETextAlignH::Center);
	Text->SetFontSize(20.f);
	Text->SetPos(0.f, 5.f);
	mItemUse->SetChild(Text);

	AddWidget(mBack);
	AddWidget(mSlot);
	AddWidget(mItemInfo);
	AddWidget(mQuick1);
	AddWidget(mQuick2);
	AddWidget(mQuick3);
	AddWidget(mQuick4);
	AddWidget(mItemUse);

	mItemUse->SetEnable(false);
	mQuick1->SetEnable(false);
	mQuick2->SetEnable(false);
	mQuick3->SetEnable(false);
	mQuick4->SetEnable(false);



	return true;
}
void CInvenItemInfo::Update(float DeltaTime)
{
	CUserWidget::Update(DeltaTime);
}
void CInvenItemInfo::Render()
{
	CUserWidget::Render();
}

void CInvenItemInfo::SetItemInfo(FItemData* ItemData)
{
	// 만약 아이템 정보가 없을경우
	if (!ItemData)
	{
		// 아이템 설명을 아이템 정보가 없다고 설정
		mItemInfo->SetText(TEXT("아이템 정보 없음"));
		// 슬롯 아이콘 설정X
		mSlot->SetTextureIcon(nullptr);
		mSlot->SetEnableInfoTag(false);

		// 버튼 전부 비활성화.
		mQuick1->SetEnable(false);
		mQuick2->SetEnable(false);
		mQuick3->SetEnable(false);
		mQuick4->SetEnable(false);
		mItemUse->SetEnable(false);
		return;
	}

	mQuick1->SetEnable(false);
	mQuick2->SetEnable(false);
	mQuick3->SetEnable(false);
	mQuick4->SetEnable(false);
	mItemUse->SetEnable(false);

	// 아이템 정보가 있을 경우

	// 슬롯 아이콘 설정, 아이템 이름설정
	mSlot->SetTextureIcon(ItemData->TextureName);
	mSlot->SetInfoText(ToWString(ItemData->Name).c_str());
	mSlot->SetInfoTextBoxSize(196.f, 35.f);
	mSlot->SetTextPos(2.f, 2.f);
	mSlot->SetEnableInfoTag(true);
	
	// 아이템 설명 설정.
	mItemInfo->SetText(ToWString(ItemData->Desc).c_str());

	// 만약 아이템이 소모품이면
	if (ItemData->Category == EItemCategory::Consumable && ItemData->Useable)
	{
		// 버튼 전부 활성화.
		mQuick1->SetEnable(true);
		mQuick2->SetEnable(true);
		mQuick3->SetEnable(true);
		mQuick4->SetEnable(true);
		mItemUse->SetEnable(true);
	}

	// 만약 아이템이 장비면
	else if (ItemData->Equipable)
	{
		// 사용 버튼만 활성화.
		mItemUse->SetEnable(true);
	}
}
