#include "NueBossGrogyState.h"
#include "../../StateMachineComponent.h"

#include "../../../Object/NueBossMonster.h"

void CNueBossGrogyState::Start()
{
	CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);

	if (!Boss)
		return;

	Boss->ChangeAnimation("NueBossMonster_Grogy");
}

void CNueBossGrogyState::Update(float DeltaTime)
{
}

void CNueBossGrogyState::End()
{
}
