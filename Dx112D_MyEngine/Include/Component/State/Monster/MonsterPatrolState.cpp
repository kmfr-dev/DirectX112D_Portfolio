#include "MonsterPatrolState.h"
#include "../../StateMachineComponent.h"
#include "../../MonsterMovement.h"
#include "../../../Object/MonsterObject.h"
#include "../../../Object/NearingMonster.h"
#include "../../../Object/MageMonster.h"
void CMonsterPatrolState::Start()
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

void CMonsterPatrolState::Update(float DeltaTime)
{
    //CMonsterObject* Monster = dynamic_cast<CMonsterObject*>(mOwner);


    //if (!Monster)
    //    return;

    //// 몬스터가 추격중이면 추격상태로 변경.
    //if (Monster->IsTracing())
    //{
    //    EObjectDir Dir = Monster->GetMonsterDir();
    //    Monster->FindNonSceneComponent<CStateMachineComponent>()->ChangeStateMonster(EMonsterAIState::Trace, EObjectDir::R);
    //}
    //// 몬스터가 추격상태가 아니면 순찰 시작.
    //else
    //{
    //    if (!mHasPatrolTarget || (Monster->GetWorldPosition() - mPatrolTarget).Length() < 10.f)
    //    {
    //        // 랜덤한 좌표 생성 (맵 범위 내에서)
    //        // 순찰 범위 조정 가능
    //        float PatrolRange = 500.f;
    //        FVector3D CurrentPos = Monster->GetWorldPosition();

    //        float RandomX = CurrentPos.x + (rand() % static_cast<int>(PatrolRange * 2)) - PatrolRange;
    //        float RandomY = CurrentPos.y + (rand() % static_cast<int>(PatrolRange * 2)) - PatrolRange;

    //        mPatrolTarget = FVector3D(RandomX, RandomY, CurrentPos.z);
    //        mHasPatrolTarget = true;
    //    }

    //    // 현재 위치에서 목표 위치로 이동
    //    FVector3D Direction = (mPatrolTarget - Monster->GetWorldPosition());

    //    Direction.Normalize();
    //    float Speed = 100.f;

    //    Monster->SetWorldPos(Monster->GetWorldPosition() + Direction * Speed * DeltaTime);
    //    Monster->FindNonSceneComponent<CStateMachineComponent>()->SetAnimationByDir(Direction);
    //   

    //    if ((Monster->GetWorldPosition() - mPatrolTarget).Length() < 10.f)
    //    {
    //        mPatrolTimer = 5.f;
    //        mHasPatrolTarget = false;
    //    }

    //    Start();
    //    
    //}

    CMonsterObject* Monster = dynamic_cast<CMonsterObject*>(mOwner);
    if (!Monster)
        return;

    if (Monster->IsTracing())
    {
        EObjectDir Dir = Monster->GetMonsterDir();
        Monster->FindNonSceneComponent<CStateMachineComponent>()->ChangeStateMonster(EMonsterAIState::Trace, EObjectDir::R);
        return;
    }

    if (!mHasPatrolTarget || (Monster->GetWorldPosition() - mPatrolTarget).Length() < 10.f)
    {
        float PatrolRange = 500.f;
        FVector3D CurrentPos = Monster->GetWorldPosition();

        float RandomX = CurrentPos.x + (rand() % static_cast<int>(PatrolRange * 2)) - PatrolRange;
        float RandomY = CurrentPos.y + (rand() % static_cast<int>(PatrolRange * 2)) - PatrolRange;

        mPatrolTarget = FVector3D(RandomX, RandomY, CurrentPos.z);
        mHasPatrolTarget = true;
    }

    FVector3D Direction = mPatrolTarget - Monster->GetWorldPosition();
    Direction.Normalize();

    CMonsterMovement* MoveComp = Monster->GetMovement();
    if (MoveComp)
    {
        MoveComp->SetMoveAxis(EAxis::None);
        MoveComp->AddMove(Direction); // ✔ 이동 방향만 넘기면 내부에서 다 처리됨
    }

    Monster->FindNonSceneComponent<CStateMachineComponent>()->SetAnimationByDir(Direction);

    if ((Monster->GetWorldPosition() - mPatrolTarget).Length() < 10.f)
    {
        mPatrolTimer = 5.f;
        mHasPatrolTarget = false;
    }

    Start();
}

void CMonsterPatrolState::End()
{
    CMonsterObject* Monster = dynamic_cast<CMonsterObject*>(mOwner);

    if (!Monster)
        return;;

    // 마지막 이동 방향을 저장
    EObjectDir LastDir = Monster->FindNonSceneComponent< CStateMachineComponent>()->GetCurrentDir();
    Monster->FindNonSceneComponent< CStateMachineComponent>()->SetCurrentDir(LastDir);

}
