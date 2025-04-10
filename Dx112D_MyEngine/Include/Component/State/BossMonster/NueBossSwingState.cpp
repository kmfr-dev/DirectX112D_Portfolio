#include "NueBossSwingState.h"
#include "../../StateMachineComponent.h"
#include "../../SceneComponent.h"

#include "../../../Object/NueBossMonster.h"
#include "../../../Object/BossBullet.h"

#include "../../../Asset/Sound/SoundManager.h"
#include "../../../Asset/Sound/Sound.h"
#include "../../../Scene/SceneAssetManager.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Scene/Scene.h"
#include "../../../ProfileManager.h"

void CNueBossSwingState::Start()
{
	CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);

	if (!Boss)
		return;

    FVector3D MonsterPos = Boss->GetWorldPosition();
    FVector3D TargetPos = Boss->GetTarget()->GetWorldPosition();
    FVector3D Direction = TargetPos - MonsterPos;
    Direction.Normalize();

    std::vector<float> Angles = { 0.f, -15.f, 15.f };

    for (float angle : Angles)
    {
        // 각도 → 라디안 변환
        float rad = angle * (3.141592f / 180.f);
        float cosA = cosf(rad);
        float sinA = sinf(rad);

        // 방향을 2D 기준으로 회전 (Z축 회전)
        FVector3D RotatedDir;
        RotatedDir.x = Direction.x * cosA - Direction.y * sinA;
        RotatedDir.y = Direction.x * sinA + Direction.y * cosA;
        RotatedDir.z = 0.f; 

        // 총알 생성
        CBossBullet* Bullet = CSceneManager::GetInst()->GetCurrentScene()->
            CreateObj<CBossBullet>("BossBullet");

        CSceneComponent* Root = Bullet->GetRootComponent();

        FVector3D Pos = Boss->GetWorldPosition();

        Root->SetWorldScale(60.f, 60.f);
        Root->SetWorldRotation(Boss->GetWorldRotation());
        Root->SetWorldPos(Pos + RotatedDir * 75.f); 

        Bullet->SetSpeed(500.f);
        Bullet->SetDirection(RotatedDir);

        Bullet->ChangeAnimation("NueBoss_AttackBall");
        Bullet->SetLifeTime(4.f);
    }

    CProfileManager::GetInst()->SetInteraction("BossAttack", ECollisionChannel::BossMonster,
        ECollisionInteraction::Ignore);
    Boss->ChangeAnimation("NueBossMonster_Swing");
    mAttackTimer = 3.f;

    CSceneManager::GetInst()->GetCurrentScene()->GetAssetManager()
        ->FindSound("NueBossAttack")->Play();
}

void CNueBossSwingState::Update(float DeltaTime)
{
	mAttackTimer -= DeltaTime;

	if (mAttackTimer <= 0.f)
	{
		CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);

		if (!Boss)
			return;

		Boss->FindNonSceneComponent<CStateMachineComponent>()->
			ChangeStateBossMonster(EBossMonsterAIState::Idle);
	}
}

void CNueBossSwingState::End()
{
    CSceneManager::GetInst()->GetCurrentScene()->GetAssetManager()
        ->FindSound("NueBossAttack")->Stop();
}
