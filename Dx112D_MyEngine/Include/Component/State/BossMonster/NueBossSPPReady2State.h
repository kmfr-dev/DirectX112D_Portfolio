#pragma once
#include "../State.h"

class CNueBossSPPReady2State :
    public CState
{
    friend class CState;

private:
    float mReady2Timer = 5.f;

public:
    void Start() override;
    void Update(float DeltaTime) override;
    void End() override;
};

