#pragma once

#include "../State.h"

class CPlayerHurtState :
    public CState
{
    friend class CState;

private:
    float mHurtTimer = 1.f;

public:
    void Start() override;
    void Update(float DeltaTime) override;
    void End() override;
};

