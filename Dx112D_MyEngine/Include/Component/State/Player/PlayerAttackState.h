#pragma once


#include "../State.h"
class CPlayerAttackState :
    public CState
{
    friend class CState;

private:
    bool mCanAttack = true;
    float mAttackTimer = 1.2f;
public:
    void Start() override;
    void Update(float DeltaTime) override;
    void End() override;
};

