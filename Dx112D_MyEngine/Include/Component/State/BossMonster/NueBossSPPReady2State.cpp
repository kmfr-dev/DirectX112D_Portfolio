#include "NueBossSPPReady2State.h"
#include "../../StateMachineComponent.h"

#include "../../../Object/NueBossMonster.h"

void CNueBossSPPReady2State::Start()
{
	CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);

	if (!Boss)
		return;

	Boss->ChangeAnimation("NueBossMonster_SpecialPatternReady2");
	mReady2Timer = 5.f;
}

void CNueBossSPPReady2State::Update(float DeltaTime)
{

	mReady2Timer -= DeltaTime;

	if (mReady2Timer <= 0.f)
	{
		CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);

		if (!Boss)
			return;

		Boss->FindNonSceneComponent<CStateMachineComponent>()->
			ChangeStateBossMonster(EBossMonsterAIState::Wait);
	}
}

void CNueBossSPPReady2State::End()
{
}
