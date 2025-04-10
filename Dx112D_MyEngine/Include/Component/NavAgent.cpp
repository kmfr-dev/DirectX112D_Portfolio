#include "NavAgent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Navigation.h"


CNavAgent::CNavAgent()
{
}

CNavAgent::CNavAgent(const CNavAgent& Com)
{
}

CNavAgent::CNavAgent(CNavAgent&& Com)
{
}

CNavAgent::~CNavAgent()
{
}

bool CNavAgent::FindPath(const FVector2D& Start,
	const FVector2D& End)
{
	CScene* Scene = CSceneManager::GetInst()->GetCurrentScene();

	return Scene->GetNavigation()->FindPath(Start, End, mPathList);
}

bool CNavAgent::FindPath(const FVector3D& Start, const FVector2D& End)
{
	FVector2D _Start;
	_Start.x = Start.x;
	_Start.y = Start.y;

	return FindPath(_Start, End);
}

bool CNavAgent::FindPath(const FVector3D& Start,
	const FVector3D& End)
{
	FVector2D _Start;
	_Start.x = Start.x;
	_Start.y = Start.y;

	FVector2D _End;
	_End.x = End.x;
	_End.y = End.y;

	return FindPath(_Start, _End);
}

