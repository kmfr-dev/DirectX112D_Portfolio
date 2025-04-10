#pragma once

#include "../GameInfo.h"



class CNavAgent
{
	friend class CMovementComponent;

protected:
	CNavAgent();
	CNavAgent(const CNavAgent& Com);
	CNavAgent(CNavAgent&& Com);
	~CNavAgent();

public:
	// 최종경로 목록
	std::list<FVector2D>  mPathList;

public:
	// 경로를 찾는 함수
	bool FindPath(const FVector2D& Start,
		const FVector2D& End);
	bool FindPath(const FVector3D& Start,
		const FVector2D& End);
	bool FindPath(const FVector3D& Start,
		const FVector3D& End);
};

