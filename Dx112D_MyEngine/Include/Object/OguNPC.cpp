#include "OguNPC.h"
#include "PlayerObject.h"
#include "../Component/SpriteComponent.h"
#include "../Component/ColliderSphere2D.h"
#include "../Animation/Animation2D.h"

#include "../Scene/Scene.h"

#include "../Quest/QuestManager.h"
#include "../Asset/Texture/Texture.h"


COguNPC::COguNPC()
{
}

COguNPC::COguNPC(const COguNPC& Obj)
{
}

COguNPC::COguNPC(COguNPC&& Obj)
{
}

COguNPC::~COguNPC()
{
}

bool COguNPC::Init()
{
    CNPCObject::Init();

    mNPCName = "BigOguNPC";
    mNPCID = 1;
    mCurrentScriptID = 1;

    mRoot->SetWorldPos(1000.f, 700.f, 0.f);
    mRoot->SetWorldScale(410.f, 410.f, 1.f);
    mRoot->SetPivot(0.435f, 0.5f);
    mBody->SetRadius(90.f);

    mQuestMark->SetRelativeScale(0.3f, 0.3f);
    mQuestMark->SetRelativePos(0.f, 160.f);

    mAnimation->AddSequence("BigOguNPC", 4.f, 1.f, true, false);

    CPlayerObject* Player = mScene->FindObjectFromType<CPlayerObject>();
    if (Player && !Player->IsQuestCompleted(1))
        AddQuestID(1);
    if (Player && Player->IsQuestCompleted(1))
        AddQuestID(2);
    if (Player && Player->IsQuestCompleted(2))
        AddQuestID(3);

    // NPC가 생성됐을 때 퀘스트 상태에 따른 마크 처리
    EQuestState State =
        CQuestManager::GetInst()->GetNPCQuestMarkState(GetQuestIDList(), Player);

    // 만약 플레이어가 수락전이라면
    // 느낌표 애니메이션 

    switch (State)
    {
    case EQuestState::NotAccepted:
        ChangQuesteAnimation("QuestMark1");
        SetQuestMarkEnable(true);
        break;
    case EQuestState::Progress:
    case EQuestState::Complete:
        ChangQuesteAnimation("QuestMark2");
        SetQuestMarkEnable(true);
        break;
    case EQuestState::None:
        SetQuestMarkEnable(false);
    }

    return true;
}

void COguNPC::Update(float DeltaTime)
{
    CNPCObject::Update(DeltaTime);
}

void COguNPC::Render()
{
    CNPCObject::Render();
}
