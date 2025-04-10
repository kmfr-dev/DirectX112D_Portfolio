#include "QuestWidget.h"
#include "QuestInfo.h"

#include "../Common/Image.h"
#include "../Common/TextBlock.h"
#include "../Common/Button.h"
#include "../Common/QuestTag.h"

#include "../UserWidget/Slot.h"

#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneAssetManager.h"
#include "../../Asset/Sound/SoundManager.h"

#include "../../Quest/Quest.h"
#include "../../Quest/QuestManager.h"
#include "../../Device.h"
#include "../../Asset/Texture/Texture.h"

CQuestWidget::CQuestWidget()
{
}

CQuestWidget::~CQuestWidget()
{
}

bool CQuestWidget::Init()
{
    CWindowWidget::Init();

    FResolution RS = CDevice::GetInst()->GetResolution();

    mQuestBack = mScene->GetUIManager()->CreateWidget<CImage>("QuestBack");
    mQuestBack->SetTexture("QuestBack", TEXT("Texture\\UI\\BasicBox.png"));
    mQuestBack->SetSize((float)RS.Width, (float)RS.Height);

    mQuestText = mScene->GetUIManager()->CreateWidget<CTextBlock>("QuestTitle");
    mQuestText->SetSize((float)RS.Width, 50.f);
    mQuestText->SetPos(0.f, (float)RS.Height - 100.f);
    mQuestText->SetAlignH(ETextAlignH::Center);
    mQuestText->SetTextColor(0, 0, 0, 255);
    mQuestText->SetText(TEXT("퀘스트 정보"));
    mQuestText->SetFontSize(30.f);

    mQuestInfo = mScene->GetUIManager()->CreateWidget<CQuestInfo>("QuestInfo");
    mQuestInfo->SetPos(RS.Width - 800.f, 50.f);

    AddWidget(mQuestBack);
    AddWidget(mQuestText);
    AddWidget(mQuestInfo);

    mQuestList.resize(6);
    
    for (int i = 0; i < 6; ++i)
    {
        std::string QuestTagName = 
            "QuestSlot" + std::to_string(i);

        CQuestTag* QuestTag = 
            mScene->GetUIManager()->CreateWidget<CQuestTag>(QuestTagName);

        QuestTag->SetSize(670.f, 70.f);
        QuestTag->SetPos(100.f, RS.Height - (200.f + i * 110.f));
        QuestTag->SetIndex(i);
        mQuestList[i] = QuestTag;
        mQuestList[i]->SetClickCallback([this, i]()
            {
                SelectQuest(i);
            });


        AddWidget(QuestTag);
    }


    return true;
}

void CQuestWidget::Update(float DeltaTime)
{
    CWindowWidget::Update(DeltaTime);
}

void CQuestWidget::Render()
{
    CWindowWidget::Render();
}

void CQuestWidget::SetQuestEnableBtn(bool Flag)
{
    mQuestInfo->SetEnableBtn(Flag);
}

void CQuestWidget::SetQuestTagEnable(bool Flag)
{
    mQuestInfo->SetEnableProgress(Flag);
}


void CQuestWidget::ClearQuestSlots()
{
    for (int i = 0; i < mQuestList.size(); ++i)
    {
        mQuestList[i]->SetNameTag(TEXT(""));
        mQuestList[i]->SetTint(255.f, 255.f, 255.f);
        mQuestList[i]->SetQuest(nullptr);
    }

    mSelectedQuest = nullptr;
    mSelectedSlotIndex = -1;
    mQuestInfo->SetQuestInfo(nullptr);
}

bool CQuestWidget::IsEnableAcceptedBtn()
{  
    return mQuestInfo->mAcceptedBtn->IsEnable();
}

void CQuestWidget::OpenQuestUI(const std::list<CQuest*>& QuestList)
{
    // 퀘스트 창 활성화 시키기 이전 
    // 퀘스트 목록 UI 텍스트 초기화
    ClearQuestSlots();

    // 만약 보여줄 퀘스트 목록이 있으면
    // 퀘스트 정보를 갱신해서 보여줘야 한다.
    if (!QuestList.empty())
    {
        int Index = 0;

        auto iter = QuestList.begin();
        auto iterEnd = QuestList.end();

        for (; iter != iterEnd; ++iter)
        {
            CQuest* Quest = *iter;

            if (!Quest)
                continue;

            mQuestList[Index]->SetNameTag(ToWString(Quest->GetQuestTitle()).c_str());
            mQuestList[Index]->SetQuest(Quest);
            mQuestInfo->SetQuestInfo(Quest);

            ++Index;
        }
        // 만약 퀘스트가 1개이상이면
        // 0번 슬롯을 셀렉션 한다.
        if (Index > 0)
            SelectQuest(0);
    }

    // 퀘스트 UI창 활성화
    SetEnable(true);
    mScene->GetAssetManager()->FindSound("UIOpen")->Play();
}

void CQuestWidget::SelectQuest(int Index)
{
    if (Index < 0 || Index >= mQuestList.size())
        return;

    // 이전 선택 슬롯 해제
    if (mSelectedSlotIndex >= 0 && mSelectedSlotIndex < mQuestList.size())
        mQuestList[mSelectedSlotIndex]->SetSelected(false);

    // 새로운 선택 슬롯 설정
    mSelectedSlotIndex = Index;
    mQuestList[mSelectedSlotIndex]->SetSelected(true);
    mSelectedQuest = mQuestList[mSelectedSlotIndex]->GetQuest();

    // 만약 선택했을 때 퀘스트 슬롯에 퀘스트가 있으면
    if (mSelectedQuest)
    {
        // 퀘스트 슬롯에 현재 선택한 퀘스트 정볼르 설정해준다.
        // Tint도 초록색으로 변경
        mQuestInfo->SetQuestInfo(mSelectedQuest);
        mQuestList[mSelectedSlotIndex]->SetTint(0, 255.f, 0);

        // 만약 퀘스트가 진행중이면 수락버튼 비활성화
        if (mSelectedQuest->GetQuestState() == EQuestState::Progress)
            mQuestInfo->SetEnableBtn(false);

        // 만약 퀘스트를 수락하지 않았다면 수락버튼 활성화.
        else if (mSelectedQuest->GetQuestState() == EQuestState::NotAccepted)
            mQuestInfo->SetEnableBtn(true);/*
        else if(mSelectedQuest->GetQuestState() == EQuestState::Completed)*/

           
    }

    // 만약 선택한 퀘스트 슬롯에 퀘스트가 없으면
    else
    {
        // 퀘스트 정보를 기본설정으로 변경.
        mQuestInfo->SetQuestInfo(nullptr);
        // 퀘스트 수락버튼 비활성화.
        mQuestInfo->SetEnableBtn(false);
    }
    
}
