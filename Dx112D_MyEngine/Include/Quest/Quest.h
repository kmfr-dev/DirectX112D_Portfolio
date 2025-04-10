#pragma once

#include "../GameInfo.h"

// 퀘스트 타입
// ex) 몬스터를 죽이는 퀘스트,
// NPC 한테 말거는 퀘스트
enum class EQuestType
{
	None,
	Kill,
	Talk,
	End
};

// 퀘스트 진행 상태,
// 수락, 진행중, 완료
enum class EQuestState
{
	None,
	NotAccepted,
	Progress,
	Complete,
	Completed,
	End
};

// 퀘스트 목표
struct FQuestGoal
{
	// 퀘스트 타입
	EQuestType QuestType = EQuestType::None;
	// 퀘스트 목표 대상이 누구인지.
	std::string TargetName;
	// 현재 퀘스트 수치
	int CurrentCount = 0;
	// 퀘스트 목표 수치
	int	TargetCount = 1;
};

struct FQuestRewardItem
{
	std::string ItemName;
	int			Count;
};

class CQuest
{
	friend class CQuestManager;

private:
	CQuest();
	CQuest(const CQuest& Quest);
	CQuest(CQuest&& Quest);
public:
	virtual ~CQuest();

private:
	// 퀘스트 ID
	int	mQuestID;
	// 다음 퀘스트 ID
	int mNextQuestID = -1;
	// 퀘스트 제목
	std::string mQuestTitle;
	// 퀘스트 설명
	std::string mDescription;
	// 퀘스트 목표
	FQuestGoal mQuestGoal;
	// 현재 퀘스트 상태
	EQuestState	mQuestState;
	// 퀘스트 보상 (ex 골드 , 아이템)
	FQuestRewardItem mRewardItem;
	// 퀘스트 보상 골드
	int		mGold = 0;

public:
	// 퀘스트 상태를 설정 해주는 함수
	void SetQuestState(EQuestState State) { mQuestState = State; }
	// 퀘스트 보상을 설정 해주는 함수
	void SetRewardItem(const FQuestRewardItem& Reward) { mRewardItem = Reward; }
	void SetRewardGold(int Gold) { mGold = Gold; }
	// 퀘스트 현재 수치 저장 셋함수
	void SetCurrentCount(int Count) { mQuestGoal.CurrentCount = Count; }
	// 퀘스트의 다음퀘스트 ID 설정 함수
	void SetNextQuestID(int QuestID) { mNextQuestID = QuestID; }
	
public:
	// 퀘스트 ID를 얻어오는 함수
	int GetQuestID() const { return mQuestID; }
	// 퀘스트의 다음퀘스트 ID를 얻어오는 함수
	int GetNextQuestID() const { return mNextQuestID; }
	// 퀘스트 제목을 얻어오는 함수
	const std::string& GetQuestTitle() const { return mQuestTitle; }
	// 퀘스트 설명을 얻어오는 함수
	const std::string& GetQuestDesc() const { return mDescription; }
	// 퀘스트 상태 정보를 얻어오는 함수
	EQuestState GetQuestState() const { return mQuestState; }
	// 퀘스트 목표 정보를 얻어오는 함수
	const FQuestGoal& GetQuestGoal() const { return mQuestGoal; }
	FQuestGoal& GetQuestGoal() { return mQuestGoal; }
	// 퀘스트의 보상 정보를 얻어오는 함수
	const FQuestRewardItem& GetRewardItem() const { return mRewardItem; }
	int GetRewardGold() const { return mGold; }
public:
	// 퀘스트 진행수치를 올려주는 함수
	void AddProgress();
	// 퀘스트를 완료했는지 확인하는 함수
	bool IsComplete() const
	{
		return mQuestState == EQuestState::Complete;
	}
	// 퀘스트가 완전히 완료됐는지 확인하는 함수
	bool IsCompleted() const 
	{ 
		return mQuestState == EQuestState::Completed;
	}

	bool CheckComplete()
	{
		// 만약 퀘스트가 완료 조건에 도달했으면 완료상태로 변경해준다.
		if (mQuestGoal.CurrentCount >= mQuestGoal.TargetCount)
		{
			mQuestState = EQuestState::Complete;
			return true;
		}

		return false;
	}
};

