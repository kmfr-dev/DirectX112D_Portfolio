#pragma once

#include "Component.h"
#include "NavAgent.h"


// 컴포넌트와 네비게이션 컴포넌트 다중 상속
class CMovementComponent :
	public CComponent,
	public CNavAgent
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CMovementComponent();
	CMovementComponent(const CMovementComponent& Com);
	CMovementComponent(CMovementComponent&& Com);
	virtual ~CMovementComponent();

protected:
	CSharedPtr<class CTileMapObj>		mTileMapObj;
	CSharedPtr<class CSceneComponent>	mUpdateComponent;
	EAxis::Type		mMoveAxis = EAxis::None;

protected:
	FVector3D		mVelocity;
	FVector3D		mMoveStep;
	float			mSpeed = 1.f;
	bool			mVelocityInit = true;

	// 타겟과의 거리
	float			mTargetDist = 0.f;

public:
	const FVector3D& GetMoveStep()	const
	{
		return mMoveStep;
	}

	const FVector3D& GetVelocity()	const
	{
		return mVelocity;
	}
	
	float GetVelocityLength()	const
	{
		return mVelocity.Length();
	}

	float GetMoveDistance()	const
	{
		return mMoveStep.Length();
	}

public:
	void SetUpdateComponent(class CSceneComponent* Target);
	void SetMoveSpeed(float Speed)
	{
		mSpeed = Speed;
	}

	void AddMove(const FVector3D& Dir)
	{
		mVelocity += Dir;
	}

	void SetMove(const FVector3D& Dir)
	{
		mVelocity = Dir;
	}

	void SetVelocityInit(bool VelocityInit)
	{
		mVelocityInit = VelocityInit;
	}

	void SetMoveAxis(EAxis::Type Axis)
	{
		mMoveAxis = Axis;
	}

	void SetMoveStep(const FVector3D Step)
	{
		mMoveStep = Step;
	}
	void SetMovePoint(const FVector2D& Pos);
	

public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CMovementComponent* Clone();
	virtual void EndFrame();


	virtual bool CheckBlockedByTile();
};


