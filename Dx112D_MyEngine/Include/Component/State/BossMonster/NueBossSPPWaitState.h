#pragma once
#include "../State.h"

class CNueBossSPPWaitState :
    public CState
{
    friend class CState;

private:
    float mWaitTimer = 1.f;
public:
    void Start() override;
    void Update(float DeltaTime) override;
    void End() override;
};

