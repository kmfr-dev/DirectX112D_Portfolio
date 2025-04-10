#pragma once
#include "../State.h"

class CNueBossSPPReadyState :
    public CState
{
    friend class CState;

private:
    float mSpecialReady = 3.f;
public:
    void Start() override;
    void Update(float DeltaTime) override;
    void End() override;
};

