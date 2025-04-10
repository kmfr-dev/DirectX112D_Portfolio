#include "Quest.h"
#include "../Asset/Texture/Texture.h"

CQuest::CQuest()
{

}
CQuest::CQuest(const CQuest& Quest)
{
    mQuestID = Quest.mQuestID;
    mQuestTitle = Quest.mQuestTitle;
    mDescription = Quest.mDescription;
    mQuestGoal = Quest.mQuestGoal; 
    mQuestState = EQuestState::NotAccepted;
   /* mRewardGold = Quest.mRewardGold;*/
}
CQuest::CQuest(CQuest&& Quest)
{

}
CQuest::~CQuest()
{

}

void CQuest::AddProgress()
{
    // 만약 퀘스트가 진행중이 아니라면 
    // 퀘스트의 현재 수치를 올려주면 안됨
    if (mQuestState != EQuestState::Progress)
        return;

    if (mQuestGoal.CurrentCount < mQuestGoal.TargetCount)
        ++mQuestGoal.CurrentCount;

    CheckComplete();
}
