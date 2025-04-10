#include "NueBossReadyState.h"
#include "../../StateMachineComponent.h"

#include "../../../Object/NueBossMonster.h"

void CNueBossReadyState::Start()
{
	CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);

	if (!Boss)
		return;

	Boss->ChangeAnimation("NueBossMonster_Ready");
	mReadyTimer = 1.f;
}

void CNueBossReadyState::Update(float DeltaTime)
{
	mReadyTimer -= DeltaTime;

	if (mReadyTimer <= 0.f)
	{
		CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);

		if (!Boss)
			return;

		Boss->FindNonSceneComponent<CStateMachineComponent>()->ChangeStateBossMonster(EBossMonsterAIState::Idle);
	}
}

void CNueBossReadyState::End()
{
}
