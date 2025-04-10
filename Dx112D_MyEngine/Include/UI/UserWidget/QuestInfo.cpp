#include "QuestInfo.h"
#include "../Common/Image.h"
#include "../Common/TextBlock.h"
#include "../Common/Button.h"
#include "../Common/QuestTag.h"

#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"

#include "../../Quest/Quest.h"
#include "../../Asset/Texture/Texture.h"

CQuestInfo::CQuestInfo()
{
}

CQuestInfo::~CQuestInfo()
{
}

bool CQuestInfo::Init()
{
    mBack = mScene->GetUIManager()->CreateWidget<CImage>("QuestInfoBack");
    mBack->SetTexture("QuestInfoBack", TEXT("Texture\\UI\\BasicBox.png"));
    mBack->SetSize(700.f, 750.f);
    
    mQuestTitle = mScene->GetUIManager()->CreateWidget<CTextBlock>("QuestTitle");
    mQuestTitle->SetText(TEXT("퀘스트 제목"));
    mQuestTitle->SetAlignH(ETextAlignH::Center);
    mQuestTitle->SetTextColor(0, 0, 0, 255);
    mQuestTitle->SetFontSize(25.f);
    mQuestTitle->SetSize(700.f, 50.f);
    mQuestTitle->SetPos(0.f, mBack->GetSize().y - 100.f);

    mQuestDesc = mScene->GetUIManager()->CreateWidget<CTextBlock>("QuestDesc");
    mQuestDesc->SetText(TEXT("퀘스트 설명...."));
    mQuestDesc->SetAlignH(ETextAlignH::Center);
    mQuestDesc->SetTextColor(0, 0, 0, 255);
    mQuestDesc->SetFontSize(23.f);
    mQuestDesc->SetSize(600.f, 300.f);
    mQuestDesc->SetPos(50.f, mBack->GetSize().y - 325.f);

    mQuestTag = mScene->GetUIManager()->CreateWidget<CQuestTag>("QuestProgress");
    mQuestTag->SetSize(200.f, 50.f);
    mQuestTag->SetPos(400.f, 50.f);
    mQuestTag->SetTint(255.f, 255.f, 255.f);

    mAcceptedBtn = mScene->GetUIManager()->CreateWidget<CButton>("QuestBtn");
    mAcceptedBtn->SetTexture(EButtonState::Normal, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
    mAcceptedBtn->SetTexture(EButtonState::Hovered, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
    mAcceptedBtn->SetTexture(EButtonState::Click, "InvenBtn", TEXT("Texture\\UI\\InteracBtn.png"));
    mAcceptedBtn->SetSize(250.f, 50.f);
    mAcceptedBtn->SetPos(230.f, 75.f);

    mBtnText = mScene->GetUIManager()->CreateWidget<CTextBlock>("QuestBtnText");
    mBtnText->SetText(TEXT("퀘스트 수락 (F키)"));
    mBtnText->SetTextColor(0, 0, 0, 255);
    mBtnText->SetAlignH(ETextAlignH::Center);
    mBtnText->SetFontSize(20.f);
    mBtnText->SetSize(250.f, 50.f);
    mBtnText->SetPos(230.f, 75.f);
    mAcceptedBtn->SetChild(mBtnText);

    AddWidget(mBack);
    AddWidget(mQuestTitle);
    AddWidget(mQuestDesc);
    AddWidget(mQuestTag);
    AddWidget(mAcceptedBtn);
    AddWidget(mBtnText);

    return true;
}

void CQuestInfo::Update(float DeltaTime)
{
    CUserWidget::Update(DeltaTime);
}

void CQuestInfo::Render()
{
    CUserWidget::Render();
}

void CQuestInfo::SetEnableBtn(bool Flag)
{
    mAcceptedBtn->SetEnable(Flag);
    mBtnText->SetEnable(Flag);
}

void CQuestInfo::SetEnableProgress(bool Flag)
{
    mQuestTag->SetEnable(Flag);
}

void CQuestInfo::SetQuestProgress(int CurrentCount, int TargetCount)
{
    mQuestTag->SetText(ToWString("진행도 : " + std::to_string(CurrentCount) 
        + " / " + std::to_string(TargetCount)).c_str());
}

void CQuestInfo::SetQuestInfo(CQuest* Quest)
{
    // 만약 퀘스트가 없다면
    // 퀘스트 정보를 없다고 표시 해준다.
    if (!Quest)
    {
        mQuestTitle->SetText(TEXT("퀘스트 제목"));
        mQuestDesc->SetText(TEXT("현재 퀘스트가 없음..."));
        mQuestTag->SetEnable(false);
        mAcceptedBtn->SetEnable(false);
        mBtnText->SetEnable(false);
    }

    // 만약 퀘스트가 있다면
    // 퀘스트 제목과 설명을 설정해준다.
    else
    {
        mQuestTitle->SetText(ToWString(Quest->GetQuestTitle()).c_str());
        mQuestDesc->SetText(ToWString(Quest->GetQuestDesc()).c_str());
        

        switch (Quest->GetQuestState())
        {
        case EQuestState::NotAccepted:
            mBtnText->SetText(TEXT("퀘스트 수락 (F키)"));
            SetEnableProgress(false);
            SetEnableBtn(true);
            break;
        case EQuestState::Progress:
            SetQuestProgress(Quest->GetQuestGoal().CurrentCount,
                Quest->GetQuestGoal().TargetCount);
            SetEnableProgress(true);
            SetEnableBtn(false);
            break;
        case EQuestState::Complete:
            mBtnText->SetText(TEXT("퀘스트 완료 (F키)"));
            SetQuestProgress(Quest->GetQuestGoal().CurrentCount,
                Quest->GetQuestGoal().TargetCount);
            SetEnableProgress(false);
            SetEnableBtn(true);
            break;
        }

        // 만약 조건을 채워 완료가 됐지만
        // 완전히 퀘스트를 완료하지 않았을 경우 진행도는 보여줘야한다.
        if (Quest->GetQuestState() == EQuestState::Complete &&
            Quest->GetQuestState() != EQuestState::Completed)
        {
            SetEnableProgress(true);
        }
    }
}

