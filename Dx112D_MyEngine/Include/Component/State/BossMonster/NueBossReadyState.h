#pragma once
#include "../State.h"

class CNueBossReadyState :
    public CState
{
    friend class CState;

private:
    float mReadyTimer = 1.f;
public:
    void Start() override;
    void Update(float DeltaTime) override;
    void End() override;
};

