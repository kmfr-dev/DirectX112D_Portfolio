#pragma once
#include "../State.h"

class CMonsterIdleState :
    public CState
{
    friend class CState;

private:
    float mPatrolTimer = 3.f;

public:
    void Start() override;
    void Update(float DeltaTime) override;
    void End() override;
};

