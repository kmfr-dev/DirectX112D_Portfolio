#pragma once

#include "../State.h"

class CPlayerIdleState :
    public CState
{
    friend class CState;

public:
    void Start() override;
    void Update(float DeltaTime) override;
    void End() override;
};

