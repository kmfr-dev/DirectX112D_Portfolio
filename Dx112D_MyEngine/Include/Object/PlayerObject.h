#pragma once

#include "SceneObject.h"
#include "ObjectData\PlayerData.h"

class CPlayerObject :
    public CSceneObject
{
    friend class CPlayerManager;
    friend class CScene;

protected:
    CPlayerObject();
    CPlayerObject(const CPlayerObject& Obj);
    CPlayerObject(CPlayerObject&& Obj);
    virtual ~CPlayerObject();

protected:
    CSharedPtr<class CSpriteComponent>      mRoot;
    CSharedPtr<class CColliderAABB2D>        mBody;
    CSharedPtr<class CColliderSphere2D>       mAttackRange;

    //CSharedPtr<class CRotationComponent>    mRotation;
    CSharedPtr<class CSceneComponent>       mRotationPivot;

    CSharedPtr<class CPlayerMovement>    mMovement;
    CSharedPtr<class CInventoryComponent>    mInventory;

    CSharedPtr<class CStateMachineComponent> mStateMachine;

    class CAnimation2D* mAnimation = nullptr;

    CPlayerData*        mPlayerData = nullptr;
    CQuest*             mWaitingRewardQuest = nullptr;

    FVector3D       mKnockBackVelocity;
    bool            mIsKnockBack = false;

    bool                mOpenedQuestUIByDialogue = false;
    bool                mWalkingFlag = false;

public:
    virtual bool  Init();
    virtual bool InitAfter();
    virtual void  PreUpdate(float DeltaTime) override;
    virtual void  Update(float DeltaTime);
    virtual void  Render();
    virtual void  Damage(int Dmg);
    virtual float Damage(float Attack, CSceneObject* Obj);
    virtual void  EndFrame() override;
    virtual void  ConstrainToMap();

    void         ChangeAnimation(std::string AnimName);
    
public:
    void         ApplyData();
    CPlayerData* GetData() const { return mPlayerData; }
    void         SetData(CPlayerData* Data) { mPlayerData = Data; }

public:
    class CInventoryComponent* GetInventory() const;
    class CColliderBase* GetCollider() const override;
    class CColliderBase* GetAttackCollider() const;
    class CPlayerMovement* GetMovement() { return mMovement.Get(); }
    const FVector3D& GetKnockBackVelocity() { return mKnockBackVelocity; }

    
    EObjectDir  GetCurrentDir() { return mPlayerData ? mPlayerData->Dir : EObjectDir::End; }
    int         GetHP() { return mPlayerData ? mPlayerData->HP : 0; }
    int         GetHPMax() { return mPlayerData ? mPlayerData->HPMax : 0; }
    int         GetGold() { return mPlayerData ? mPlayerData->Gold : 0; }
    int         GetCurrentNPCID() { return mPlayerData ? mPlayerData->CurrentNPCID : 0; }
    FItemData*  GetQuickSlotItem(int Index) const { return mPlayerData ? mPlayerData->QuickSlots[Index] : nullptr; }
    const std::list<class CQuest*>& GetQuestList() const { return mPlayerData->mQuestList; }
    const std::vector<int>& GetCompletedQuestList() const { return mPlayerData->mCompletedQuestIDs; }
    class CQuest* FindQuestByID(int QuestID) const;
public:
    void    SetInvincible(bool Invincible);
    void    SetKnockback(bool KnockBack) { mIsKnockBack = KnockBack; }
    void    SetKnockBackVelocity(FVector3D Vector) { mKnockBackVelocity = Vector; }
    
  
    void    SetSceneType(ESceneType SceneType) { mPlayerData->CurrentScene = SceneType; }
    void    SetAttack(int Value) { mPlayerData->Attack = Value; }
    void    SetCanMove(bool Flag) { mPlayerData->CanMove = Flag; }
    void    SetCanAttack(bool Flag) { mPlayerData->CanAttack = Flag; }
    void    SetMoving(bool Flag) { mPlayerData->Moving = Flag; }
    void    SetAttacking(bool Flag) { mPlayerData->Attacking = Flag; }
    void    SetHitting(bool Flag) { mPlayerData->Hitting = Flag; }
    void    SetWalkFlag(bool Flag) { mWalkingFlag = Flag; }

public:
    void AddHP(int HP)
    {
        mPlayerData->HP += HP;

        if (mPlayerData->HP > mPlayerData->HPMax)
            mPlayerData->HP = mPlayerData->HPMax;

        if (mPlayerData->HP < 0)
        {
            mPlayerData->HP = 0;
            mPlayerData->Dead = true;
        }
    }

    void AddGold(int Gold)
    {
        mPlayerData->Gold += Gold;
    }
    
    void AddQuest(class CQuest* Quest);
    void AddCompletedQuest(int QuestID)
    {
        if (std::find(mPlayerData->mCompletedQuestIDs.begin(),
            mPlayerData->mCompletedQuestIDs.end(), 
            QuestID) == mPlayerData->mCompletedQuestIDs.end())
        {
            mPlayerData->mCompletedQuestIDs.push_back(QuestID);
        }
    }

    void RemoveQuest(int QuestID);

public:
    // 퀵슬롯 등록, 퀵슬롯 아이템 사용
    void    RegisterQuickSlot(int Index, FItemData* Item);
    void    UseQuickSlot(int Index);

public:
    bool    IsControl();
    bool    IsKnockBack() { return mIsKnockBack; }
    bool    IsInvincible() { return mPlayerData->Invincible; }
    bool    IsCanMove() { return mPlayerData->CanMove; }
    bool    IsMoving() { return mPlayerData->Moving; };
    bool    IsRunning() { return mPlayerData->Running; }
    bool    IsAttacking() { return mPlayerData->Attacking; }
    bool    IsHitting() { return mPlayerData->Hitting; }
    bool    IsWalkFlag() { return mWalkingFlag; }
    bool    IsQuestCompleted(int QuestId)
    {
        auto iter = mPlayerData->mCompletedQuestIDs.begin();
        auto iterEnd = mPlayerData->mCompletedQuestIDs.end();

        for (; iter != iterEnd; ++iter)
        {
            if ((*iter) == QuestId)
                return true;
        }

        return false;
    }
    
private:
    void    InitAnimAddSequence();
    void    InitAnimBindFunction();
    void    InitBindKey();

private:
    void    MoveUp(float DeltaTime);
    void    MoveDown(float DeltaTime);
    void    MoveLeft(float DeltaTime);
    void    MoveRight(float DeltaTime);

    void    Attack(float DeltaTime);

    void    QuickSlotKey1();
    void    QuickSlotKey2();
    void    QuickSlotKey3();
    void    QuickSlotKey4();

    void    Interaction();
    void    EscapeInteraction();
    void    InventoryUIInteraction();
    void    QuestUIInteraction();
    void    DialogueInteraction();
    
private:
    void CollisionPlayerAttack(const FVector3D& HitPoint,
        class CColliderBase* Dest);
    void CollisionPlayer(const FVector3D& HitPoint,
        class CColliderBase* Dest);
    void CollisionPlayerEnd(class CColliderBase* Dest);

public:
    void    AttackEnd();
    void    AttackNotify();

    void QuickSlotInteraction(int Index);
};

