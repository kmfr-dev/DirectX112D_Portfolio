#include "NueBossSpecialPatternState.h"
#include "../../StateMachineComponent.h"
#include "../../ColliderBase.h"
#include "../../../Share/Log.h"
#include "../../../Object/NueBossMonster.h"
#include "../../../Object/TileMapObj.h"

#include "../../../Scene/Scene.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Scene/SceneAssetManager.h"
#include "../../../Asset/Sound/SoundManager.h"

void CNueBossSpecialPatternState::Start()
{
    CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);
    if (!Boss) 
        return;

    // 현재 페이즈가 돌진상태면
    if (mPhase == EChargePhase::Charging)
    {
        // 현재 첫번째 돌진이면
        // 무조건 아래로 돌진 -> 연출용
        if (mIsFirstCharge)
        {
            // 방향을 아래로 설정하고
            // 첫번째 돌진은 충돌체를 꺼줌
            SetPatternDirDown();
            mIsFirstCharge = false;
            Boss->GetCollider()->SetEnable(false);
        }
        
        // 첫번재 돌진이 아니면
        else 
        {
            // 돌진 타겟을 정하고, 방향을 정해준다.
            // 그리고 충돌체를 켜준다.
            mChargeTarget = GetPatternTarget();
            SetPatternDir(mChargeTarget);
            Boss->GetCollider()->SetEnable(true);
        }

        // 방향에 맞춰 보스를 회전시켜준다.
        SetPatternRotation();
    }

    else if (mPhase == EChargePhase::Preparing)
    {
        mDelayTimer = 0.5f;
    }

    else if (mPhase == EChargePhase::End)
    {
        // 돌진이 전부 끝났으면 마지막으로
        // 보스의 원래 스폰 위치로 돌진하기위해 
        // 타겟과 방향을 설정해준다.
        mChargeTarget = GetPatternTarget();
        SetPatternDir(mChargeTarget);
        SetPatternRotation();
    }

    Boss->ChangeAnimation("NueBossMonster_SpecialPattern");
    CSceneManager::GetInst()->GetCurrentScene()->GetAssetManager()
        ->FindSound("NueBossPattern")->Play();
}

void CNueBossSpecialPatternState::Update(float DeltaTime)
{
    // 만약 돌진상태면
    if (mPhase == EChargePhase::Charging)
    {
        // 실제 돌진을 실행한다.
        SetPatternMove(DeltaTime);

        // 만약 돌진상태인데 맵밖에 나갔을 경우
        if (IsMapOutBounds())
        {
            // 돌진 횟수를 늘려준다.
            ++mChargeCount;

            // 만약 현재 돌진횟수가 설정해준 횟수가 됐을 경우
            if (mChargeCount >= mMaxChargeCount)
            {
                // 돌진을 종료한다.
                mPhase = EChargePhase::End;
                mIsFinishedCharge = true;
            }
            // 그게아니라면 돌진 준비상태 돌입
            else
                mPhase = EChargePhase:: Preparing;

            Start(); 
            CSceneManager::GetInst()->GetCurrentScene()->GetAssetManager()
                ->FindSound("NueBossPattern")->Stop();
            return;
        }
    }

    else if (mPhase == EChargePhase:: Preparing)
    {
        mDelayTimer -= DeltaTime;
        if (mDelayTimer <= 0.f)
        {
            mPhase = EChargePhase::Charging;
            Start();
            return;
        }
    }

    // 만약 돌진이 횟수를 종료했을 경우
    else if (mPhase == EChargePhase::End)
    {
        CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);
        if (!Boss)
            return;

        // Start에서 보스 스폰위치로 타겟을 잡아줬다.
        // 그러므로 스폰위치로 돌진 수행
        SetPatternMove(DeltaTime);
        
        // 보스 현재위치와 타겟위치를 가져온다.
        FVector3D Pos = Boss->GetWorldPosition();
        FVector3D Target = GetPatternTarget();

        // 오차 허용 범위
        float Tolerance = 5.f; 

        // float 좌표는 == 으로 비교하면
        // 잘 안 맞는 경우가 많기 때문에 오차를 허용한 비교

        // 보스가 도착지점에 도착했을 경우 Idle 상태로 변경
        if (fabs(Pos.x - Target.x) < Tolerance && 
            fabs(Pos.y - Target.y) < Tolerance)
        {
            Boss->FindNonSceneComponent<CStateMachineComponent>()
                ->ChangeStateBossMonster(EBossMonsterAIState::Idle);
        }

    }
}

void CNueBossSpecialPatternState::End()
{
    CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);
    if (!Boss) return;

    Boss->SetSpPattern(false);

    mPhase = EChargePhase::Charging;
    mChargeCount = 0;
    mIsFirstCharge = true;
    mIsFinishedCharge = false;

    Boss->SetWorldRotationZ(0);
    Boss->GetCollider()->SetEnable(true);
    Boss->SetWorldPos(2050.f, 2500.f);
   
}

void CNueBossSpecialPatternState::SetPatternDirDown()
{
    // 돌진 방향을 아래로 설정.
    mCurrentChargeDir = FVector3D(0, -1, 0);
}

void CNueBossSpecialPatternState::SetPatternDir(const FVector3D& TargetPos)
{
    CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);
    if (!Boss) 
        return;

    // 돌진 방향을 타겟방향으로 설정.
    mCurrentChargeDir = (TargetPos - Boss->GetWorldPosition());
    mCurrentChargeDir.Normalize();
}

void CNueBossSpecialPatternState::SetPatternMove(float DeltaTime)
{
    CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);
    if (!Boss)
        return;

    // 현재 돌진방향으로 보스를 이동시킨다.
    Boss->AddWorldPos(mCurrentChargeDir * Boss->GetSpeed() * DeltaTime);
}

void CNueBossSpecialPatternState::SetPatternRotation()
{
    float Angle = atan2f(mCurrentChargeDir.y, mCurrentChargeDir.x)
        * (180.f / 3.141592f);

    Angle += 90.f;

    CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);
    if (!Boss)
        return;

    Boss->SetWorldRotationZ(Angle);

}

FVector3D CNueBossSpecialPatternState::GetPatternTarget()
{
    CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);

    if (!Boss)
        return FVector3D::Zero;

    // 만약 보스의 타겟이 있을 경우
    // 타겟의 WorldPos를 가져옴
    if(mPhase == EChargePhase::Charging || 
        mPhase ==  EChargePhase::Preparing)
    {
        if (Boss->GetTarget())
            return Boss->GetTarget()->GetWorldPosition();
    }
    else if (mPhase == EChargePhase::End)
    {
        FVector3D LastTarget = FVector3D(2050, 2500.f, 1.f);
        return LastTarget;
    }

    return FVector3D::Zero;
}

FVector3D CNueBossSpecialPatternState::GetMapRandomOutPos() const
{
    float Margin = 100.f;
    FVector2D MapSize = CSceneManager::GetInst()->
        GetCurrentScene()->GetMapObj()->GetTileMap()->GetTileMapSize();

    // 맵 상하좌우
    switch (rand() % 4)
    {
    case 0:
        return FVector3D(-Margin, (float)(rand() % (int)MapSize.y), 1.f);
    case 1:
        return FVector3D(MapSize.x + Margin, (float)(rand() % (int)MapSize.y), 1.f);
    case 2:
        return FVector3D((float)(rand() % (int)MapSize.x), -Margin, 1.f);
    case 3:
        return FVector3D((float)(rand() % (int)MapSize.x), MapSize.y + Margin, 1.f);
    }

    return FVector3D::Zero;
}

bool CNueBossSpecialPatternState::IsMapOutBounds()
{
    CNueBossMonster* Boss = dynamic_cast<CNueBossMonster*>(mOwner);
    if (!Boss)
        return false;

    FVector3D Pos = Boss->GetWorldPosition();
    FVector2D MapSize = CSceneManager::GetInst()->
        GetCurrentScene()->GetMapObj()->GetTileMap()->GetTileMapSize();

    float Margin = 100.f;

    // 만약 보스 위치가 맵밖을 나갔을 경우 맵밖을 나갔다고 판단.
    if (Pos.x < -Margin ||
        Pos.y < -Margin ||
        Pos.x > MapSize.x + Margin ||
        Pos.y > MapSize.y + Margin)
        return true;

    return false;
}
