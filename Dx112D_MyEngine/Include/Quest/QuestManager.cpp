#include "QuestManager.h"
#include "../Asset/Texture/Texture.h"
#include "../Object/PlayerObject.h"

DEFINITION_SINGLE(CQuestManager);

CQuestManager::CQuestManager()
{

}

CQuestManager::~CQuestManager()
{
	auto iter = mQuestMap.begin();
	auto iterEnd = mQuestMap.end();

	for (; iter != iterEnd; ++iter)
		SAFE_DELETE(iter->second);

	mQuestMap.clear();
}

bool CQuestManager::Init()
{
	CreateQuest(1, "오구 마을을 위기에서 구해줘! (1)",
		"오구 마을 주변에 몬스터들이 생겨나기 시작했다. 근접 몬스터를 소탕해서 주민들을 도와주자!",
		EQuestType::Kill, "NearingMonster", 5, 1500, "레몬", 3);

	FindQuest(1)->SetNextQuestID(2);
	CreateQuest(2, "오구 마을을 위기에서 구해줘! (2)",
		"오구 마을 주변에 몬스터들이 생겨나기 시작했다. 원거리 몬스터를 소탕해서 주민들을 도와주자!",
		EQuestType::Kill,"MageMonster", 5, 5000, "신비한 과일", 5);
	FindQuest(2)->SetNextQuestID(3);
	CreateQuest(3, "오구 마을을 위기에서 구해줘! (3)",
		"자 이제 마지막이야! 우리를 괴롭히는 흉악한 녀석이 있어, 원래는 착한아이였는데.. 네가 누에용사를 구해줘!",
		EQuestType::Kill, "NueBossMonster", 1, 5000, "?", 1);

	return true;
}

void CQuestManager::Update(float DeltaTime)
{
	
}

void CQuestManager::CreateQuest(int QuestID, const std::string& QuestTitle,
	const std::string& QuestDesc, EQuestType QuestType, 
	const std::string& TargetName, int TargetCount, int Gold, 
	const std::string& ItemName, int ItemCount)
{
	// 만약 동일한 ID의 퀘스트가 맵에 존재한다면
	// 퀘스트를 만들면 안됨
	if (mQuestMap.find(QuestID) != mQuestMap.end())
		return;

	// 퀘스트 목표 설정
	FQuestGoal Goal;
	Goal.QuestType = QuestType;
	Goal.TargetName = TargetName;
	Goal.TargetCount = TargetCount;
	Goal.CurrentCount = 0;

	// 퀘스트 생성
	CQuest* NewQuest = new CQuest;
	NewQuest->mQuestID = QuestID;
	NewQuest->mQuestGoal = Goal;
	NewQuest->mQuestState = EQuestState::NotAccepted;
	NewQuest->mQuestTitle = QuestTitle;
	NewQuest->mDescription = QuestDesc;
	NewQuest->mGold = Gold;
	NewQuest->mRewardItem.ItemName =ItemName;
	NewQuest->mRewardItem.Count = ItemCount;

	// 퀘스트 맵에 추가
	mQuestMap[QuestID] = NewQuest;
}

void CQuestManager::AddQuest(CQuest* NewQuest)
{
	// 만약 추가할 퀘스트가 없다면 return
	if (!NewQuest)
		return;

	// 추가할 퀘스트의 ID를 얻어온다.
	int QuestID = NewQuest->GetQuestID();

	// 추가할 퀘스트가 맵에 존재하지 않을 때만
	// 퀘스트를 맵에 추가 해준다.
	if (mQuestMap.find(QuestID) == mQuestMap.end())
		mQuestMap[QuestID] = NewQuest;
}

void CQuestManager::RemoveQuest(int QuestID)
{
	// QuestId로 맵에서 찾는다.
	auto iter = mQuestMap.find(QuestID);
	
	// 해당 ID의 퀘스트를 찾았으면
	// 퀘스트를 지우고 맵에서 지움
	if (iter != mQuestMap.end())
	{
		SAFE_DELETE(iter->second);
		mQuestMap.erase(iter);
	}
}

CQuest* CQuestManager::FindQuest(int QuestID)
{
	// 퀘스트를 맵에서 찾는다.
	auto iter = mQuestMap.find(QuestID);

	// 퀘스트를 찾았으면 return
	if (iter != mQuestMap.end())
		return iter->second;

	// 못찾았으면 nullptr 반환
	return nullptr;
}


EQuestState CQuestManager::GetNPCQuestMarkState(const std::list<int>& QuestIDs, 
	CPlayerObject* Player)
{
	// 진행중, 수락전 bool 변수
	bool HasProgress = false;
	bool HasNotAccepted = false;

	// NPC 퀘스트ID 목록을 순회
	for (int QuestID : QuestIDs)
	{
		// 플레이어가 가진 퀘스트 확인
		CQuest* PlayerQuest = Player->FindQuestByID(QuestID);

		// NPC와 같은 퀘스트를 가지고 있지 않으면
		if (!PlayerQuest)
		{
			// 수락 전 상태
			HasNotAccepted = true;
			continue;
		}

		// 완료 조건 만족한 퀘스트가 있다면
		if (PlayerQuest->IsComplete())
			return EQuestState::Complete;

		// 퀘스트 상태가 진행중이면
		if (PlayerQuest->GetQuestState() == EQuestState::Progress)
			HasProgress = true;
	}

	
	if (HasNotAccepted)
		return EQuestState::NotAccepted;

	if (HasProgress)
		return EQuestState::Progress;

	// 모든 퀘스트가 완료되어 표시할 필요 없음
	return EQuestState::End;
}

void CQuestManager::QuestNotify(const std::string& TargetName)
{
	auto iter = mQuestMap.begin();
	auto iterEnd = mQuestMap.end();

	for (; iter != iterEnd; ++iter)
	{
		CQuest* Quest = iter->second;

		if (!Quest)
			continue;

		// 만약 퀘스트가 진행중이 아니라면 continue
  		if (Quest->GetQuestState() != EQuestState::Progress)
			continue;

		// 퀘스트의 목표를 가져온다.
		FQuestGoal Goal = Quest->GetQuestGoal();

		if (Goal.QuestType != EQuestType::None &&
			Goal.TargetName == TargetName &&
			!Quest->IsCompleted())
		{
			Quest->AddProgress();
		}
	}
}
