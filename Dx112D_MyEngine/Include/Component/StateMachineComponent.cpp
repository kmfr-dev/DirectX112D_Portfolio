#include "StateMachineComponent.h"
#include "SceneComponent.h"
#include "State/Player/PlayerIdleState.h"
#include "State/Player/PlayerWalkState.h"
#include "State/Player/PlayerRunState.h"
#include "State/Player/PlayerAttackState.h"
#include "State/Player/PlayerHurtState.h"

#include "State/Monster/MonsterIdleState.h"
#include "State/Monster/MonsterTraceState.h"
#include "State/Monster/MonsterAttackState.h"
#include "State/Monster/MonsterPatrolState.h"
#include "State/Monster/MonsterDeathState.h"

#include "State/BossMonster/NueBossReadyState.h"
#include "State/BossMonster/NueBossIdleState.h"

#include "State/BossMonster/NueBossSwingState.h"

#include "State/BossMonster/NueBossSPPReadyState.h"
#include "State/BossMonster/NueBossSPPReady2State.h"
#include "State/BossMonster/NueBossSPPWaitState.h"
#include "State/BossMonster/NueBossSpecialPatternState.h"

#include "State/BossMonster/NueBossGrogyState.h"
#include "State/BossMonster/NueBossDeathState.h"

CStateMachineComponent::CStateMachineComponent()
{
}

CStateMachineComponent::~CStateMachineComponent()
{
    for (auto& State : mStates)
    {
        SAFE_DELETE(State.second);
    }
    for (auto& State : mMonsterStates)
    {
        SAFE_DELETE(State.second);
    }
    for (auto& State : mBossMonsterStates)
    {
        SAFE_DELETE(State.second);
    }
}

void CStateMachineComponent::InitStates(EObjectType Type)
{
    switch (Type)
    {
    case EObjectType::Player:
        AddState(EObjectState::Idle,    new CPlayerIdleState);
        AddState(EObjectState::Walk,    new CPlayerWalkState);
        AddState(EObjectState::Run,     new CPlayerRunState);
        AddState(EObjectState::Attack,  new CPlayerAttackState);
        AddState(EObjectState::Hit,     new CPlayerHurtState);
        break;
    case EObjectType::Monster:
        AddStateMonster(EMonsterAIState::Idle, new CMonsterIdleState);
        AddStateMonster(EMonsterAIState::Trace, new CMonsterTraceState);
        AddStateMonster(EMonsterAIState::Patrol, new CMonsterPatrolState);
        AddStateMonster(EMonsterAIState::Death, new CMonsterDeathState);
        AddStateMonster(EMonsterAIState::Attack, new CMonsterAttackState);
        break;
    case EObjectType::BossMonster:
        // 0 Ready          / 처음 등장 애니메이션
       // 1 Idle           / 보스 기본 Idle
       // 2 Swing          / 기본 공격 애니메이션
       // 3 SpecialPatternReady / 특수 패턴 전 Ready 애니메이션
       // 4 SpecialPattern / 특수패턴 애니메이션
       // 5 PatternReady   / 기본 패턴전 Ready 애니메이션
       // 6 Grogy          / 그로기 애니메이션
       // 7 Death          / 죽는 애니메이션
        AddStateBossMonster(EBossMonsterAIState::Start, new CNueBossReadyState);
        AddStateBossMonster(EBossMonsterAIState::Idle, new CNueBossIdleState);

        AddStateBossMonster(EBossMonsterAIState::Attack, new CNueBossSwingState);
        AddStateBossMonster(EBossMonsterAIState::Grogy, new CNueBossGrogyState);
        AddStateBossMonster(EBossMonsterAIState::Death, new CNueBossDeathState);

        AddStateBossMonster(EBossMonsterAIState::Ready, new CNueBossSPPReadyState);
        AddStateBossMonster(EBossMonsterAIState::Ready2, new CNueBossSPPReady2State);
        AddStateBossMonster(EBossMonsterAIState::Wait, new CNueBossSPPWaitState);

        AddStateBossMonster(EBossMonsterAIState::SpecialPattern, new CNueBossSpecialPatternState);
        break;
    case EObjectType::NPC:
        break;
    }
}

void CStateMachineComponent::AddState(EObjectState StateType, CState* State)
{
    mStates[StateType] = State;
}
void CStateMachineComponent::AddStateMonster(EMonsterAIState StateType, CState* State)
{
    mMonsterStates[StateType] = State;
}
void CStateMachineComponent::AddStateBossMonster(EBossMonsterAIState StateType, CState* State)
{
    mBossMonsterStates[StateType] = State;
}

void CStateMachineComponent::ChangeState(EObjectState NewState, EObjectDir Dir)
{
    if (mCurrentState)
        mCurrentState->End();
    else
        mCurrentDir = Dir;
  
    mCurrentState = mStates[NewState];

    // 이전 방향을 유지
    if (NewState == EObjectState::Idle || 
        NewState == EObjectState::Attack || 
        NewState == EObjectState::Hit)
        Dir = mCurrentDir;

    mCurrentDir = Dir;

    if (mCurrentState)
        mCurrentState->SetOwner(mOwnerObject);

    mCurrentState->Start();
}
void CStateMachineComponent::ChangeStateMonster(EMonsterAIState NewState, EObjectDir Dir)
{
    if (mCurrentState)
        mCurrentState->End();
    else
        mCurrentDir = Dir;

    mCurrentState = mMonsterStates[NewState];

    // 이전 방향을 유지
    if (NewState == EMonsterAIState::Idle ||
        NewState == EMonsterAIState::Attack ||
        NewState == EMonsterAIState::Patrol || 
        NewState == EMonsterAIState::Trace)
        Dir = mCurrentDir;

    mCurrentDir = Dir;

    if (mCurrentState)
        mCurrentState->SetOwner(mOwnerObject);

    mCurrentState->Start();
}
void CStateMachineComponent::ChangeStateBossMonster(EBossMonsterAIState NewState)
{
    if (mCurrentState)
        mCurrentState->End();

    mCurrentState = mBossMonsterStates[NewState];

    if (mCurrentState)
        mCurrentState->SetOwner(mOwnerObject);

    mCurrentState->Start();
}


EObjectDir CStateMachineComponent::GetConvertToDir(const FVector3D& Dir)
{
    float Angle = atan2(Dir.y, Dir.x) * (180.0f / 3.141592f);

    if (Angle >= -22.5f && Angle < 22.5f)
        return EObjectDir::R;

    else if (Angle >= 22.5f && Angle < 67.5f)
        return EObjectDir::TR;

    else if (Angle >= 67.5f && Angle < 112.5f)
        return EObjectDir::T;

    else if (Angle >= 112.5f && Angle < 157.5f)
        return EObjectDir::TL;

    else if (Angle >= -67.5f && Angle < -22.5f)
        return EObjectDir::BR;

    else if (Angle >= -112.5f && Angle < -67.5f)
        return EObjectDir::B;

    else if (Angle >= -157.5f && Angle < -112.5f)
        return EObjectDir::BL;
    else
        return EObjectDir::L;
}

void CStateMachineComponent::SetAnimationByDir(const FVector3D& Dir)
{
    float Angle = atan2(Dir.y, Dir.x) * (180.f / 3.141592f);

        if (Angle >= -22.5f && Angle < 22.5f)
            mCurrentDir = EObjectDir::R;
       
        else if (Angle >= 22.5f && Angle < 67.5f)
            mCurrentDir = EObjectDir::TR;

        else if (Angle >= 67.5f && Angle < 112.5f)
            mCurrentDir = EObjectDir::T;

        else if (Angle >= 112.5f && Angle < 157.5f)
            mCurrentDir = EObjectDir::TL;

        else if (Angle >= 157.5f || Angle < -157.5f)
            mCurrentDir = EObjectDir::L;

        else if (Angle >= -157.5f && Angle < -112.5f)
            mCurrentDir = EObjectDir::BL;

        else if (Angle >= -112.5f && Angle < -67.5f)
            mCurrentDir = EObjectDir::B;

        else if (Angle >= -67.5f && Angle < -22.5f)
            mCurrentDir = EObjectDir::BR;
}

bool CStateMachineComponent::Init()
{
    if (!CComponent::Init())
        return false;

    return true;
}

bool CStateMachineComponent::Init(const char* FileName)
{
    if (!CComponent::Init(FileName))
        return false;

    return true;
}

void CStateMachineComponent::PreUpdate(float DeltaTime)
{
    CComponent::PreUpdate(DeltaTime);
}

void CStateMachineComponent::Update(float DeltaTime)
{
    CComponent::Update(DeltaTime);

    mCurrentState->Update(DeltaTime);
}

void CStateMachineComponent::PostUpdate(float DeltaTime)
{
    CComponent::PostUpdate(DeltaTime);
}

CStateMachineComponent* CStateMachineComponent::Clone()
{
    return new CStateMachineComponent(*this);
}

void CStateMachineComponent::EndFrame()
{
    CComponent::EndFrame();

}
