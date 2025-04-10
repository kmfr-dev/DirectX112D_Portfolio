#include "NueBossSPPWaitState.h"
#include "../../StateMachineComponent.h"
#include "../../../Object/NueBossMonster.h"
#include "../../../Scene/SceneAssetManager.h"
#include "../../../Scene/SceneManager.h"

#include"../../../Scene/Scene.h"
#include "../../../Asset/Sound/SoundManager.h"
#include "../../../Asset/Sound/Sound.h"

void CNueBossSPPWaitState::Start()
{
	CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);

	if (!Boss)
		return;

	Boss->ChangeAnimation("NueBossMonster_SpecialPatternWait");

	mWaitTimer = 2.f;

	

	CSceneManager::GetInst()->GetCurrentScene()->GetAssetManager()
		->FindSound("NueBossWait")->Play();
}

void CNueBossSPPWaitState::Update(float DeltaTime)
{

	mWaitTimer -= DeltaTime;

	if (mWaitTimer <= 0.f)
	{
		CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);

		if (!Boss)
			return;

		Boss->FindNonSceneComponent<CStateMachineComponent>()->
			ChangeStateBossMonster(EBossMonsterAIState::SpecialPattern);
	}
}

void CNueBossSPPWaitState::End()
{
	CSceneManager::GetInst()->GetCurrentScene()->GetAssetManager()
		->FindSound("NueBossWait")->Stop();
}
