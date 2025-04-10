#include "MainWidget.h"
#include "HeadInfo.h"
#include "Inventory.h"
#include "DialogueWidget.h"
#include "QuickSlotList.h"
#include "StoreWidget.h"
#include "QuestWidget.h"

#include "../Common/Button.h"
#include "../Common/TextBlock.h"
#include "../Common/ProgressBar.h"
#include "../Common/ResourceInfo.h"
#include "../Common/Image.h"
#include "../Common/RewardInfo.h"

#include "../../Scene/Scene.h"
#include "../../Scene/Input.h"
#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneManager.h"

#include "../../ScriptManager.h"

#include "../../Object/PlayerObject.h"
#include "../../Object/NPCObject.h"
#include "../../Object/ShopNPC.h"
#include "../../Object/NueBossMonster.h"

#include "../../Component/MovementComponent.h"
#include "../../Component/StoreComponent.h"
#include "../../Component/InventoryComponent.h"

#include "../../Quest/Quest.h"
#include "../../Quest/QuestManager.h"

#include "../../Device.h"
#include "../../Share/Timer.h"
#include "../../Share/Log.h"
#include "../../Asset/Texture/Texture.h"

#include "../../ItemManager.h"


CMainWidget::CMainWidget()
{
}

CMainWidget::~CMainWidget()
{
}

bool CMainWidget::Init()
{
    CUserWidget::Init();

    FResolution RS = CDevice::GetInst()->GetResolution();

    // 인벤토리 위젯
    mInventory = mScene->GetUIManager()->CreateWidget<CInventory>("Inventory");
    mInventory->SetZOrder(10);
    mInventory->SetEnable(false);

    AddWidget(mInventory);

    // HPBar 위젯
    mHPBarSymbol = mScene->GetUIManager()->CreateWidget<CImage>("PlayerHPBarSymbol");
    mHPBarSymbol->SetTexture("PlayerHPBarSymbol", TEXT("Texture\\UI\\HPBar\\Player_HPSymbol.png"));
    mHPBarSymbol->SetPos(30.f, RS.Height - 100.f);
    mHPBarSymbol->SetSize(40.f, 40.f);

    mHPBar = mScene->GetUIManager()->CreateWidget<CProgressBar>("PlayerHPBar");
    mHPBar->SetPos(70.f, RS.Height - 100.f);
    mHPBar->SetSize(200.f, 40.f);
    mHPBar->SetTexture(EProgressBarImageType::Frame, "PlayerHPBarFrame",
        TEXT("Texture\\UI\\HPBar\\Player_HPFrame.png"));
    mHPBar->SetTexture(EProgressBarImageType::Back, "PlayerHPBarBack",
        TEXT("Texture\\UI\\HPBar\\Player_HPBarBack.png"));
    mHPBar->SetTexture(EProgressBarImageType::Fill, "PlayerHPBarFill",
        TEXT("Texture\\UI\\HPBar\\Player_HPBarFill.png"));
    mHPBar->SetTint(EProgressBarImageType::Fill, 0.f, 160.f, 0.f);

    AddWidget(mHPBar);
    AddWidget(mHPBarSymbol);

    // 보스 HP 위젯
    mBossHPUI = mScene->GetUIManager()->CreateWidget<CHeadInfo>("BossHPUI");
    mBossHPUI->GetHPWidget()->SetPercent(1.f);
    mBossHPUI->SetPos(RS.Width * 0.5f - 400.f, RS.Height - 100.f);

    AddWidget(mBossHPUI);

    mBossHPUI->SetEnable(false);

    // 대화창 UI
    mDialogueUI = mScene->GetUIManager()->CreateWidget<CDialogueWidget>("DialogueUI");
    mDialogueUI->SetPos(100.f, 30.f);
    mDialogueUI->SetEnable(false);

    AddWidget(mDialogueUI);

    // 퀵슬롯
    mQuickSlots = mScene->GetUIManager()->CreateWidget<CQuickSlotList>("QuickSlotList");
    mQuickSlots->SetPos(40.f, (float)RS.Height - 180.f);

    AddWidget(mQuickSlots);
   
    // 상점 위젯
    mStore = mScene->GetUIManager()->CreateWidget<CStoreWidget>("StoreWidget");
    mStore->SetEnable(false);
    AddWidget(mStore);

    // 퀘스트 위젯
    mQuestUI = mScene->GetUIManager()->CreateWidget<CQuestWidget>("QuestWidget");
    mQuestUI->SetEnable(false);
    AddWidget(mQuestUI);

    // 보상 위젯
    mRewardUI = mScene->GetUIManager()->CreateWidget<CRewardInfo>("RewardWidget");
    mRewardUI->SetPos((float)RS.Width * 0.5f - 250.f, (float)RS.Height * 0.5f - 250.f);
    mRewardUI->SetEnable(false);
    AddWidget(mRewardUI);

    return true;
}

void CMainWidget::Update(float DeltaTime)
{
    CUserWidget::Update(DeltaTime);

    // 플레이어 HPBar가 활성화되어있으면 매 프레임 갱신
    if(mHPBar->IsEnable())
    {
        mPlayer = mScene->FindObjectFromType<CPlayerObject>();

        if (mPlayer)
        {
            float HPPercent = (float)mPlayer->GetHP() / (float)mPlayer->GetHPMax();
            mHPBar->SetPercent(HPPercent);
        }
    }

    // 보스 HPUI가 활성화 되어 있으면 매 프레임 갱신
    if (mBossHPUI->IsEnable())
    {
        mBoss = mScene->FindObjectFromType<CNueBossMonster>();

        if (mBoss)
        {
            float HPPercent = (float)mBoss->GetHP() / (float)mBoss->GetMaxHP();
            mBossHPUI->GetHPWidget()->SetPercent(HPPercent);
        }
    }  
}

void CMainWidget::Render()
{
    CUserWidget::Render();
}

void CMainWidget::SetPlayerHPPerCent()
{
}

void CMainWidget::SetBossHPPerCent()
{
}

void CMainWidget::SetEnableBossUI(bool Enable)
{
    mBossHPUI->SetEnable(Enable);
}

void CMainWidget::SetEnableDialogueUI(bool Enable)
{
    mDialogueUI->SetEnable(Enable);
}

void CMainWidget::SetDialgoueText(const TCHAR* Text)
{
    mDialogueUI->SetDialogueText(Text);
}

void CMainWidget::SetDialgoueNameTag(const TCHAR* Name)
{
    mDialogueUI->SetNameTagText(Name);
}

void CMainWidget::ShowScript(const FScriptLine* Line)
{
    SetEnableDialogueUI(true);
    SetDialgoueNameTag(ToWString(Line->NPCName).c_str());
    SetDialgoueText(ToWString(Line->Text).c_str());
}

void CMainWidget::ShowScript(int NPCID, int ScriptID)
{
    SetEnableDialogueUI(true);
    SetDialgoueNameTag(ToWString(CScriptManager::GetInst()
        ->FindScriptLine(NPCID, ScriptID)->NPCName).c_str());
    SetDialgoueText(ToWString(CScriptManager::GetInst()
        ->FindScriptLine(NPCID, ScriptID)->Text).c_str());

}


CQuickSlotList* CMainWidget::GetQuickSlot() const
{
    return mQuickSlots;
}

CInventory* CMainWidget::GetInventoryUI() const
{
    return mInventory;
}

CStoreWidget* CMainWidget::GetStoreUI() const
{
    return mStore;
}

CQuestWidget* CMainWidget::GetQuestUI() const
{
    return mQuestUI;
}

CRewardInfo* CMainWidget::GetRewardUI() const
{
    return mRewardUI;
}

CDialogueWidget* CMainWidget::GetDialogueUI() const
{
    return mDialogueUI;
}

bool CMainWidget::IsAnyUIOpened() const
{
    return GetDialogueUI()->IsEnable() ||
        GetInventoryUI()->IsEnable() ||
        GetQuestUI()->IsEnable() ||
        GetStoreUI()->IsEnable() ||
        GetRewardUI()->IsEnable();
}
