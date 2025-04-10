#include "PlayerWalkState.h"
#include "../../StateMachineComponent.h"
#include "../../PlayerMovement.h"
#include "../../../Object/PlayerObject.h"

#include "../../../Scene/SceneAssetManager.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Scene/Scene.h"
#include "../../../Asset/Sound/SoundManager.h"


void CPlayerWalkState::Start()
{
    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(mOwner);

    if (!Player)
        return;

    EObjectDir Dir = mOwner->FindNonSceneComponent<CStateMachineComponent>()->GetCurrentDir();


    switch (Dir)
    {
    case EObjectDir::T:
        Player->ChangeAnimation("Player_WalkUp");
        break;
    case EObjectDir::B:
        Player->ChangeAnimation("Player_WalkDown");
        break;
    case EObjectDir::L:
        Player->ChangeAnimation("Player_WalkLeft");
        break;
    case EObjectDir::R:
        Player->ChangeAnimation("Player_WalkRight");
        break;
    case EObjectDir::TL:
        Player->ChangeAnimation("Player_WalkUpLeft");
        break;
    case EObjectDir::TR:
        Player->ChangeAnimation("Player_WalkUpRight");
        break;
    case EObjectDir::BL:
        Player->ChangeAnimation("Player_WalkDownLeft");
        break;
    case EObjectDir::BR:
        Player->ChangeAnimation("Player_WalkDownRight");
        break;
    }

    if (!Player->IsWalkFlag())
    {
        CSceneManager::GetInst()->GetCurrentScene()->GetAssetManager()
            ->FindSound("PlayerWalk")->Play();
        Player->SetWalkFlag(true);
    }
}

void CPlayerWalkState::Update(float DeltaTime)
{
    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(mOwner);
    if (!Player)
        return;

    if (Player->GetMovement()->GetVelocity().Length() == 0.f)
        Player->SetMoving(false);

    // 만약 이동중이 아니라면 Idle 상태로 전환.
    if (!Player->IsMoving()) 
    {
        CStateMachineComponent* StateMachine =
            Player->FindNonSceneComponent<CStateMachineComponent>();
        EObjectDir Dir = StateMachine->GetCurrentDir(); 
        StateMachine->ChangeState(EObjectState::Idle, Dir);

        Player->SetCanMove(true);
    }
}

void CPlayerWalkState::End()
{

    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(mOwner);
    if (!Player)
        return;

    // 이동 상태가 끝날 때 마지막 이동 방향을 저장
    EObjectDir LastDir = Player->FindNonSceneComponent< CStateMachineComponent>()->GetCurrentDir();
    Player->FindNonSceneComponent< CStateMachineComponent>()->SetCurrentDir(LastDir);

    CSceneManager::GetInst()->GetCurrentScene()->GetAssetManager()
        ->FindSound("PlayerWalk")->Stop();
}
