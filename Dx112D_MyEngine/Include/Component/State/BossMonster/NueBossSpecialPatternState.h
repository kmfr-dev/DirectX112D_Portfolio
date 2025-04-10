#pragma once
#include "../State.h"

enum class EChargePhase
{
    Charging,
    Preparing,
    End
};

class CNueBossSpecialPatternState :
    public CState
{
    friend class CState;

private:
    // 현재 페이즈 상태.
    EChargePhase mPhase = EChargePhase::Charging;

    // 현재 돌진 카운트 
    int mChargeCount = 0;
    // 얼마나 돌진을 진행할지
    int mMaxChargeCount = 5;
    // 돌진할 Target
    FVector3D mChargeTarget = FVector3D::Zero;
    // 현재 돌진방향
    FVector3D mCurrentChargeDir;
    
    // 딜레이 시간
    float mDelayTimer = 0.f;

    bool mIsFirstCharge = true;
    bool mIsFinishedCharge = false;


public:
    void Start() override;
    void Update(float DeltaTime) override;
    void End() override;

public:
    // 돌진 타겟 Set함수
    void SetChargeTarget(const FVector3D& Pos) 
    { 
        mChargeTarget = Pos; 
    };

    void SetPatternDirDown();
    void SetPatternDir(const FVector3D& TargetPos);
    void SetPatternMove(float DeltaTime);
    void SetPatternRotation();

public:
    // 돌진 타겟 Get함수
    const FVector3D& GetChargeTarget() const 
    { 
        return mChargeTarget; 
    }

    // 현재 돌진 카운트 Get함수
    int GetChargeCount() const 
    { 
        return mChargeCount; 
    }
    
    FVector3D GetPatternTarget();
    FVector3D GetMapRandomOutPos() const;

public:
    bool IsChargeFinished()
    {
        return mChargeCount >= mMaxChargeCount;
    }
    bool IsMapOutBounds();

};

