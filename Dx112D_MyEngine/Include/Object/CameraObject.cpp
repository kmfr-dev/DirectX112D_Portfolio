#include "CameraObject.h"
#include "TileMapObj.h"
#include "../Component/CameraComponent.h"
#include "../Device.h"
#include "../Share/Timer.h"
CCameraObject::CCameraObject()
{
}

CCameraObject::CCameraObject(const CCameraObject& Obj)
{
}

CCameraObject::CCameraObject(CCameraObject&& Obj)
{
}

CCameraObject::~CCameraObject()
{
}

bool CCameraObject::Init()
{
	CSceneObject::Init();

	mCamera = CreateComponent<CCameraComponent>();

	mCamera->SetProjectionType(ECameraProjectionType::Ortho);

	SetRootComponent(mCamera);

	return true;
}

void CCameraObject::PreUpdate(float DeltaTime)
{
	CSceneObject::PreUpdate(DeltaTime);

	FollowTarget();
}

void CCameraObject::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);
}

void CCameraObject::Render()
{
	CSceneObject::Render();
}

void CCameraObject::SetTileMap(CSharedPtr<class CTileMapObj> Map)
{
	mTileMap = Map;
}

void CCameraObject::FollowTarget()
{
    if (!mTarget || !mTileMap)
        return;

    FVector3D TargetPos = mTarget->GetWorldPosition();

    FVector2D MinBounds = mTileMap->GetMinBounds();
    FVector2D MaxBounds = mTileMap->GetMaxBounds();

    FVector2D TargetCameraPos;
    TargetCameraPos.x = TargetPos.x;
    TargetCameraPos.y = TargetPos.y;

    // 맵 왼쪽 끝에 도달
    if (TargetCameraPos.x < MinBounds.x)
        TargetCameraPos.x = MinBounds.x;

    // 맵 오른쪽 끝에 도달
    else if (TargetCameraPos.x > MaxBounds.x)
        TargetCameraPos.x = MaxBounds.x;

    // 맵 위쪽 끝
    if (TargetCameraPos.y < MinBounds.y)
        TargetCameraPos.y = MinBounds.y;

    // 맵 아래쪽 끝
    else if (TargetCameraPos.y > MaxBounds.y)
        TargetCameraPos.y = MaxBounds.y;

    FVector2D NewCamPos;

    if (mFirst)
    {
        // 첫 프레임은 강제로 위치 설정
        NewCamPos = FVector2D(TargetCameraPos.x, TargetCameraPos.y);
        mFirst = false;
    }

    else
    {
        // 현재 카메라 위치
        FVector2D CurrentCamPos = FVector2D(mCamera->GetWorldPosition().x,
            mCamera->GetWorldPosition().y);

        float LerpSpeed = 5.0f;
        FVector2D SmoothPos = FVector2D::Lerp(CurrentCamPos,
            TargetCameraPos, LerpSpeed * CTimer::GetDeltaTime());

        NewCamPos = FVector2D(SmoothPos.x, SmoothPos.y);
    }

    mCamera->SetWorldPos(NewCamPos);
}
