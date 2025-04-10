#pragma once
#include "BattleObjectData.h"

enum class EMonsterAI : unsigned char
{
    Idle,
    Trace,
    Patrol,
    Attack,
    Death,
    Skill,
    Custom,
    End
};

struct FMonsterAIData
{
    // ========= Patrol ========
    FVector3D   mPatrolTarget = FVector3D::Zero;
    bool        mHasPatrolTarget = false;
    float       mPatrolTimer = 0.f;
};

class CMonsterData :
    public CBattleObjectData
{
public:
	CMonsterData();
	CMonsterData(const CMonsterData& Data);
	CMonsterData(CMonsterData&& Data);
	virtual ~CMonsterData();

public:
    EMonsterAI     mMonsterAI = EMonsterAI::Idle;
    EObjectDir     mMonsterDir = EObjectDir::B;
    EMonsterType   mMonsterType = EMonsterType::None;
    FMonsterAIData mMonsterAIData;

    FVector3D       mPatrolTarget = FVector3D::Zero;
    float           mPatrolTimer = 0.f;
    bool            mHasPatrolTarget = false;

    bool             mCanPatrol = true;
    bool            mMoving = false;

public:
	virtual bool LoadFromFile(const char* FileName) override;
	virtual bool SaveToFile(const char* FileName) override;
};

