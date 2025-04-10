#pragma once

#include "../../GameInfo.h"

class CObjectData
{
public:
	CObjectData();
	CObjectData(const CObjectData& Data);
	CObjectData(CObjectData&& Data);
	virtual ~CObjectData();

public:
	std::string		Name;
	FVector2D		Pos;
	FVector3D		Scale;
	FVector2D		Pivot;
	EObjectType		Type = EObjectType::None;

public:
	virtual bool LoadFromFile(const char* FileName);
	virtual bool SaveToFile(const char* FileName);
};

