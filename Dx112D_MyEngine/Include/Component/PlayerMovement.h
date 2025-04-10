#pragma once
#include "MovementComponent.h"

class CPlayerMovement :
    public CMovementComponent
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CPlayerMovement();
	CPlayerMovement(const CPlayerMovement& Com);
	CPlayerMovement(CPlayerMovement&& Com);
	virtual ~CPlayerMovement();

public:
	virtual bool Init() override;
	virtual bool Init(const char* FileName) override;
	virtual void PreUpdate(float DeltaTime) override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
	virtual CPlayerMovement* Clone() override;
	virtual void EndFrame() override;

	virtual bool CheckBlockedByTile() override;
};

