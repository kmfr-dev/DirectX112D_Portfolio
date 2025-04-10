#include "MonsterIdleState.h"
#include "../../StateMachineComponent.h"
#include "../../../Object/MonsterObject.h"
#include "../../../Object/NearingMonster.h"
#include "../../../Object/MageMonster.h"

void CMonsterIdleState::Start()
{
    CMonsterObject* Monster = dynamic_cast<CMonsterObject*>(mOwner);
    
    if (!Monster)
        return;

    std::string AnimationName;

    Monster->GetMonsterType();

    switch (Monster->GetMonsterType())
    {
    case EMonsterType::Mage:
        AnimationName = "MageMonster_";
        break;
    case EMonsterType::Nearing:
        AnimationName = "NearingMonster_";
        break;
    }
    

    EObjectDir Dir = Monster->FindNonSceneComponent<CStateMachineComponent>()->GetCurrentDir();

    switch (Dir)
    {
    case EObjectDir::T:
        AnimationName += "IdleUp";
        break;
    case EObjectDir::B:
        AnimationName += "IdleDown";
        break;
    case EObjectDir::L:
        AnimationName += "IdleLeft";
        break;
    case EObjectDir::R:
        AnimationName += "IdleRight";
        break;
    case EObjectDir::TL:
        AnimationName += "IdleUpLeft";
    case EObjectDir::TR:
        AnimationName += "IdleUpRight";
        break;
    case EObjectDir::BL:
        AnimationName += "IdleDownLeft";
        break;
    case EObjectDir::BR:
        AnimationName += "IdleDownRight";
        break;
    }
    Monster->ChangeAnimation(AnimationName);
}

void CMonsterIdleState::Update(float DeltaTime)
{
    CMonsterObject* Monster = dynamic_cast<CMonsterObject*>(mOwner);

    EObjectDir Dir = Monster->GetMonsterDir();
    if (!Monster)
        return;

    if (Monster->IsTracing())
    {
        Monster->FindNonSceneComponent<CStateMachineComponent>()->ChangeStateMonster(EMonsterAIState::Trace, Dir);
    }

    else
    {
        mPatrolTimer -= DeltaTime;

        if (mPatrolTimer <= 0.f)
        {
            EObjectDir Dir = Monster->GetMonsterDir();
            Monster->FindNonSceneComponent<CStateMachineComponent>()->ChangeStateMonster(EMonsterAIState::Patrol, Dir);
        }
    }
        
}

void CMonsterIdleState::End()
{
    CMonsterObject* Monster = dynamic_cast<CMonsterObject*>(mOwner);
    mPatrolTimer = 3.f;
}
