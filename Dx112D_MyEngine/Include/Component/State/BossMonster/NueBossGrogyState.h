#pragma once
#include "../State.h"
class CNueBossGrogyState :
    public CState
{
    friend class CState;

private:
    float mGrogyTimer = 5.f;

public:
    void Start() override;
    void Update(float DeltaTime) override;
    void End() override;
};