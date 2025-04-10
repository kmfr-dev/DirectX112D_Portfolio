#pragma once

#include "GameInfo.h"

struct FScriptLine
{
	// 현재 대사 ID
	int CurrentLineID;
	// 현재 NPC의 이름
	std::string NPCName;
	// 대사
	std::string Text;
	// 다음 대사 ID
	int NextLineID = -1;
};

class CScriptManager
{
private:
	// 1번째 맵 Key : NPCID
	// 2번째 맵 Key : ScriptID
	std::unordered_map<int, std::unordered_map<int, FScriptLine*>> mScriptMap;

public:
	bool Init();

public:
	// 맵에서 NPCID와 스크립트 ID로 대사를 찾는다.
	const FScriptLine* FindScriptLine(int NPCID, int ScriptLineID) const;
	// 대사 추가
	void	AddScript(int NPCID, int ScriptID, std::string NPCName, std::string Script);
	// 대사의 다음 대사 추가 함수
	void	SetNextLine(int NPCID, int FromScriptID, int ToScriptID);

	DECLARE_SINGLE(CScriptManager)
};

