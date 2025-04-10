#include "MovementComponent.h"
#include "SceneComponent.h"
#include "NavAgent.h"
#include "TileMapComponent.h"
#include "ColliderBase.h"
#include "ColliderAABB2D.h"

#include "../Object/MonsterObject.h"
#include "../Object/MageMonster.h"
#include "../Object/NearingMonster.h"
#include "../Object/PlayerObject.h"
#include "../Object/TileMapObj.h"
#include "../Scene/Scene.h"
#include "../Share/Log.h"

CMovementComponent::CMovementComponent()
{
}

CMovementComponent::CMovementComponent(
    const CMovementComponent& Com)  :
    CComponent(Com)
{
}

CMovementComponent::CMovementComponent(CMovementComponent&& Com) :
    CComponent(Com)
{
}

CMovementComponent::~CMovementComponent()
{

}

void CMovementComponent::SetUpdateComponent(CSceneComponent* Target)
{
    mUpdateComponent = Target;
}

void CMovementComponent::SetMovePoint(const FVector2D& Pos)
{
    if (mUpdateComponent)
    {
        FVector2D WorldPos;
        WorldPos.x = mUpdateComponent->GetWorldPosition().x;
        WorldPos.y = mUpdateComponent->GetWorldPosition().y;

        FindPath(WorldPos, Pos);

        if (!mPathList.empty())
        {
            // 타겟과의 거리를 구해준다.
            mTargetDist = WorldPos.Distance(mPathList.front());
        }

    };
}

bool CMovementComponent::Init()
{
    if (!CComponent::Init())
        return false;

    mTileMapObj = mScene->FindObjectFromType<CTileMapObj>();

    return true;
}

bool CMovementComponent::Init(const char* FileName)
{
    if (!CComponent::Init(FileName))
        return false;

    mTileMapObj = mScene->FindObjectFromType<CTileMapObj>();

    return true;
}

void CMovementComponent::PreUpdate(float DeltaTime)
{
    CComponent::PreUpdate(DeltaTime);

    if (mUpdateComponent)
    {
        if (!mUpdateComponent->IsActive())
            mUpdateComponent = nullptr;
    }
}

void CMovementComponent::Update(float DeltaTime)
{
    CComponent::Update(DeltaTime);

    if (mUpdateComponent)
    {
        if (!mUpdateComponent->IsActive())
            mUpdateComponent = nullptr;

        else
        {
            // 경로가 없다면 속도벡터를 가지고 이동하게끔 처리
            if(mPathList.empty())
            {

                if (mMoveAxis != EAxis::None)
                {
                    mVelocity = mUpdateComponent->GetAxis(mMoveAxis);
          
                }

                mVelocity.Normalize();

                if (mVelocity.Length() > 0.f)
                {
                    // 방향 벡터를 속도와 곱해서 전체 이동거리를 계산.
                    FVector3D Step = mVelocity * mSpeed * DeltaTime;

                    // X축과 Y축을 분리한다.
                    // 각각 따로 체크해서 가능한 방향만 이동하기 위함
                    FVector3D MoveStepX(Step.x, 0.f, 0.f);
                    FVector3D MoveStepY(0.f, Step.y, 0.f);

                    // X축과 Y축 bool 변수
                    bool CanMoveX = false; 
                    bool CanMoveY = false;

                    // X축 이동 불가시 
                    // X축은 막힘
                    mMoveStep = MoveStepX;
                    if (!CheckBlockedByTile())
                        CanMoveX = true;

                    // Y축 이동 불가시
                    // Y축은 막힘
                    mMoveStep = MoveStepY;
                    if (!CheckBlockedByTile())
                        CanMoveY = true;

                    // 만약 X축 이동이 가능하면
                    // X축으로 이동한다.
                    if (CanMoveX)
                        mUpdateComponent->AddWorldPos(MoveStepX);

                    // 만약 Y축 이동이 가능하면
                    // Y축으로 이동한다.
                    if (CanMoveY)
                        mUpdateComponent->AddWorldPos(MoveStepY);

                    // 이동거리 초기화
                    mMoveStep = FVector3D::Zero;
                }
            }

            // 경로 목록이 있다면
            else
            {
                // 이동 타겟, 경로 목록의 맨앞에 것을 가져온다.
                FVector3D Target;
                Target.x = mPathList.front().x;
                Target.y = mPathList.front().y;
    
                // 경로를 이용한 방향 벡터를 만든다.
                FVector3D Dir = Target -
                    mUpdateComponent->GetWorldPosition();
                Dir.Normalize();

                mMoveStep = Dir * mSpeed * DeltaTime;

                // MoveStep의 크기를 구해준다.
                float StepLength = mMoveStep.Length();

                if (StepLength >= mTargetDist)
                {
                    FVector2D _Target = mPathList.front();

                    mPathList.pop_front();

                    if (!mPathList.empty())
                    {
                        mTargetDist = _Target.Distance(mPathList.front());
                    }

                    mUpdateComponent->SetWorldPos(Target);
                }

                else
                {
                    mTargetDist -= StepLength;
                    mUpdateComponent->AddWorldPos(mMoveStep);
                }

            }
        }
    }
}

void CMovementComponent::PostUpdate(float DeltaTime)
{
    CComponent::PostUpdate(DeltaTime);

    if (mUpdateComponent)
    {
        if (!mUpdateComponent->IsActive())
            mUpdateComponent = nullptr;
    }
}

CMovementComponent* CMovementComponent::Clone()
{
    return new CMovementComponent(*this);
}

void CMovementComponent::EndFrame()
{
    CComponent::EndFrame();

    if (mVelocityInit)
        mVelocity = FVector3D::Zero;

    mMoveStep = FVector3D::Zero;
}

bool CMovementComponent::CheckBlockedByTile()
{
    return false;
}