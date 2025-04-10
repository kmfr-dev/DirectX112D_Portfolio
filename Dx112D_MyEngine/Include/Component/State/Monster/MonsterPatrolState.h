#pragma once
#include "../State.h"

class CMonsterPatrolState :
    public CState
{
    friend class CState;

private:
    FVector3D   mPatrolTarget = FVector3D::Zero;
    bool        mHasPatrolTarget = false;
    float       mPatrolTimer = 3.f;

public:
    void Start() override;
    void Update(float DeltaTime) override;
    void End() override;
};

