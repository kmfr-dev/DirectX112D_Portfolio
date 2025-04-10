#pragma once
#include "MovementComponent.h"

class CMonsterMovement :
    public CMovementComponent
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CMonsterMovement();
	CMonsterMovement(const CMonsterMovement& Com);
	CMonsterMovement(CMonsterMovement&& Com);
	virtual ~CMonsterMovement();

public:
	virtual bool Init() override;
	virtual bool Init(const char* FileName) override;
	virtual void PreUpdate(float DeltaTime) override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
	virtual CMonsterMovement* Clone() override;
	virtual void EndFrame() override;

	virtual bool CheckBlockedByTile() override;
};

