#include "MonsterDeathState.h"
#include "../../StateMachineComponent.h"
#include "../../../Object/MonsterObject.h"
#include "../../../Object/NearingMonster.h"
#include "../../../Object/MageMonster.h"
void CMonsterDeathState::Start()
{
    CMonsterObject* Monster = dynamic_cast<CMonsterObject*>(mOwner);

    if (!Monster)
        return;

    Monster->ChangeAnimation("NormalMonster_Death");
    Monster->AIDeath();
    Monster->SetLifeTime(2.f);

}

void CMonsterDeathState::Update(float DeltaTime)
{
}

void CMonsterDeathState::End()
{
}
