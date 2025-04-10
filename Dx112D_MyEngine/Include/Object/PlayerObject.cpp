#include "PlayerObject.h"
#include "SpriteEffect.h"
#include "TileMapObj.h"
#include "MonsterObject.h"
#include "BulletObject.h"
#include "BossBullet.h"
#include "NPCObject.h"
#include "ShopNPC.h"

#include "../Scene/SceneAssetManager.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Texture/Texture.h"
#include "../Asset/Texture/TextureManager.h"
#include "../Asset/Sound/SoundManager.h"

#include "../Component/PlayerMovement.h"
#include "../Component/RotationComponent.h"
#include "../Component/ColliderAABB2D.h"
#include "../Component/ColliderSphere2D.h"
#include "../Component/ColliderLine2D.h"
#include "../Component/SpriteComponent.h"
#include "../Component/InventoryComponent.h"
#include "../Component/StateMachineComponent.h"
#include "../Component/State/Player/PlayerIdleState.h"
#include "../Component/State/Player/PlayerWalkState.h"
#include "../Component/WidgetComponent.h"
#include "../Component/StoreComponent.h"

#include "../UI/UserWidget/HeadInfo.h"
#include "../UI/Common/ProgressBar.h"
#include "../UI/UserWidget/MainWidget.h"
#include "../UI/UserWidget/QuickSlotList.h"
#include "../UI/UserWidget/Inventory.h"
#include "../UI/UserWidget/DialogueWidget.h"
#include "../UI/UserWidget/StoreWidget.h"
#include "../UI/UserWidget/QuestWidget.h"
#include "../UI/Common/RewardInfo.h"

#include "../Scene/Scene.h"
#include "../Scene/Input.h"
#include "../Scene/SceneAssetManager.h"
#include "../Scene/SceneUIManager.h"

#include "../Animation/Animation2D.h"
#include "../Share/Log.h"
#include "../Device.h"

#include "../PlayerManager.h"
#include "../Quest/Quest.h"
#include "../Quest/QuestManager.h"
#include "../ItemManager.h"
#include "../ScriptManager.h"
#include "../ProfileManager.h"

CPlayerObject::CPlayerObject()
{
}
CPlayerObject::CPlayerObject(const CPlayerObject& Obj)  :
    CSceneObject(Obj)
{
}
CPlayerObject::CPlayerObject(CPlayerObject&& Obj) :
    CSceneObject(Obj)
{
}
CPlayerObject::~CPlayerObject()
{
    FVector2D pos;
    pos.x = GetWorldPosition().x;
    pos.y= GetWorldPosition().y;
    mPlayerData->Pos = pos;
   
    CPlayerManager::GetInst()->GetPlayerData()->
        SetInventoryItems(mInventory->GetItemList());
    CPlayerManager::GetInst()->Save();
}

bool CPlayerObject::Init()
{
    mRoot = CreateComponent<CSpriteComponent>("Player");
   /* mRoot->SetPivot(0.5f, 0.45f);
    mRoot->SetWorldScale(150.f, 150.f, 1.f);*/

    SetRootComponent(mRoot);
    mBody = CreateComponent<CColliderAABB2D>();
    mBody->SetCollisionProfile("Player");
    
    mRoot->AddChild(mBody);

    mAttackRange = CreateComponent<CColliderSphere2D>();
    mAttackRange->SetCollisionProfile("PlayerAttack");
    mAttackRange->SetEnable(false);
    mAttackRange->SetRadius(45.f);
    mRoot->AddChild(mAttackRange);

    mRotationPivot = CreateComponent<CSceneComponent>();
    mRoot->AddChild(mRotationPivot);

    mMovement = CreateComponent<CPlayerMovement>();
    mMovement->SetUpdateComponent(mRoot);
    mMovement->SetMoveSpeed(600.f);

    mAnimation = mRoot->CreateAnimation2D<CAnimation2D>();

    mInventory = CreateComponent<CInventoryComponent>();

    mStateMachine = CreateComponent<CStateMachineComponent>();
    mStateMachine->InitStates(EObjectType::Player);

    mBody->SetCollisionBeginFunc<CPlayerObject>(this,
        &CPlayerObject::CollisionPlayer);
    mBody->SetCollisionEndFunc<CPlayerObject>(this,
        &CPlayerObject::CollisionPlayerEnd);
    
    mAttackRange->SetCollisionBeginFunc<CPlayerObject>(this,
        &CPlayerObject::CollisionPlayerAttack);

    InitAnimAddSequence();
    InitAnimBindFunction();
    InitBindKey();
   
    mPlayerData = CPlayerManager::GetInst()->GetPlayerData();

    ApplyData();

    return true;
}

bool CPlayerObject::InitAfter()
{
    mMovement->SetEnable(true);
    mPlayerData->CanMove = true;
    mPlayerData->Hitting = false;
    mPlayerData->CanAttack = true;
    mPlayerData->Invincible = false;
    mPlayerData->Attacking = false;
    mPlayerData->CanSpeak = false;
    mPlayerData->Dead = false;
    mPlayerData->Running = false;
    mPlayerData->Moving = false;
    mPlayerData->Speed = 450.f;

    

    return true;
}

void CPlayerObject::PreUpdate(float DeltaTime)
{
    CSceneObject::PreUpdate(DeltaTime);

    /*if (mCurrentDir == EObjectDir::End)
        mCurrentDir = mPrevDir;*/
}

void CPlayerObject::Update(float DeltaTime)
{
    CSceneObject::Update(DeltaTime);

    if (mRoot->IsEnable())
    {
        int a = 5;
    }

    mStateMachine->Update(DeltaTime);
  
    ConstrainToMap();
}

void CPlayerObject::Render()
{
    CSceneObject::Render();
}

void CPlayerObject::Damage(int Dmg)
{
    mPlayerData->HP -= Dmg;
}
float CPlayerObject::Damage(float Attack, CSceneObject* Obj)
{
    Attack = CSceneObject::Damage(Attack, Obj);

    mPlayerData->HP -= (int)Attack;

    return Attack;
}

void CPlayerObject::EndFrame()
{
    CSceneObject::EndFrame();
}

void CPlayerObject::ApplyData()
{

    if(mPlayerData->GameType == EGameType::NewGame ||
        mPlayerData->GameType == EGameType::LoadGame)
    {
        mRoot->SetWorldPos(mPlayerData->Pos);
    }

    else if(mPlayerData->GameType == EGameType::MoveNextScene)
    {
        mRoot->SetWorldPos(mPlayerData->NextSpawnPos);
    }

    mStateMachine->ChangeState(EObjectState::Idle, mPlayerData->Dir);
    mRoot->SetWorldScale(mPlayerData->Scale);
    mRoot->SetPivot(mPlayerData->Pivot);
    mBody->SetBoxSize(mPlayerData->ColliderSize);
    mMovement->SetMoveSpeed(mPlayerData->Speed);
    
    const std::vector<FInventorySaveData>& SaveList = mPlayerData->GetInvenSaveList();

    if(!SaveList.empty())
    {
        for (const FInventorySaveData& Data : SaveList)
        {
            FItemData* NewItem = CItemManager::GetInst()->CloneItem(Data.ItemName);

            if (NewItem)
            {
                NewItem->Count = Data.Count;
                mInventory->AddItem(NewItem);
            }
        }
    }
}

CInventoryComponent* CPlayerObject::GetInventory() const
{
    return mInventory;
}

CColliderBase* CPlayerObject::GetCollider() const
{
    if (mBody)
        return mBody;

    return nullptr;
}

CColliderBase* CPlayerObject::GetAttackCollider() const
{
    if (mAttackRange)
        return mAttackRange;

    return nullptr;
}


CQuest* CPlayerObject::FindQuestByID(int QuestID) const
{
    for (CQuest* Quest : GetQuestList())
    {
        if (Quest && Quest->GetQuestID() == QuestID)
            return Quest;
    }

    return nullptr;
}

void CPlayerObject::SetInvincible(bool Invincible)
{
    if (Invincible)
    {
        mPlayerData->Invincible = Invincible;
        mBody->SetEnable(false);
    }
    else
    {
        mPlayerData->Invincible = Invincible;
        mBody->SetEnable(true);
    }
    
}

void CPlayerObject::AddQuest(CQuest* Quest)
{
    if (Quest)
        mPlayerData->mQuestList.push_back(Quest);
}

void CPlayerObject::RemoveQuest(int QuestID)
{
    auto iter = mPlayerData->mQuestList.begin();
    auto iterEnd = mPlayerData->mQuestList.end();

    for (; iter != iterEnd; ++iter)
    {
        if ((*iter)->GetQuestID() == QuestID)
        {
            mPlayerData->mQuestList.erase(iter);
            return;
        }
    }
}

void CPlayerObject::RegisterQuickSlot(int Index, FItemData* Item)
{
    if (!Item)
        return;

    mPlayerData->QuickSlots[Index] = Item;

    CQuickSlotList* QuickUI = mScene->GetMainWidget()->GetQuickSlot();

    if (QuickUI)
        QuickUI->SetSlotIcon(Index, Item ? Item->Icon : nullptr);
}

void CPlayerObject::UseQuickSlot(int Index)
{
    FItemData* Item = mPlayerData->QuickSlots[Index];

    if (!Item)
        return;

    // 아이템 사용
    GetInventory()->UseItem(Item);
    mScene->GetAssetManager()->FindSound("PlayerEat")->Play();
    
    // 아이템의 갯수가 모두 소진되었을 경우 
    // 등록된 아이템과 UI제거
    if (Item->Count <= 0)
    {
        // 모든 퀵슬롯 검사
        for (int i = 0; i < 4; ++i)
        {
            if (mPlayerData->QuickSlots[i] == Item)
            {
                mPlayerData->QuickSlots[i] = nullptr;

                // UI 갱신
                CQuickSlotList* QuickUI = mScene->GetMainWidget()->GetQuickSlot();
                if (QuickUI)
                    QuickUI->SetSlotIcon(i, nullptr);
            }
        }
    }
}

bool CPlayerObject::IsControl()
{
    CMainWidget* MainWidget = mScene->GetMainWidget();

    return !MainWidget->GetDialogueUI()->IsEnable() &&
        !MainWidget->GetInventoryUI()->IsEnable() &&
        !MainWidget->GetQuestUI()->IsEnable() &&
        !MainWidget->GetStoreUI()->IsEnable() &&
        !MainWidget->GetRewardUI()->IsEnable();
}

void CPlayerObject::ChangeAnimation(std::string AnimName)
{
    mAnimation->ChangeAnimation(AnimName);
}

void CPlayerObject::InitAnimAddSequence()
{
    // Idle 모션 8방향
    {
        mAnimation->AddSequence("Player_IdleDown", 1.f, 1.f, true, false);
        mAnimation->AddSequence("Player_IdleDownLeft", 1.f, 1.f, true, false);
        mAnimation->AddSequence("Player_IdleDownRight", 1.f, 1.f, true, false);
        mAnimation->AddSequence("Player_IdleUp", 1.f, 1.f, true, false);
        mAnimation->AddSequence("Player_IdleUpLeft", 1.f, 1.f, true, false);
        mAnimation->AddSequence("Player_IdleUpRight", 1.f, 1.f, true, false);
        mAnimation->AddSequence("Player_IdleLeft", 1.f, 1.f, true, false);
        mAnimation->AddSequence("Player_IdleRight", 1.f, 1.f, true, false);
    }
    

    // 걷는 모션 8방향
    {
        mAnimation->AddSequence("Player_WalkDown", 1.f, 1.f, true, false);
        mAnimation->AddSequence("Player_WalkDownLeft", 1.f, 1.f, true, false);
        mAnimation->AddSequence("Player_WalkDownRight", 1.f, 1.f, true, false);
        mAnimation->AddSequence("Player_WalkUp", 1.f, 1.f, true, false);
        mAnimation->AddSequence("Player_WalkUpLeft", 1.f, 1.f, true, false);
        mAnimation->AddSequence("Player_WalkUpRight", 1.f, 1.f, true, false);
        mAnimation->AddSequence("Player_WalkLeft", 1.f, 1.f, true, false);
        mAnimation->AddSequence("Player_WalkRight", 1.f, 1.f, true, false);
    }


    // 공격 모션 8방향
    {
        mAnimation->AddSequence("Player_AttackDown", 0.7f, 0.7f, false, false);
        mAnimation->AddSequence("Player_AttackDownLeft", 0.7f, 0.7f, false, false);
        mAnimation->AddSequence("Player_AttackLeft", 0.7f, 0.7f, false, false);
        mAnimation->AddSequence("Player_AttackUpLeft", 0.7f, 0.7f, false, false);
        mAnimation->AddSequence("Player_AttackUp", 0.7f, 0.7f, false, false);
        mAnimation->AddSequence("Player_AttackUpRight", 0.7f, 0.7f, false, false);
        mAnimation->AddSequence("Player_AttackRight", 0.7f, 0.7f, false, false);
        mAnimation->AddSequence("Player_AttackDownRight", 0.7f, 0.7f, false, false);
    }

    // 뛰는 모션 8방향
    {
        mAnimation->AddSequence("Player_RunDown", 0.7f, 1.f, true, false);
        mAnimation->AddSequence("Player_RunUp", 0.7f, 1.f, true, false);
        mAnimation->AddSequence("Player_RunLeft", 0.7f, 1.f, true, false);
        mAnimation->AddSequence("Player_RunRight", 0.7f, 1.f, true, false);
        mAnimation->AddSequence("Player_RunDownLeft", 0.7f, 1.f, true, false);
        mAnimation->AddSequence("Player_RunUpLeft", 0.7f, 1.f, true, false);
        mAnimation->AddSequence("Player_RunDownRight", 0.7f, 1.f, true, false);
        mAnimation->AddSequence("Player_RunUpRight", 0.7f, 1.f, true, false);
    }

    // 플레이어 맞는 모션 8방향
    {
        mAnimation->AddSequence("Player_HurtDown",      0.5f, 1.f, false, false);
        mAnimation->AddSequence("Player_HurtUp",        0.5f, 1.f, false, false);
        mAnimation->AddSequence("Player_HurtLeft",      0.5f, 1.f, false, false);
        mAnimation->AddSequence("Player_HurtRight",     0.5f, 1.f, false, false);
        mAnimation->AddSequence("Player_HurtDownLeft",  0.5f, 1.f, false, false);
        mAnimation->AddSequence("Player_HurtUpLeft",    0.5f, 1.f, false, false);
        mAnimation->AddSequence("Player_HurtDownRight", 0.5f, 1.f, false, false);
        mAnimation->AddSequence("Player_HurtUpRight",   0.5f, 1.f, false, false);
    }
}
void CPlayerObject::InitAnimBindFunction()
{
    // 공격 Notify
    {
        mAnimation->AddNotify<CPlayerObject>("Player_AttackDown",
            1, this, &CPlayerObject::AttackNotify);
        mAnimation->AddNotify<CPlayerObject>("Player_AttackDownLeft",
            1, this, &CPlayerObject::AttackNotify);
        mAnimation->AddNotify<CPlayerObject>("Player_AttackLeft",
            1, this, &CPlayerObject::AttackNotify);
        mAnimation->AddNotify<CPlayerObject>("Player_AttackUpLeft",
            1, this, &CPlayerObject::AttackNotify);
        mAnimation->AddNotify<CPlayerObject>("Player_AttackUp",
            1, this, &CPlayerObject::AttackNotify);
        mAnimation->AddNotify<CPlayerObject>("Player_AttackUpRight",
            1, this, &CPlayerObject::AttackNotify);
        mAnimation->AddNotify<CPlayerObject>("Player_AttackRight",
            1, this, &CPlayerObject::AttackNotify);
        mAnimation->AddNotify<CPlayerObject>("Player_AttackDownRight",
            1, this, &CPlayerObject::AttackNotify);
    }
    
    // 공격 End Function
    {
        mAnimation->SetEndFunction<CPlayerObject>("Player_AttackDown",
            this, &CPlayerObject::AttackEnd);
        mAnimation->SetEndFunction<CPlayerObject>("Player_AttackDownLeft",
            this, &CPlayerObject::AttackEnd);
        mAnimation->SetEndFunction<CPlayerObject>("Player_AttackLeft",
            this, &CPlayerObject::AttackEnd);
        mAnimation->SetEndFunction<CPlayerObject>("Player_AttackUpLeft",
            this, &CPlayerObject::AttackEnd);
        mAnimation->SetEndFunction<CPlayerObject>("Player_AttackUp",
            this, &CPlayerObject::AttackEnd);
        mAnimation->SetEndFunction<CPlayerObject>("Player_AttackUpRight",
            this, &CPlayerObject::AttackEnd);
        mAnimation->SetEndFunction<CPlayerObject>("Player_AttackRight",
            this, &CPlayerObject::AttackEnd);
        mAnimation->SetEndFunction<CPlayerObject>("Player_AttackDownRight",
            this, &CPlayerObject::AttackEnd);
    }
}
void CPlayerObject::InitBindKey()
{
    // 이동 관련 BindKey
    {
        // 상하좌우 이동
        mScene->GetInput()->AddBindKey("MoveUp", 'W');
        mScene->GetInput()->AddBindKey("MoveDown", 'S');
        mScene->GetInput()->AddBindKey("MoveRight", 'D');
        mScene->GetInput()->AddBindKey("MoveLeft", 'A');

        // 인벤토리 활성화
        mScene->GetInput()->AddBindKey("Inventory", 'I');
        mScene->GetInput()->AddBindFunction<CPlayerObject>("Inventory",
            EInputType::Down, this, &CPlayerObject::InventoryUIInteraction);

        // 퀵 슬롯 등록 & 사용
        mScene->GetInput()->AddBindKey("Quick1", '1');
        mScene->GetInput()->AddBindKey("Quick2", '2');
        mScene->GetInput()->AddBindKey("Quick3", '3');
        mScene->GetInput()->AddBindKey("Quick4", '4');

        // UI창 닫기
        mScene->GetInput()->AddBindKey("CloseUI", VK_ESCAPE);
        mScene->GetInput()->AddBindFunction<CPlayerObject>("CloseUI",
            EInputType::Down, this, &CPlayerObject::EscapeInteraction);

        // 상호작용 키
        mScene->GetInput()->AddBindKey("InteractionKey", 'F');
        mScene->GetInput()->AddBindFunction<CPlayerObject>("InteractionKey",
            EInputType::Down, this, &CPlayerObject::Interaction);

        // 대화하기 키
        mScene->GetInput()->AddBindKey("DialogueInteraction", 'E');
        mScene->GetInput()->AddBindFunction<CPlayerObject>("DialogueInteraction",
            EInputType::Down, this, &CPlayerObject::DialogueInteraction);

        // 퀘스트 UI 열고 닫기
        mScene->GetInput()->AddBindKey("QuestUI", 'Q');
        mScene->GetInput()->AddBindFunction<CPlayerObject>("QuestUI",
            EInputType::Down, this, &CPlayerObject::QuestUIInteraction);

        // 상하좌우 이동 BindFunction
        mScene->GetInput()->AddBindFunction<CPlayerObject>("MoveUp",
            EInputType::Hold, this, &CPlayerObject::MoveUp);
        mScene->GetInput()->AddBindFunction<CPlayerObject>("MoveDown",
            EInputType::Hold, this, &CPlayerObject::MoveDown);
        mScene->GetInput()->AddBindFunction<CPlayerObject>("MoveLeft",
            EInputType::Hold, this, &CPlayerObject::MoveLeft);
        mScene->GetInput()->AddBindFunction<CPlayerObject>("MoveRight",
            EInputType::Hold, this, &CPlayerObject::MoveRight);

        // 퀵슬롯 BindFunction
        mScene->GetInput()->AddBindFunction<CPlayerObject>("Quick1",
            EInputType::Down, this, &CPlayerObject::QuickSlotKey1);
        mScene->GetInput()->AddBindFunction<CPlayerObject>("Quick2",
            EInputType::Down, this, &CPlayerObject::QuickSlotKey2);
        mScene->GetInput()->AddBindFunction<CPlayerObject>("Quick3",
            EInputType::Down, this, &CPlayerObject::QuickSlotKey3);
        mScene->GetInput()->AddBindFunction<CPlayerObject>("Quick4",
            EInputType::Down, this, &CPlayerObject::QuickSlotKey4);
    }

    mScene->GetInput()->AddBindKey("PlayerAttack", VK_SPACE);
    mScene->GetInput()->AddBindFunction<CPlayerObject>("PlayerAttack",
        EInputType::Down, this, &CPlayerObject::Attack);
}

void CPlayerObject::MoveUp(float DeltaTime)
{
    // 만약 움직일 수 없거나 맞았다면,
    // NPC와 대화중이라면 이동X
    if (!mPlayerData->CanMove || 
        mPlayerData->Hitting || 
        !IsControl())
        return;

    // 역방향 키를 누르면 이동 X
    if (mScene->GetInput()->IsKeyHold('S')) 
        return;

    bool MoveLeft = mScene->GetInput()->IsKeyHold('A');
    bool MoveRight = mScene->GetInput()->IsKeyHold('D');

    // 좌우 둘다 눌렀으면 이동 X
    if (MoveLeft && MoveRight) 
        return; 

    mPlayerData->Moving = true;

    // 대각선 이동 처리
    if (MoveLeft)
    {
       
        if (mScene->GetInput()->IsKeyHold(VK_LSHIFT))
        {
            mPlayerData->Dir = EObjectDir::TL;
            mStateMachine->ChangeState(EObjectState::Run, mPlayerData->Dir);
        }
        
        else
        {
            mPlayerData->Dir = EObjectDir::TL;
            mStateMachine->ChangeState(EObjectState::Walk, EObjectDir::TL);
        }
       
        mMovement->AddMove(FVector3D(-1, 1, 0));
    }

    else if (MoveRight)
    {
        mPlayerData->Dir = EObjectDir::TR;
        mStateMachine->ChangeState(EObjectState::Walk, EObjectDir::TR);
        mMovement->AddMove(FVector3D(1, 1, 0));
    }

    else
    {
        mPlayerData->Dir = EObjectDir::T;
        mStateMachine->ChangeState(EObjectState::Walk, EObjectDir::T);
        mMovement->AddMove(FVector3D(0, 1, 0));
    }

}
void CPlayerObject::MoveDown(float DeltaTime)
{
    // 만약 움직일 수 없거나 맞았다면,
    // NPC와 대화중이라면 이동X
    if (!mPlayerData->CanMove ||
        mPlayerData->Hitting ||
        !IsControl())
        return;

    // 역방향 키를 누르면 이동 X
    if (mScene->GetInput()->IsKeyHold('W')) 
        return; 

    bool MoveLeft = mScene->GetInput()->IsKeyHold('A');
    bool MoveRight = mScene->GetInput()->IsKeyHold('D');

    // 좌우 둘다 눌렀으면 이동 X
    if (MoveLeft && MoveRight) 
        return;

    mPlayerData->Moving = true;

    // 대각선 이동처리
    if (MoveLeft)
    {
        mPlayerData->Dir = EObjectDir::BL;
        mStateMachine->ChangeState(EObjectState::Walk, EObjectDir::BL);
        mMovement->AddMove(FVector3D(-1, -1, 0));
    }

    else if (MoveRight)
    {
        mPlayerData->Dir = EObjectDir::BR;
        mStateMachine->ChangeState(EObjectState::Walk, EObjectDir::BR);
        mMovement->AddMove(FVector3D(1, -1, 0));
    }

    else
    {
        mPlayerData->Dir = EObjectDir::B;
        mStateMachine->ChangeState(EObjectState::Walk, EObjectDir::B);
        mMovement->AddMove(FVector3D(0, -1, 0));
    }
}
void CPlayerObject::MoveLeft(float DeltaTime)
{   
    // 만약 움직일 수 없거나 맞았다면,
    // NPC와 대화중이라면 이동X
    if (!mPlayerData->CanMove ||
        mPlayerData->Hitting ||
        !IsControl())
        return;

    // 역방향 키를 누르면 이동 X
    if (mScene->GetInput()->IsKeyHold('D')) 
        return;

    bool MoveUp = mScene->GetInput()->IsKeyHold('W');
    bool MoveDown = mScene->GetInput()->IsKeyHold('S');

    // 상하 둘다 눌렀으면 이동 X
    if (MoveUp && MoveDown) 
        return;

    mPlayerData->Moving = true;

    // 대각선 이동처리
    if (MoveUp)
    {
        mPlayerData->Dir = EObjectDir::TL;
        mStateMachine->ChangeState(EObjectState::Walk, EObjectDir::TL);
        mMovement->AddMove(FVector3D(-1, 1, 0));
    }

    else if (MoveDown)
    {
        mPlayerData->Dir = EObjectDir::BL;
        mStateMachine->ChangeState(EObjectState::Walk, EObjectDir::BL);
        mMovement->AddMove(FVector3D(-1, -1, 0));
    }

    else
    {
        mPlayerData->Dir = EObjectDir::L;
        mStateMachine->ChangeState(EObjectState::Walk, EObjectDir::L);
        mMovement->AddMove(FVector3D(-1, 0, 0));
    }

}
void CPlayerObject::MoveRight(float DeltaTime)
{
    // 만약 움직일 수 없거나 맞았다면,
    // NPC와 대화중이라면 이동X
    if (!mPlayerData->CanMove ||
        mPlayerData->Hitting ||
        !IsControl())
        return;

    // 역방향 키를 누르면 이동 X
    if (mScene->GetInput()->IsKeyHold('A')) 
        return;

    bool MoveUp = mScene->GetInput()->IsKeyHold('W');
    bool MoveDown = mScene->GetInput()->IsKeyHold('S');

    // 상하 둘다 눌렀으면 이동X
    if (MoveUp && MoveDown) 
        return;

    mPlayerData->Moving = true;

    // 대각선 이동처리
    if (MoveUp)
    {
        mPlayerData->Dir = EObjectDir::TR;
        mStateMachine->ChangeState(EObjectState::Walk, EObjectDir::TR);
        mMovement->AddMove(FVector3D(1, 1, 0));
    }

    else if (MoveDown)
    {
        mPlayerData->Dir = EObjectDir::BR;
        mStateMachine->ChangeState(EObjectState::Walk, EObjectDir::BR);
        mMovement->AddMove(FVector3D(1, -1, 0));
    }
    else
    {
        mPlayerData->Dir = EObjectDir::R;
        mStateMachine->ChangeState(EObjectState::Walk, EObjectDir::R);
        mMovement->AddMove(FVector3D(1, 0, 0));
    }
}

void CPlayerObject::Attack(float DeltaTime)
 {
    // 만약 공격 불가거나 맞고 있으면,
    // NPC와 대화중이라면 공격 불가

    if (!mPlayerData->CanAttack ||
        mPlayerData->Hitting ||
        !IsControl() ||
        mPlayerData->Attacking)
        return;

    mRoot->SetWorldScale(200.f, 200.f, 1.f);

    mStateMachine->ChangeState(EObjectState::Attack, mPlayerData->Dir);
}
void CPlayerObject::QuickSlotKey1()
{
    QuickSlotInteraction(0);
}
void CPlayerObject::QuickSlotKey2()
{
    QuickSlotInteraction(1);
}
void CPlayerObject::QuickSlotKey3()
{
    QuickSlotInteraction(2);
}
void CPlayerObject::QuickSlotKey4()
{
    QuickSlotInteraction(3);
}

void CPlayerObject::Interaction()
{
    CMainWidget* MainWidget = mScene->GetMainWidget();

    // 1. 상점창이 열려있다면
    if (MainWidget->GetStoreUI()->IsEnable())
    {
        // 현재 상점에서 플레이어가 선택한 아이템을 가져온다.
        FItemData* Item = MainWidget->GetStoreUI()->GetSelectedItem();
        CStoreComponent* StoreComp = MainWidget->GetStoreUI()->GetStoreComponent();

        // 현재 상점에서 선택된 아이템이 있고
        // 상점 컴포넌트가 있다면
        if (Item && StoreComp)
        {
            // 아이템 구매를 진행한다.
            StoreComp->BuyItem(CItemManager::GetInst()->CloneItem(Item->Name));
        }
        mScene->GetAssetManager()->FindSound("UIOpen");
    }

    // 2. 퀘스트 창이 열려있고, 퀘스트 상호작용 버튼이 있을때만
    // 퀘스트 수락 & 완료 처리를 진행한다.
    else if (MainWidget->GetQuestUI()->IsEnable() && 
        MainWidget->GetQuestUI()->IsEnableAcceptedBtn())
    {
        // 현재 플레이어가 선택한 퀘스트를 얻어온다.
        CQuest* SelectedQuest = MainWidget->GetQuestUI()->GetSelectedQuest();

        // 만약 선택된 퀘스트가 있다면
        if (SelectedQuest)
        {
            // 현재 상호작용하고 있는 NPC를 씬에서 얻어온다.
            CNPCObject* NPC = mScene->FindNPCByID(mPlayerData->CurrentNPCID);

            if (NPC)
            {
                // 만약 선택된 퀘스트가 수락전 이었다면 
                // 플레이어가 퀘스트 수락을 진행하는 것이니 진행상태로 변경한다.
                if (SelectedQuest->GetQuestState() == EQuestState::NotAccepted)
                {
                    SelectedQuest->SetQuestState(EQuestState::Progress);
                    // 퀘스트 상태를 변경하고 플레이어의 퀘스트 목록에 추가.
                    AddQuest(SelectedQuest);
                }

                // 만약 선택된 퀘스트를 플레이어가 완료처리를 진행해야 한다면
                // 퀘스트 완료 처리를 진행한다.
                // Complete 는 완료 조건 달성 / Completed 는 완료된 퀘스트
                else if (SelectedQuest->GetQuestState() == EQuestState::Complete)
                {
                    // 선택한 퀘스트를 완료된 상태로 변경
                    SelectedQuest->SetQuestState(EQuestState::Completed);
                    // 완료된 퀘스트이니 플레이어 퀘스트 목록에서 빼준다.
                    RemoveQuest(SelectedQuest->GetQuestID());
                    // 플레이어의 완료된 퀘스트 목록에 넣는다.
                    AddCompletedQuest(SelectedQuest->GetQuestID());
                    // NPC에게도 해당 퀘스트ID를 지워준다.
                    NPC->RemoveQuestID(SelectedQuest->GetQuestID());

                    // 선택한 퀘스트의 다음 퀘스트 ID를 얻어온다.
                    int NextQuestID = SelectedQuest->GetNextQuestID();

                    // 만약 다음 퀘스트 ID가 유효하고 플레이어가 완료한 퀘스트가 아니라면
                    if (NextQuestID != -1 && !IsQuestCompleted(NextQuestID))
                    {
                        // 퀘스트 매니저에서 해당 퀘스트를 가져온다.
                        CQuest* NextQuest = CQuestManager::GetInst()->FindQuest(NextQuestID);

                        // 다음 퀘스트를 NPC에게 추가.
                        if (NextQuest)
                            NPC->AddQuestID(NextQuestID);
                    }

                    // 완료 대사 출력 준비

                    NPC->SetCurrentScriptID(400); // 400 = 완료 대사 시작 스크립트 ID
                    mPlayerData->IsTalking = true;
                    mWaitingRewardQuest = SelectedQuest;

                    MainWidget->GetQuestUI()->SetEnable(false);
                    MainWidget->GetDialogueUI()->SetEnable(true);
                    MainWidget->ShowScript(mPlayerData->CurrentNPCID, NPC->GetCurrentScriptID());

                    const FScriptLine* Line = CScriptManager::GetInst()->FindScriptLine(
                        mPlayerData->CurrentNPCID, NPC->GetCurrentScriptID());

                    if (Line)
                        NPC->SetCurrentScriptID(Line->NextLineID);
                    else
                        NPC->SetCurrentScriptID(-1);
                }

                // NPC의 퀘스트 유무에 따라 NPC의 퀘스트 UI 표시
                EQuestState State =
                    CQuestManager::GetInst()->GetNPCQuestMarkState(NPC->GetQuestIDList(), this);

                switch (State)
                {
                case EQuestState::NotAccepted:
                    NPC->ChangQuesteAnimation("QuestMark1");
                    NPC->SetQuestMarkEnable(true);
                    break;
                case EQuestState::Progress:
                case EQuestState::Complete:
                    NPC->ChangQuesteAnimation("QuestMark2");
                    NPC->SetQuestMarkEnable(true);
                    break;
                case EQuestState::End:
                    NPC->SetQuestMarkEnable(false);
                    break;
                }

                MainWidget->GetQuestUI()->SetEnable(false);
                mScene->GetAssetManager()->FindSound("UIClose");
                return;
            }
        }
    }

    // 3. 인벤토리 창이 열려있을 경우
    if (MainWidget->GetInventoryUI()->IsEnable())
    {
        // 현재 선택된 아이템이 있을 경우에만 아이템 사용을 진행해야 한다.
        if (MainWidget->GetInventoryUI() &&
            MainWidget->GetInventoryUI()->GetSelectedItem())
        {
            // 현재 인벤토리 창에서 선택된 아이템을 얻어온다.
            FItemData* Selected = MainWidget->GetInventoryUI()
                ->GetSelectedItem();

            // 현재 선택된 아이템이 사용가능한 아이템일 경우 (소모품)
            if (Selected && Selected->Useable)
            {
                GetInventory()->UseItem(Selected);
                MainWidget->GetInventoryUI()->
                    SetCategory(MainWidget->GetInventoryUI()->GetCurrentCategory());
                mScene->GetAssetManager()->FindSound("PlayerEat")->Play();
            }

        }
    }
}
void CPlayerObject::EscapeInteraction()
{
    // 메인 위젯을 씬에서 가져온다.
    CMainWidget* MainWidget = mScene->GetMainWidget();

    // 1. 대화창이 열려있으면 닫기
    if (MainWidget->GetDialogueUI()->IsEnable())
    {
        if (mPlayerData->IsTalking)
        {
            CNPCObject* NPC = mScene->FindNPCByID(mPlayerData->CurrentNPCID);
            if (!NPC)
                return;
            
            NPC->SetCurrentScriptID(NPC->GetStartScriptID());
            mPlayerData->IsTalking = false;
        }
        mScene->GetAssetManager()->FindSound("UIClose")->Play();
        MainWidget->GetDialogueUI()->SetEnable(false);
    }

    // 2. 상점이 열려 있으면 닫기
    else if (MainWidget->GetStoreUI()->IsEnable())
    {

        // 씬에서 상점 NPC를 찾는다.
        CShopNPC* ShopNPC = dynamic_cast<CShopNPC*>
            (mScene->FindNPCByID(mPlayerData->CurrentNPCID));

        if (ShopNPC)
        {
            if (ShopNPC && ShopNPC->GetStoreComponent())
                ShopNPC->GetStoreComponent()->CloseStore();
        }

        mScene->GetAssetManager()->FindSound("UIClose")->Play();
        MainWidget->GetStoreUI()->SetEnable(false);
    }

    // 3. 인벤토리 창이 열려있으면 닫기
    else if (MainWidget->GetInventoryUI()->IsEnable())
    {
        MainWidget->GetInventoryUI()->SetEnable(false);
        MainWidget->GetInventoryUI()->SetSelectedItem(nullptr);
        mScene->GetAssetManager()->FindSound("UIClose")->Play();
    }

    // 4. 퀘스트 창이 열려 있으면 닫기
    else if (MainWidget->GetQuestUI()->IsEnable())
    {
        CNPCObject* NPC = mScene->FindNPCByID(mPlayerData->CurrentNPCID);
        if (!NPC)
            return;

        if (mOpenedQuestUIByDialogue)
        {
            // 150번 출력
            MainWidget->ShowScript(mPlayerData->CurrentNPCID, 150);

            // CurrentScriptID를 151로 설정 (다음 줄)
            NPC->SetCurrentScriptID(151);

            // 대화 모드로 진입한 상태로 표시
            mPlayerData->IsTalking = true;

            // 퀘스트창 닫기
            mOpenedQuestUIByDialogue = false;
        }

        MainWidget->GetQuestUI()->SetEnable(false);
        mScene->GetAssetManager()->FindSound("UIClose")->Play();
    }
      
    // 5. 보상창이 열려 있으면 닫기
    // 단 이후 받을 보상이 더 있다면 다 보여주고 닫는다.
    else if (MainWidget->GetRewardUI()->IsEnable())
    {
        MainWidget->GetRewardUI()->ShowNextReward();
    }

    mPlayerData->CanSpeak = true;
}
void CPlayerObject::InventoryUIInteraction()
{
    CMainWidget* MainWidget = mScene->GetMainWidget();

    // 다른 UI가 열려있으면 인벤토리를 열수 없다.
    if (MainWidget->IsAnyUIOpened() && 
        !MainWidget->GetInventoryUI()->IsEnable())
        return;

    // I키를 눌렀을 때 인벤토리가 열려 있다면
    // 인벤토리 창을 닫아준다.
    if (MainWidget->GetInventoryUI()->IsEnable())
    {
        MainWidget->GetInventoryUI()->SetSelectedItem(nullptr);
        MainWidget->GetInventoryUI()->SetEnable(false);
        mScene->GetAssetManager()->FindSound("UIClose")->Play();
    }

    // 만약 닫혀 있으면 인벤토리 창을 열어야한다.
    // 인벤토리 창에 플레이어 골드정보를 갱신 시켜준다.
    // 열때마다 고정으로 소모품 카테고리 아이템 목록을 보여준다.
    else
    {
        MainWidget->GetInventoryUI()->SetGoldInfo(mPlayerData->Gold);
        MainWidget->GetInventoryUI()->SetCategory(EItemCategory::Consumable);
        MainWidget->GetInventoryUI()->SetEnable(true);
        mScene->GetAssetManager()->FindSound("UIOpen")->Play();
    }
}
void CPlayerObject::QuestUIInteraction()
{
    CMainWidget* MainWidget = mScene->GetMainWidget();

    // 다른 UI가 열려있고 퀘스트 창은 아직 안열려있다면
    // 열수 없다.
    if (MainWidget->IsAnyUIOpened() && 
        !MainWidget->GetQuestUI()->IsEnable())
        return;


    // 퀘스트 창이 열려있으면
    // 퀘스트 창을 닫는다.
    if (MainWidget->GetQuestUI()->IsEnable())
    {
        MainWidget->GetQuestUI()->SetEnable(false);
        mScene->GetAssetManager()->FindSound("UIClose")->Play();
    }

    // 퀘스트 창이 닫혀있었으면 이제 열어야한다.
    // 플레이어의 퀘스트 목록을 넘겨 보여준다.
    else
    {
        MainWidget->GetQuestUI()->OpenQuestUI(mPlayerData->mQuestList);
        MainWidget->GetQuestUI()->SetQuestEnableBtn(false);
    }
       
}
void CPlayerObject::DialogueInteraction()
{
    // 만약 플레이어가 대화가 가능하지 않다면 return
    if (!mPlayerData->CanSpeak)
        return;

    CMainWidget* MainWidget = mScene->GetMainWidget();
    CNPCObject* NPC = mScene->FindNPCByID(mPlayerData->CurrentNPCID);

    if (MainWidget->GetQuestUI()->IsEnable() ||
        MainWidget->GetRewardUI()->IsEnable())
        return;

    if (!NPC)
        return;

    if (NPC->GetLastScriptID() == 151)
    {
        MainWidget->ShowScript(mPlayerData->CurrentNPCID, 151);
        mPlayerData->IsTalking = false;
        MainWidget->GetDialogueUI()->SetEnable(false);
        NPC->SetCurrentScriptID(-1);
        mScene->GetAssetManager()->FindSound("UIClose")->Play();
        return;
    }

    // 대화 중이 아니었다면
    if (!mPlayerData->IsTalking)
    {
        // 대화 시작 상태로 전환
        mPlayerData->IsTalking = true;
        // 대화창 활성화
        MainWidget->GetDialogueUI()->SetEnable(true);

        NPC->SetCurrentScriptID(NPC->GetStartScriptID());

        // 현재 NPCID와 NPC의 현재 스크립트 아이디를 넘겨서
        // 해당 NPC의 대사를 보여준다.
        MainWidget->ShowScript(mPlayerData->CurrentNPCID,
            NPC->GetCurrentScriptID());

        // NPC의 현재 스크립트 아이디를
        // 현재 스크립트 아이디의 다음 스크립트 ID로 설정한다.
        const FScriptLine* Line = CScriptManager::GetInst()->
            FindScriptLine(mPlayerData->CurrentNPCID, NPC->GetCurrentScriptID());

        if (Line)
            NPC->SetCurrentScriptID(Line->NextLineID);
        else
            NPC->SetCurrentScriptID(-1);

        return;
    }

    // NPC의 현재 스크립트 ID가 -1이면
    // 출력할 대사가 없다는 것이다.
    if (NPC->GetCurrentScriptID() == -1)
    {
        // 대화 종료.
        mPlayerData->IsTalking = false;
        MainWidget->GetDialogueUI()->SetEnable(false);

        // 대화 종료 후 처리

        // 1. 상점 NPC
        CShopNPC* ShopNPC = dynamic_cast<CShopNPC*>(NPC);

        // 만약 지금 대화가 끝난 NPC가
        // 상점 NPC이고 상점 컴포넌트를 가지고 있다면
        // 상점을 열어준다.
        if (ShopNPC && ShopNPC->GetStoreComponent())
            ShopNPC->GetStoreComponent()->OpenStore(this);


        // 2. NPC 퀘스트 수락 가능 여부 확인
        else
        {
            // 만약 보상 퀘스트가 있다면
            // 대화 종료 후 보상 UI 오픈
            if (mWaitingRewardQuest)
            {
                MainWidget->GetRewardUI()->OpenRewardUI(mWaitingRewardQuest);
                mWaitingRewardQuest = nullptr;

                // NPC의 퀘스트 유무에 따라 NPC의 퀘스트 UI 표시
                EQuestState State =
                    CQuestManager::GetInst()->GetNPCQuestMarkState(NPC->GetQuestIDList(), this);

                switch (State)
                {
                case EQuestState::NotAccepted:
                    NPC->ChangQuesteAnimation("QuestMark1");
                    NPC->SetQuestMarkEnable(true);
                    break;
                case EQuestState::Progress:
                case EQuestState::Complete:
                    NPC->ChangQuesteAnimation("QuestMark2");
                    NPC->SetQuestMarkEnable(true);
                    break;
                case EQuestState::End:
                    NPC->SetQuestMarkEnable(false);
                    break;
                }
                return;
            }

            // 화면에 보여줄 퀘스트 목록을 저장할 list
            std::list<CQuest*> OpenableQuests;

            // 현재 상호작용중인 NPC의 퀘스트 ID목록을 가져온다.
            for (int QuestID : NPC->GetQuestIDList())
            {
                // 플레이어한테 NPC의 퀘스트가 있는지 찾는다.
                CQuest* PlayerQuest = FindQuestByID(QuestID);

                // 만약 플레이어한테 NPC가 가지고 있는 퀘스트가 없다면
                // 퀘스트를 아직 수락하지 않았다는 이야기
                if (!PlayerQuest)
                {
                    // 퀘스트 매니저에서 해당 퀘스트를 얻어온다.
                    CQuest* Quest = CQuestManager::GetInst()->FindQuest(QuestID);
                    // 보여줄 퀘스트 목록에 추가
                    if (Quest)
                        OpenableQuests.push_back(Quest);
                }

                // 만약 플레이어의 퀘스트가 완료 조건이 채워졌다면
                // 보여줄 목록에 완료한 퀘스트도 추가한다.
                if (PlayerQuest && PlayerQuest->IsComplete())
                {
                    OpenableQuests.push_back(PlayerQuest);
                }
            }

            // 보여줄 퀘스트 목록이 있으면
            // 해당 퀘스트 목록을 보여준다.
            if (!OpenableQuests.empty())
            {
                MainWidget->GetQuestUI()->OpenQuestUI(OpenableQuests);
                MainWidget->GetQuestUI()->SetQuestTagEnable(false);
                mOpenedQuestUIByDialogue = true;
            }
        }

        return;
    }


    // NPC의 출력할 다음 대사가 있다면
    // 다음 대사를 보여주고. 또 다음 대사를 NPC에 세팅한다.
    MainWidget->ShowScript(mPlayerData->CurrentNPCID, NPC->GetCurrentScriptID());
    NPC->SetCurrentScriptID(CScriptManager::GetInst()->
        FindScriptLine(mPlayerData->CurrentNPCID, NPC->GetCurrentScriptID())->NextLineID);
}

void CPlayerObject::ConstrainToMap()
{
    FVector2D Pos;
    Pos.x = mRootComponent->GetWorldPosition().x;
    Pos.y = mRootComponent->GetWorldPosition().y;

    // X축 제한 (왼쪽 & 오른쪽 벽)
    // 최대 X 경계를 벗어나면 고정
    if (Pos.x > mMap->GetPlayerMaxBounds().x)
        Pos.x = mMap->GetPlayerMaxBounds().x;

    // 최소 X 경계를 벗어나면 고정
    else if (Pos.x < mMap->GetPlayerMinBounds().x)
        Pos.x = mMap->GetPlayerMinBounds().x;

    // Y축 제한 (위 & 아래 벽)
    // 최대 Y 경계를 벗어나면 고정
    if (Pos.y > mMap->GetPlayerMaxBounds().y)
        Pos.y = mMap->GetPlayerMaxBounds().y;
    // 최소 Y 경계를 벗어나면 고정
    else if (Pos.y < mMap->GetPlayerMinBounds().y)
        Pos.y = mMap->GetPlayerMinBounds().y;

    mRootComponent->SetWorldPos(Pos);
}

void CPlayerObject::CollisionPlayerAttack(const FVector3D& HitPoint,
    CColliderBase* Dest)
{
    FCollisionProfile* DestProfile = Dest->GetProfile();

    if (DestProfile->Channel == ECollisionChannel::Monster)
    {
        CMonsterObject* Monster = dynamic_cast<CMonsterObject*>(Dest->GetOwner());

        if (!Monster)
            return;

        if (Monster->IsDead())
            return;

        Monster->Damage(1.f, this);
    }

    else if (DestProfile->Channel == ECollisionChannel::BossAttack)
    {
        CBossBullet* DestBullet = dynamic_cast<CBossBullet*>(Dest->GetOwner());
        if (!DestBullet)
            return;

        DestBullet->SetDirection(Dest->GetWorldPosition() - GetWorldPosition());
        DestBullet->ChangeAnimation("NueBoss_AttackBall");

        CProfileManager::GetInst()->SetInteraction("BossAttack", ECollisionChannel::BossMonster,
            ECollisionInteraction::Collision);

    }
}

void CPlayerObject::CollisionPlayer(const FVector3D& HitPoint, 
    CColliderBase* Dest)
{
    FCollisionProfile* DestProfile = Dest->GetProfile();
    
    if (DestProfile->Channel == ECollisionChannel::Monster 
        || DestProfile->Channel == ECollisionChannel::MonsterAttack 
        || DestProfile->Channel == ECollisionChannel::BossMonster
        || DestProfile->Channel == ECollisionChannel::BossAttack)
    {   
        if (mPlayerData->Hitting || mIsKnockBack)
            return;

        if (!Dest)
            return;

        FVector3D DestPos = Dest->GetWorldPosition();
        FVector3D KnockbackDir = GetWorldPosition() - DestPos;
        KnockbackDir.Normalize();

        float KnockbackStrength = 300.f; 
        mKnockBackVelocity = KnockbackDir * KnockbackStrength;

        mIsKnockBack = true;

        mStateMachine->ChangeState(EObjectState::Hit, mPlayerData->Dir);

        AddHP(-1);
    }
    
    else if (DestProfile->Channel == ECollisionChannel::NPC)
    {
        // 만약 NPC와 충돌중이라면 
        // 플레이어를 대화 가능상태로 만들어준다.
        mPlayerData->CanSpeak = true;

        CNPCObject* NPC = dynamic_cast<CNPCObject*>(Dest->GetOwner());

        // 현재 충돌한 NPC의 아이디를 얻어온다.
        if (NPC)
            mPlayerData->CurrentNPCID = NPC->GetNPCID();

    }
}

void CPlayerObject::CollisionPlayerEnd(CColliderBase* Dest)
{
    // 만약 대화 가능상태였다면
    // 대화불가 및 상호작용 상태 해제
    if(mPlayerData->CanSpeak)
    {
        mPlayerData->CanSpeak = false;
    }
}

void CPlayerObject::AttackEnd()
{
    mRoot->SetWorldScale(150.f, 150.f);
    mRoot->SetPivot(0.5f, 0.5f);

    // 충돌체 비활성화.
    mAttackRange->SetEnable(false);

    // 공격이 끝났다면 
    // 이동 가능, 공격 가능
    mPlayerData->Attacking = false;
    mPlayerData->CanMove = true;
    mPlayerData->CanAttack = true;
}
void CPlayerObject::AttackNotify()
{
    mAttackRange->SetEnable(true);

    mPlayerData->CanAttack = false;
    mPlayerData->CanMove = false;
}

void CPlayerObject::QuickSlotInteraction(int Index)
{
    CMainWidget* UI = mScene->GetMainWidget();
    CInventory* InventoryUI = UI->GetInventoryUI();

    // 만약 인벤토리창이 현재 활성화 상태라면
    // 퀵슬롯에 아이템을 등록한다.
    if (InventoryUI && InventoryUI->IsEnable())
    {
        // 현재 인벤토리 창에서 선택된 아이템을 얻어온다.
        FItemData* Selected = InventoryUI->GetSelectedItem();
        // 만약 선택된 아이템이 있고 사용할 수 있는 아이템이라면
        // 퀵슬롯에 등록한다.
        if (Selected && Selected->Useable)
        {
            RegisterQuickSlot(Index, Selected);
            mScene->GetAssetManager()->FindSound("QuickSlotAdd")->Play();
        }
    }
    // 인벤토리 창이 비활성화면 퀵슬롯에 있는 아이템을 사용한다.
    else
    {
        UseQuickSlot(Index);
    }
}
