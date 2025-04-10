#pragma once
#include "ObjectData.h"

class CBattleObjectData :
    public CObjectData
{
public:
	CBattleObjectData();
	CBattleObjectData(const CBattleObjectData& Data);
	CBattleObjectData(CBattleObjectData&& Data);
	virtual ~CBattleObjectData();

public:
	EObjectDir		Dir;

	int				HP;
	int				HPMax;
	int				Attack = 1;
	float			Speed;



	bool			Dead = false;

	bool			CanAttack = true;
	bool            Attacking = false;

	bool            Moving = false;
	bool            CanMove = true;

	bool            Hitting = false;
	bool            Invincible = false;

	float			AttackTimer;
	float			AttackCoolDown;

public:
	virtual bool LoadFromFile(const char* FileName) override;
	virtual bool SaveToFile(const char* FileName) override;
};

