#include "PlayerIdleState.h"
#include "../../../Object/SceneObject.h"
#include "../../StateMachineComponent.h"
#include "../../../Object/PlayerObject.h"

#include "../../../Scene/SceneAssetManager.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Scene/Scene.h"
#include "../../../Asset/Sound/SoundManager.h"

void CPlayerIdleState::Start()
{
    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(mOwner);

    if (!Player)
        return;

    EObjectDir Dir = mOwner->FindNonSceneComponent<CStateMachineComponent>()->GetCurrentDir();

    switch (Dir)
    {
    case EObjectDir::T:
        Player->ChangeAnimation("Player_IdleUp");
        break;
    case EObjectDir::B:
        Player->ChangeAnimation("Player_IdleDown");
        break;
    case EObjectDir::L:
        Player->ChangeAnimation("Player_IdleLeft");
        break;
    case EObjectDir::R:
        Player->ChangeAnimation("Player_IdleRight");
        break;
    case EObjectDir::TL:
        Player->ChangeAnimation("Player_IdleUpLeft");
        break;
    case EObjectDir::TR:
        Player->ChangeAnimation("Player_IdleUpRight");
        break;
    case EObjectDir::BL:
        Player->ChangeAnimation("Player_IdleDownLeft");
        break;
    case EObjectDir::BR:
        Player->ChangeAnimation("Player_IdleDownRight");
        break;
    }
    CSceneManager::GetInst()->GetCurrentScene()->GetAssetManager()
        ->FindSound("PlayerWalk")->Stop();
}

void CPlayerIdleState::Update(float DeltaTime)
{

    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(mOwner);

    if (!Player)
        return;

    /*if (Player->IsMoving())
    {
       
        CStateMachineComponent* StateMachine =
            Player->FindNonSceneComponent<CStateMachineComponent>();
        EObjectDir Dir = StateMachine->GetCurrentDir();
        
            StateMachine->ChangeState(EObjectState::Walk, Dir);
    }*/
}

void CPlayerIdleState::End()
{
    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(mOwner);
    if (!Player)
        return;

    Player->SetWalkFlag(false);
}
