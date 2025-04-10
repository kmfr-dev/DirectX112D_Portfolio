#pragma once
#include "Quest.h"

class CQuestManager
{
private:
	std::unordered_map<int, CQuest*> mQuestMap;

public:
	bool	Init();
	void	Update(float DeltaTime);

public:
	// 퀘스트 생성함수
	void	CreateQuest(int QuestID, const std::string& QuestTitle,
		const std::string& QuestDesc, EQuestType QuestType,
		const std::string& TargetName, int TargetCount, int Gold = 0,
		const std::string& ItemName = "", int ItemCount = 0);
	// 퀘스트 추가 함수
	void	AddQuest(CQuest* NewQuest);
	// 퀘스트 제거 함수
	void	RemoveQuest(int QuestID);
	// 퀘스트 찾는 함수
	CQuest* FindQuest(int QuestID);

	// NPC가 가지고있는 퀘스트가 진행중인지
	// 수락전 인지 완료된 상태인지 판단하는 함수
	EQuestState GetNPCQuestMarkState(const std::list<int>& QuestIDs,
		class CPlayerObject* Player);

public:
	// 퀘스트 Notify
	void	QuestNotify(const std::string& TargetName);


	DECLARE_SINGLE(CQuestManager)
};

