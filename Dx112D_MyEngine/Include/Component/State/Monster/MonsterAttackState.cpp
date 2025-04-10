#include "MonsterAttackState.h"
#include "../../StateMachineComponent.h"
#include "../../SceneComponent.h"

#include "../../../Object/MonsterObject.h"
#include "../../../Object/NearingMonster.h"
#include "../../../Object/MageMonster.h"
#include "../../../Object/BulletObject.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Scene/Scene.h"

void CMonsterAttackState::Start()
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
        AnimationName += "AttackUp";
        break;
    case EObjectDir::B:
        AnimationName += "AttackDown";
        break;
    case EObjectDir::L:
        AnimationName += "AttackLeft";
        break;
    case EObjectDir::R:
        AnimationName += "AttackRight";
        break;
    case EObjectDir::TL:
        AnimationName += "AttackUpLeft";
    case EObjectDir::TR:
        AnimationName += "AttackUpRight";
        break;
    case EObjectDir::BL:
        AnimationName += "AttackDownLeft";
        break;
    case EObjectDir::BR:
        AnimationName += "AttackDownRight";
        break;
    }
    Monster->ChangeAnimation(AnimationName);

    Monster->SetAttacking(true);

}

void CMonsterAttackState::Update(float DeltaTime)
{
    CMonsterObject* Monster = dynamic_cast<CMonsterObject*>(mOwner);

    if (!Monster)
        return;
    
    mAttackTimer -= DeltaTime;

    if (mAttackTimer <= 0.f)
    {
        FVector3D MonsterPos = Monster->GetWorldPosition();
        FVector3D TargetPos = Monster->GetTarget()->GetWorldPosition();
        FVector3D Direction = TargetPos - MonsterPos;
        
        Direction.Normalize();

        CBulletObject* Bullet = CSceneManager::GetInst()->GetCurrentScene()->
            CreateObj<CBulletObject>("Bullet");
        
        Bullet->SetBulletCollisionProfile("MonsterAttack");
        
        CSceneComponent* Root = Bullet->GetRootComponent();
        
        FVector3D Pos = Monster->GetWorldPosition();
        
        Root->SetWorldScale(60.f, 60.f);
        Root->SetWorldRotation(Monster->GetWorldRotation());
        Root->SetWorldPos(Pos + Direction * 75.f);
        
        Bullet->SetLifeTime(2.f);
        
        // 총알 방향 & 속도 설정
        Bullet->SetDirection(Direction);
        Bullet->SetSpeed(700.f);

        mAttackTimer = 4.f;

        Monster->FindNonSceneComponent<CStateMachineComponent>()->SetAnimationByDir(Direction);
        Start();

    }


    if (!Monster->IsAttacking())
    {
        EObjectDir Dir = Monster->GetMonsterDir();
        Monster->FindNonSceneComponent<CStateMachineComponent>()->ChangeState(EObjectState::Idle, Dir);
    }
}

void CMonsterAttackState::End()
{
    CMonsterObject* Monster = dynamic_cast<CMonsterObject*>(mOwner);

    if (!Monster)
        return;

    // 이동 상태가 끝날 때 마지막 이동 방향을 저장
    EObjectDir LastDir = Monster->FindNonSceneComponent< CStateMachineComponent>()->GetCurrentDir();
    Monster->FindNonSceneComponent< CStateMachineComponent>()->SetCurrentDir(LastDir);

    Monster->SetAttacking(false);
}
