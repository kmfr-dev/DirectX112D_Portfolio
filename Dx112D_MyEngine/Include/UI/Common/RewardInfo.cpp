#include "RewardInfo.h"
#include "Image.h"
#include "TextBlock.h"
#include "../UserWidget/Slot.h"
#include "../../Object/PlayerObject.h"
#include "../../Asset/Sound/SoundManager.h"
#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneAssetManager.h"
#include "../../Quest/Quest.h"
#include "../../ItemManager.h"
#include "../../Component/InventoryComponent.h"

CRewardInfo::CRewardInfo()
{
}

CRewardInfo::~CRewardInfo()
{
}

bool CRewardInfo::Init()
{
    CUserWidget::Init();

    mScene->GetAssetManager()->LoadTexture("GoldSymbol", TEXT("Texture\\UI\\GoldSymbol.png"));

    mBack = mScene->GetUIManager()->CreateWidget<CImage>("RewardBack");
    mBack->SetTexture("RewardBack", TEXT("Texture\\UI\\Basicbox.png"));
    mBack->SetSize(500.f, 500.f);

    mRewardTitle = mScene->GetUIManager()->CreateWidget<CTextBlock>("RewardTitle");
    mRewardTitle->SetTextColor(0, 0, 0, 255);
    mRewardTitle->SetFontSize(35.f);
    mRewardTitle->SetText(TEXT("보상"));
    mRewardTitle->SetSize(500.f, 50.f);
    mRewardTitle->SetAlignH(ETextAlignH::Center);
    mRewardTitle->SetPos(0.f, 400.f);

    mRewardSlot = mScene->GetUIManager()->CreateWidget<CSlot>("ReWardSlot");
    mRewardSlot->SetSize(200.f, 200.f);
    mRewardSlot->SetPos(150.f, 175.f);
    mRewardSlot->SetIconSize(180.f, 180.f);
    mRewardSlot->SetIconPos(10.f, 10.f);

    mRewardDesc = mScene->GetUIManager()->CreateWidget<CTextBlock>("RewardDesc");
    mRewardDesc->SetTextColor(0, 0, 0, 255);
    mRewardDesc->SetFontSize(25.f);
    mRewardDesc->SetText(TEXT(" 획득!"));
    mRewardDesc->SetSize(400.f, 50.f);
    mRewardDesc->SetPos(50.f, 100.f);
    mRewardDesc->SetAlignH(ETextAlignH::Center);


    AddWidget(mBack);
    AddWidget(mRewardTitle);
    AddWidget(mRewardSlot);
    AddWidget(mRewardDesc);
    
    return true;
}

void CRewardInfo::Update(float DeltaTime)
{
    CUserWidget::Update(DeltaTime);
}

void CRewardInfo::Render()
{
    CUserWidget::Render();
}

void CRewardInfo::SetRewardText(const TCHAR* Text)
{
    mRewardDesc->SetText(Text);
}

void CRewardInfo::SetTextureIcon(CTexture* Texture)
{
    mRewardSlot->SetTextureIcon(Texture);
}

void CRewardInfo::SetTextureIcon(const std::string& Name)
{
    mRewardSlot->SetTextureIcon(Name);
}

void CRewardInfo::SetTextureIcon(const std::string& Name,
    const TCHAR* FileName)
{
    mRewardSlot->SetTextureIcon(Name, FileName);
}

void CRewardInfo::OpenRewardUI(CQuest* Quest)
{

    mRewardQuest = Quest;
    mRewardedGold = false;
    mRewardedItem = false;

    SetEnable(true);
    ShowNextReward();
}

void CRewardInfo::ShowNextReward()
{
    // 만약 아직 골드 보상을 받지 않았고 보상 골드가 0 이상이면
    // 퀘스트 골드 보상을 지급한다.
    if (!mRewardedGold && mRewardQuest->GetRewardGold() > 0)
    {
        // UI 표시
        mRewardSlot->SetTextureIcon("GoldSymbol");
        mRewardDesc->SetText(ToWString(std::to_string(mRewardQuest->GetRewardGold())
            + " Gold 획득!").c_str());

        // 실제 플레이어에게 골드를 지급한다.
        CPlayerObject* Player = mScene->FindObjectFromType<CPlayerObject>();
        if (Player)
            Player->AddGold(mRewardQuest->GetRewardGold());

        // 골드 보상을 지급했으므로 true로 변경
        mRewardedGold = true;

        mScene->GetAssetManager()->FindSound("AddItem")->Play();
        return;

    }

    // 만약 아직 아이템 보상을 지급하지 않았고,
    // 아이템의 이름과 갯수가 있다면
    // 아이템 보상을 지급한다.
    if (!mRewardedItem &&
        mRewardQuest->GetRewardItem().ItemName != "" &&
        mRewardQuest->GetRewardItem().Count != 0)
    {
        const FItemData* const Item = CItemManager::GetInst()->
            FindItem(mRewardQuest->GetRewardItem().ItemName);
    
        // 보상 슬롯에 텍스처 지정 및
        // 보상 지급 텍스트 세팅
        mRewardSlot->SetTextureIcon(Item->TextureName);
        mRewardDesc->SetText(ToWString(mRewardQuest->GetRewardItem().ItemName +
            " x" + std::to_string(mRewardQuest->GetRewardItem().Count) + " 획득!").c_str());

        // 실제 플레이어에게 아이템을 지급한다.
        CPlayerObject* Player = mScene->FindObjectFromType<CPlayerObject>();
        if (Player)
        {
            for (int i = 0; i < mRewardQuest->GetRewardItem().Count; ++i)
            {
                FItemData* RewardItem = CItemManager::GetInst()
                    ->CloneItem(mRewardQuest->GetRewardItem().ItemName);

                if (RewardItem)
                    Player->GetInventory()->AddItem(RewardItem);
            }
        }

        mRewardedItem = true;
        mScene->GetAssetManager()->FindSound("AddItem")->Play();
        return;
    }

    SetEnable(false);
    mScene->GetAssetManager()->FindSound("UIClose")->Play();
}
