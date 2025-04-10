#include "NueBossDeathState.h"
#include "../../StateMachineComponent.h"

#include "../../../Object/NueBossMonster.h"

void CNueBossDeathState::Start()
{
	CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);

	if (!Boss)
		return;

	Boss->ChangeAnimation("NueBossMonster_Death");
	Boss->AIDeath();

	Boss->SetLifeTime(2.f);
}

void CNueBossDeathState::Update(float DeltaTime)
{
}

void CNueBossDeathState::End()
{
}
