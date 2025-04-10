#include "PlayerHurtState.h"
#include "../../../Object/SceneObject.h"
#include "../../StateMachineComponent.h"
#include "../../PlayerMovement.h"
#include "../../../Object/PlayerObject.h"

#include "../../../Scene/SceneAssetManager.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Scene/Scene.h"
#include "../../../Asset/Sound/SoundManager.h"

void CPlayerHurtState::Start()
{
    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(mOwner);

    if (!Player)
        return;

    EObjectDir Dir = mOwner->FindNonSceneComponent<CStateMachineComponent>()->GetCurrentDir();

    switch (Dir)
    {
    case EObjectDir::T:
        Player->ChangeAnimation("Player_HurtUp");
        break;
    case EObjectDir::B:
        Player->ChangeAnimation("Player_HurtDown");
        break;
    case EObjectDir::L:
        Player->ChangeAnimation("Player_HurtLeft");
        break;
    case EObjectDir::R:
        Player->ChangeAnimation("Player_HurtRight");
        break;
    case EObjectDir::TL:
        Player->ChangeAnimation("Player_HurtUpLeft");
        break;
    case EObjectDir::TR:
        Player->ChangeAnimation("Player_HurtUpRight");
        break;
    case EObjectDir::BL:
        Player->ChangeAnimation("Player_HurtDownLeft");
        break;
    case EObjectDir::BR:
        Player->ChangeAnimation("Player_HurtDownLeft");
        break;
    }
    mHurtTimer = 1.f;

   Player->SetHitting(true);
   Player->SetCanMove(false);

   CSceneManager::GetInst()->GetCurrentScene()->GetAssetManager()
       ->FindSound("PlayerHit")->Play();
}

void CPlayerHurtState::Update(float DeltaTime)
{
    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(mOwner);

    if (!Player)
        return;

    float Friction = 600.f;
    FVector3D KnockBackVelocity = Player->GetKnockBackVelocity();
    float Speed = KnockBackVelocity.Length();

    if (Speed > 0.f)
    {
        Speed -= Friction * DeltaTime;
        if (Speed < 0.f) Speed = 0.f;

        KnockBackVelocity = FVector3D::Normalize(KnockBackVelocity) * Speed;
        FVector3D MoveStep = KnockBackVelocity * DeltaTime;

        // 타일 충돌 체크 후 이동
        CPlayerMovement* Movement = Player->GetMovement();
        if (Movement)
        {
            Movement->AddMove(KnockBackVelocity);
            if (!Movement->CheckBlockedByTile())
            {
                Player->AddWorldPos(MoveStep);
            }
        }

        Player->SetKnockBackVelocity(KnockBackVelocity);

        if (Speed == 0.f)
        {
            Player->SetKnockback(false);
            Player->SetInvincible(true);
        }
    }

    if (Player->IsInvincible())
    {
        mHurtTimer -= DeltaTime;

        if (mHurtTimer <= 0.f)
        {
            Player->SetHitting(false);
            Player->SetInvincible(false);
        }
    }

    if (!Player->IsKnockBack() && !Player->IsHitting())
    {
        EObjectDir Dir = Player->GetCurrentDir();
        Player->FindNonSceneComponent<CStateMachineComponent>()->ChangeState(EObjectState::Idle, Dir);
    }
        
}

void CPlayerHurtState::End()
{
    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(mOwner);
    if (!Player)
        return;

    // 이동 상태가 끝날 때 마지막 이동 방향을 저장
    EObjectDir LastDir = Player->FindNonSceneComponent< CStateMachineComponent>()->GetCurrentDir();
    Player->FindNonSceneComponent< CStateMachineComponent>()->SetCurrentDir(LastDir);
    Player->SetCanMove(true);
    Player->SetInvincible(false);
    Player->SetCanAttack(true);
    
    CSceneManager::GetInst()->GetCurrentScene()->GetAssetManager()
        ->FindSound("PlayerHit")->Stop();
}
