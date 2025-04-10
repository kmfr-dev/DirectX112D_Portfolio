#include "NPCObject.h"
#include "../Object/TileMapObj.h"
#include "../Component/SpriteComponent.h"
#include "../Component/ColliderSphere2D.h"
#include "../Animation/Animation2D.h"

#include "../Scene/Scene.h"

#include "PlayerObject.h"
#include "ObjectData/PlayerData.h"

#include "../Quest/Quest.h"
#include "../Quest/QuestManager.h"
#include "../Asset/Texture/Texture.h"

CNPCObject::CNPCObject()
{
}
CNPCObject::CNPCObject(const CNPCObject& Obj)
{
}
CNPCObject::CNPCObject(CNPCObject&& Obj)
{
}
CNPCObject::~CNPCObject()
{
}


void CNPCObject::ChangQuesteAnimation(const std::string& Name)
{
    mQuestMarkAnim->ChangeAnimation(Name);
}

bool CNPCObject::Init()
{
	mRoot = CreateComponent<CSpriteComponent>("NPC");
    mRoot->SetPivot(0.5f, 0.5f);
    mAnimation = mRoot->CreateAnimation2D<CAnimation2D>();

    SetRootComponent(mRoot);

    mBody = CreateComponent<CColliderSphere2D>();
    mBody->SetCollisionProfile("NPC");

    mBody->SetCollisionBeginFunc<CNPCObject>(this,
        &CNPCObject::CollisionNPCBegin);
    mBody->SetCollisionEndFunc<CNPCObject>(this,
        &CNPCObject::CollisionNPCEnd);

    mRoot->AddChild(mBody);

    mQuestMark = CreateComponent<CSpriteComponent>("QuestMark");
    mRoot->AddChild(mQuestMark);

    mQuestMark->SetPivot(0.5f, 0.5f);

    mQuestMarkAnim = mQuestMark->CreateAnimation2D<CAnimation2D>();
    mQuestMarkAnim->AddSequence("QuestMark1", 1.f, 1.f, true, false);
    mQuestMarkAnim->AddSequence("QuestMark2", 1.f, 1.f, true, false);

    mQuestMark->SetEnable(true);

	return true;
}

void CNPCObject::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);
}

void CNPCObject::Render()
{
	CSceneObject::Render();
}

int CNPCObject::GetStartScriptID() const
{
    CPlayerObject* Player = mScene->FindObjectFromType<CPlayerObject>();

    if (!Player)
        return 1;

    bool HasProgress = false;
    bool HasNotAccepted = false;

    for (int QuestID : mQuestIDList)
    {
        CQuest* PlayerQuest = Player->FindQuestByID(QuestID);

        if (!PlayerQuest)
        {
            HasNotAccepted = true;
            continue;
        }

        if (PlayerQuest->GetQuestState() == EQuestState::Complete)
        {
            return 1;
        }

        if (PlayerQuest->GetQuestState() == EQuestState::Progress)
        {
            if (QuestID == 3)
                return 350;

            HasProgress = true;
        }
    }

    if (HasProgress)
        return 200;

    if (HasNotAccepted)
    {
        // 우선순위: 퀘스트ID가 1이면 근접몬스터, 2면 원거리몬스터, 3이면 보스
        for (int QuestID : mQuestIDList)
        {
            CQuest* PlayerQuest = Player->FindQuestByID(QuestID);
            if (!PlayerQuest)
            {
                if (QuestID == 1)
                    return 50; // 근접몬스터 미수락
                else if (QuestID == 2)
                    return 100; // 원거리몬스터 미수락
                else if (QuestID == 3)
                    return 300; // 보스퀘스트 미수락
            }
        }
        return 100; // 기본 미수락
    }

    return 1; // 기본 대사
}

void CNPCObject::SetQuestMarkEnable(bool Flag)
{
    mQuestMark->SetEnable(Flag);
}

void CNPCObject::SetTileMap(CSharedPtr<class CTileMapObj> Map)
{
	mTileMap = Map;
}

void CNPCObject::AddQuestID(int ID)
{
    // 플레이어가 이 퀘스트를 완료했는지 체크
    CPlayerObject* Player = mScene->FindObjectFromType<CPlayerObject>();
    if (!Player)
        return;

    // 이미 완료한 퀘스트는 등록하지 않음
    if (Player->IsQuestCompleted(ID))
        return; 

    mQuestIDList.push_back(ID);
}

void CNPCObject::CollisionNPCBegin(const FVector3D& HitPoint,
    CColliderBase* Dest)
{

}

void CNPCObject::CollisionNPCEnd(CColliderBase* Dest)
{
}
