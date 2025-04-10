#pragma once
#include "../State.h"

class CNueBossIdleState :
    public CState
{
    friend class CState;

private:
    float mPatternTimer = 10.f;

public:
    void Start() override;
    void Update(float DeltaTime) override;
    void End() override;
};

