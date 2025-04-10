#include "PlayerAttackState.h"
#include "../../../Object/SceneObject.h"
#include "../../StateMachineComponent.h"
#include "../../SpriteComponent.h"
#include "../../../Object/PlayerObject.h"
#include "../../ColliderBase.h"


#include "../../../Scene/SceneAssetManager.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Scene/Scene.h"
#include "../../../Asset/Sound/SoundManager.h"

void CPlayerAttackState::Start()
{
    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(mOwner);
    CSpriteComponent* PlayerRoot = dynamic_cast<CSpriteComponent*>(Player->GetRootComponent());

    if (!Player || !PlayerRoot)
        return;

    PlayerRoot->SetWorldScale(200.f, 200.f);

    EObjectDir Dir = Player->FindNonSceneComponent<CStateMachineComponent>()->GetCurrentDir();
    FVector3D Offset;

    float Distance = 50.f;
    float Diagonal = Distance * 0.7071f; 

    switch (Dir)
    {
    case EObjectDir::T:
        Player->ChangeAnimation("Player_AttackUp");
        Offset = FVector3D(0.f, Distance, 0.f);
        break;
    case EObjectDir::B:
        Player->ChangeAnimation("Player_AttackDown");
        Offset = FVector3D(0.f, -Distance, 0.f);
        break;
    case EObjectDir::L:
        Player->ChangeAnimation("Player_AttackLeft");
        Offset = FVector3D(-Distance, 0.f, 0.f);
        break;
    case EObjectDir::R:
        Player->ChangeAnimation("Player_AttackRight");
        Offset = FVector3D(Distance, 0.f, 0.f);
        break;
    case EObjectDir::TL:
        Player->ChangeAnimation("Player_AttackUpLeft");
        Offset = FVector3D(-Diagonal, Diagonal, 0.f);
        PlayerRoot->SetPivot(0.55f, 0.40f);
        break;
    case EObjectDir::TR:
        Player->ChangeAnimation("Player_AttackUpRight");
        Offset = FVector3D(Diagonal, Diagonal, 0.f);
        PlayerRoot->SetPivot(0.45f, 0.45f);
        break;
    case EObjectDir::BL:
        Player->ChangeAnimation("Player_AttackDownLeft");
        Offset = FVector3D(-Diagonal, -Diagonal, 0.f);
        PlayerRoot->SetPivot(0.55f, 0.5f);
        break;
    case EObjectDir::BR:
        Player->ChangeAnimation("Player_AttackDownRight");
        Offset = FVector3D(Diagonal, -Diagonal, 0.f);
        PlayerRoot->SetPivot(0.45f, 0.5f);
        break;
    }

    Player->GetAttackCollider()->
        SetRelativePos(Offset);

    Player->SetAttacking(true);
    Player->SetCanAttack(false);
    Player->SetCanMove(false);
    mAttackTimer = 1.2f;

    CSceneManager::GetInst()->GetCurrentScene()->GetAssetManager()
        ->FindSound("PlayerAttack")->Play();
}

void CPlayerAttackState::Update(float DeltaTime)
{
    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(mOwner);

    if (!Player)
        return;

    mAttackTimer -= DeltaTime;
    if (mAttackTimer <= 0.f)
    {
        Player->SetCanAttack(true);
        Player->SetAttacking(false);
    }
    

    if (!Player->IsAttacking())
    {
        EObjectDir Dir = Player->GetCurrentDir();
        Player->FindNonSceneComponent<CStateMachineComponent>()->ChangeState(EObjectState::Idle, Dir);
    }
}

void CPlayerAttackState::End()
{
    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(mOwner);
    CSpriteComponent* PlayerRoot = dynamic_cast<CSpriteComponent*>(Player->GetRootComponent());

    if (!Player || !PlayerRoot)
        return;

    // 이동 상태가 끝날 때 마지막 이동 방향을 저장
    EObjectDir LastDir = Player->FindNonSceneComponent< CStateMachineComponent>()->GetCurrentDir();
    Player->FindNonSceneComponent< CStateMachineComponent>()->SetCurrentDir(LastDir);
    PlayerRoot->SetWorldScale(150.f, 150.f);
    PlayerRoot->SetPivot(0.5f, 0.45f);
    Player->GetAttackCollider()->SetEnable(false);
    Player->SetAttacking(false);
    Player->SetCanAttack(true);
    Player->SetCanMove(true);
    mAttackTimer = 1.2f;

    CSceneManager::GetInst()->GetCurrentScene()->GetAssetManager()
        ->FindSound("PlayerAttack")->Stop();
    
}
