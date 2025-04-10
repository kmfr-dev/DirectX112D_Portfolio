#include "PlayerRunState.h"
#include "../../../Object/SceneObject.h"
#include "../../StateMachineComponent.h"
#include "../../../Object/PlayerObject.h"

void CPlayerRunState::Start()
{
    CStateMachineComponent* StateMachine =
        mOwner->FindNonSceneComponent<CStateMachineComponent>();
    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(mOwner);

    if (!StateMachine || !Player)
        return;

    EObjectDir Dir = mOwner->FindNonSceneComponent<CStateMachineComponent>()->GetCurrentDir();

    switch (Dir)
    {
    case EObjectDir::T:
        Player->ChangeAnimation("Player_RunUp");
        break;
    case EObjectDir::B:
        Player->ChangeAnimation("Player_RunDown");
        break;
    case EObjectDir::L:
        Player->ChangeAnimation("Player_RunLeft");
        break;
    case EObjectDir::R:
        Player->ChangeAnimation("Player_RunDown");
        break;
    case EObjectDir::TL:
        Player->ChangeAnimation("Player_RunUpLeft");
        break;
    case EObjectDir::TR:
        Player->ChangeAnimation("Player_RunUpRight");
        break;
    case EObjectDir::BL:
        Player->ChangeAnimation("Player_RunDownLeft");
        break;
    case EObjectDir::BR:
        Player->ChangeAnimation("Player_RunDownRight");
        break;

        Player->GetData()->Speed = 600.f;
    }
}

void CPlayerRunState::Update(float DeltaTime)
{

    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(mOwner);

    if (!Player)
        return;

    if (!Player->IsRunning())
    {
        EObjectDir Dir = Player->GetCurrentDir();
        Player->FindNonSceneComponent<CStateMachineComponent>()->
            ChangeState(EObjectState::Walk, Dir);
    }
}

void CPlayerRunState::End()
{
    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(mOwner);

    if (!Player)
        return;

    Player->GetData()->Speed = 600.f;
}
