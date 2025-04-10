#pragma once

#include "Component.h"
#include "State/State.h"

class CStateMachineComponent :
    public CComponent
{
    friend class CScene;
    friend class CSceneObject;

protected:
    CStateMachineComponent();
    ~CStateMachineComponent();

protected:
    CState*     mCurrentState;
    EObjectDir  mCurrentDir;

    std::unordered_map<EObjectState, CState*> mStates;
    std::unordered_map<EMonsterAIState, CState*> mMonsterStates;
    std::unordered_map<EBossMonsterAIState, CState*> mBossMonsterStates;

public:
    CState* GetCurrentState()
    {
        return mCurrentState;
    }
    EObjectDir GetCurrentDir() const
    {
        return mCurrentDir;
    };

    void SetCurrentDir(EObjectDir Dir)
    {
        mCurrentDir = Dir;
    }

    void    InitStates(EObjectType Type);

    void    AddState(EObjectState StateType, CState* State);
    void    AddStateMonster(EMonsterAIState StateType, CState* State);
    void    AddStateBossMonster(EBossMonsterAIState StateType, CState* State);

    void    ChangeState(EObjectState NewState, EObjectDir Dir);
    void    ChangeStateMonster(EMonsterAIState NewState, EObjectDir Dir);
    void    ChangeStateBossMonster(EBossMonsterAIState NewState);

    EObjectDir GetConvertToDir(const FVector3D& Dir);
    void    SetAnimationByDir(const FVector3D& Dir);

public:
    virtual bool Init();
    virtual bool Init(const char* FileName);
    virtual void PreUpdate(float DeltaTime);
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CStateMachineComponent* Clone();
    virtual void EndFrame();
};

