#include "MonsterTraceState.h"
#include "../../StateMachineComponent.h"
#include "../../MonsterMovement.h"
#include "../../../Object/MonsterObject.h"
#include "../../../Object/NearingMonster.h"
#include "../../../Object/MageMonster.h"

void CMonsterTraceState::Start()
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
    Monster->SetTracing(true);
}

void CMonsterTraceState::Update(float DeltaTime)
{
    //CMonsterObject* Monster = dynamic_cast<CMonsterObject*>(mOwner);

    //if (!Monster)
    //    return;

    //CSceneObject* Target = Monster->GetTarget();

    //if (Target)
    //{
    //    // 몬스터와 타겟의 위치 벡터 계산
    //    FVector3D MonsterPos = Monster->GetWorldPosition();
    //    FVector3D TargetPos = Target->GetWorldPosition();
    //    FVector3D Direction = TargetPos - MonsterPos;

    //    Direction.Normalize();

    //    // 이동 속도 적용
    //    float Speed = 100.f;
    //    MonsterPos += Direction * Speed * DeltaTime;
    //    Monster->SetWorldPos(MonsterPos);

    //    // 이동 방향을 기반으로 애니메이션 설정
    //    Monster->FindNonSceneComponent<CStateMachineComponent>()->
    //        SetAnimationByDir(Direction);

    //    Start();
    //}
    //if (!Monster->IsTracing())
    //{
    //    EObjectDir Dir = Monster->GetMonsterDir();
    //    Monster->FindNonSceneComponent<CStateMachineComponent>()->ChangeState(EObjectState::Idle, Dir);
    //}
    CMonsterObject* Monster = dynamic_cast<CMonsterObject*>(mOwner);
    if (!Monster)
        return;

    CSceneObject* Target = Monster->GetTarget();
    if (Target)
    {
        FVector3D MonsterPos = Monster->GetWorldPosition();
        FVector3D TargetPos = Target->GetWorldPosition();
        FVector3D Direction = TargetPos - MonsterPos;
        Direction.Normalize();

        CMonsterMovement* MoveComp = Monster->GetMovement();
        if (MoveComp)
        {
            MoveComp->SetMoveAxis(EAxis::None);
            MoveComp->AddMove(Direction); // ✔ 이동 방향만 넘기면 내부에서 처리됨
        }

        Monster->FindNonSceneComponent<CStateMachineComponent>()->SetAnimationByDir(Direction);
        Start();
    }

    if (!Monster->IsTracing())
    {
        EObjectDir Dir = Monster->GetMonsterDir();
        Monster->FindNonSceneComponent<CStateMachineComponent>()->ChangeState(EObjectState::Idle, Dir);
    }
}

void CMonsterTraceState::End()
{
    CMonsterObject* Monster = dynamic_cast<CMonsterObject*>(mOwner);

    if (!Monster)
        return;

    // 이동 상태가 끝날 때 마지막 이동 방향을 저장
    EObjectDir LastDir = Monster->FindNonSceneComponent< CStateMachineComponent>()->GetCurrentDir();
    Monster->FindNonSceneComponent< CStateMachineComponent>()->SetCurrentDir(LastDir);

    Monster->SetTracing(false);
}
