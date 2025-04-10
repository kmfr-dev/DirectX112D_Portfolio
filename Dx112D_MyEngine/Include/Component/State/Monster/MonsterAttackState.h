#pragma once
#include "../State.h"

class CMonsterAttackState :
    public CState
{
    friend class CState;

private:
    float mAttackTimer = 3.f;

public:
    void Start() override;
    void Update(float DeltaTime) override;
    void End() override;
};

