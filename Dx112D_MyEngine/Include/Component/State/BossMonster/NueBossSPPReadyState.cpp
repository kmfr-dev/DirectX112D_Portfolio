#include "NueBossSPPReadyState.h"
#include "../../StateMachineComponent.h"
#include "../../../Object/NueBossMonster.h"

void CNueBossSPPReadyState::Start()
{
	CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);

	if (!Boss)
		return;


	Boss->ChangeAnimation("NueBossMonster_SpecialPatternReady");
	mSpecialReady = 3.f;
}

void CNueBossSPPReadyState::Update(float DeltaTime)
{
	
	mSpecialReady -= DeltaTime;

	if (mSpecialReady <= 0.f)
	{
		CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);

		if (!Boss)
			return;

		Boss->FindNonSceneComponent<CStateMachineComponent>()->
			ChangeStateBossMonster(EBossMonsterAIState::Ready2);
	}
}

void CNueBossSPPReadyState::End()
{
}
