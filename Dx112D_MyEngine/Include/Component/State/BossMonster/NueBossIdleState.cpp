#include "NueBossIdleState.h"
#include "../../StateMachineComponent.h"

#include "../../../Object/NueBossMonster.h"
void CNueBossIdleState::Start()
{
	CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);

	if (!Boss)
		return;

	Boss->ChangeAnimation("NueBossMonster_Idle");
	mPatternTimer = 10.f;
}

void CNueBossIdleState::Update(float DeltaTime)
{
	CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);

	if (!Boss)
		return;

	mPatternTimer -= DeltaTime;
	if (mPatternTimer <= 0.f)
	{
		if (!Boss->IsUseSpecialPattern())
		{
			Boss->FindNonSceneComponent<CStateMachineComponent>()->
				ChangeStateBossMonster(EBossMonsterAIState::Attack);
		}

		else
		{
			Boss->FindNonSceneComponent<CStateMachineComponent>()->
				ChangeStateBossMonster(EBossMonsterAIState::Ready);
		}
	}
}

void CNueBossIdleState::End()
{
	
}
