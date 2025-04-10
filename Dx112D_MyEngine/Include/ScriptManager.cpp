#include "ScriptManager.h"

DEFINITION_SINGLE(CScriptManager);

CScriptManager::CScriptManager()
{

}

CScriptManager::~CScriptManager()
{
	auto iter = mScriptMap.begin();
	auto iterEnd = mScriptMap.end();
	for (; iter != iterEnd; ++iter)
	{
		auto& InnerMap = iter->second;

		for (auto LineIter = InnerMap.begin();
			LineIter != InnerMap.end(); ++LineIter)
		{
			SAFE_DELETE(LineIter->second);
		}

		InnerMap.clear(); 
	}

	mScriptMap.clear();
}

bool CScriptManager::Init()
{
	// 오구NPC 기본 대사
	AddScript(1, 1, "BigOguNPC", "안녕! 나는 엄청나게 큰 오구야.");
	AddScript(1, 2, "BigOguNPC", "우리 마을 사람들은 엄청 귀엽고 착해!");
	AddScript(1, 3, "BigOguNPC", "우리만큼은 아니지만 너도 귀여워!");
	AddScript(1, 4, "BigOguNPC", "다른 귀여운 주민들도 더 많아졌으면 좋겠어~ ");
	SetNextLine(1, 1, 2);
	SetNextLine(1, 2, 3);
	SetNextLine(1, 3, 4);

	// 근접몬스터 퀘스트가 있으면
	AddScript(1, 50, "BigOguNPC", "지금 우리 오구 마을에 엄청난 골칫거리가 생겼어.");
	AddScript(1, 51, "BigOguNPC", "우리 마을을 위해 근접 몬스터 5마리를 소탕해줘!");
	AddScript(1, 52, "BigOguNPC", "근접 몬스터 5마리 퇴치 부탁할 수 있을까?");
	SetNextLine(1, 50, 51);
	SetNextLine(1, 51, 52);

	// 원거리 몬스터 퀘스트가 있으면
	AddScript(1, 100, "BigOguNPC", "오구야!! 큰일났어, 이번에는 원거리 몬스터가 우리 주민들을 괴롭혀..");
	AddScript(1, 101, "BigOguNPC", "이번에도 우리 주민들을 도와줄꺼지??");
	AddScript(1, 102, "BigOguNPC", "부탁해! 이번엔 원거리 몬스터 5마리를 소탕해줘!");
	SetNextLine(1, 100, 101);
	SetNextLine(1, 101, 102);

	// 퀘스트창을 껐을 때
	AddScript(1, 150, "BigOguNPC", "정말 안 도와줄꺼야?");
	AddScript(1, 151, "BigOguNPC", "생각 바뀌면 다시 말 걸어줘!");
	SetNextLine(1, 150, 151);

	// 근접 & 원거리 몬스터 퀘스트 진행 중
	AddScript(1, 200, "BigOguNPC", "아직 몬스터를 다 잡지 못했구나?");
	AddScript(1, 201, "BigOguNPC", "마저 몬스터 소탕을 부탁해!");
	SetNextLine(1, 200, 201);

	// 보스 몬스터 퀘스트가 있으면
	AddScript(1, 300, "BigOguNPC", "이번엔 마지막 부탁이야.");
	AddScript(1, 301, "BigOguNPC", "지금 몬스터 무리들 뒷편에 내 친구 누에용사가 있어.");
	AddScript(1, 302, "BigOguNPC", "원래는 착한 아이였는데.. 갑자기 기억을 잃고 타락해버렸어.");
	AddScript(1, 303, "BigOguNPC", "너라면 할 수 있을 것 같아. 제발 내 친구 누에 용사를 구해줘!");
	SetNextLine(1, 300, 301);
	SetNextLine(1, 301, 302);
	SetNextLine(1, 302, 303);
	
	// 보스 몬스터 퀘스트가 진행중이면
	AddScript(1, 350, "BigOguNPC", "내 친구 누에용사를 구해줘!!");

	// 완료됨
	AddScript(1, 400, "BigOguNPC", "잘했어! 네 덕분에 마을이 평화로워졌어.");
	AddScript(1, 401, "BigOguNPC", "이건 네가 도와줘서 고마운 마음에 주는 보답이야.");
	SetNextLine(1, 400, 401);

	// 상점NPC
	AddScript(2, 1, "ShopNPC", "내 가게에는 모든 물건을 팔고있어!");
	AddScript(2, 2, "ShopNPC", "한 번 둘러봐~");
	SetNextLine(2, 1, 2);

	// 누에용사 NPC
	AddScript(4, 1, "누에용사", "뭐야! 네가 날 도와준거야?");
	AddScript(4, 2, "누에용사", "고마워! 덕분에 잠에서 깨어났어.");
	AddScript(4, 3, "누에용사", "이 은혜 잊지 않을게.");
	SetNextLine(4, 1, 2);
	SetNextLine(4, 2, 3);

	return true;
}

const FScriptLine* CScriptManager::FindScriptLine(int NPCID, int ScriptLineID) const
{

	// 만약 NPCID가 없다면 return
	auto Iter = mScriptMap.find(NPCID);
	if (Iter == mScriptMap.end())
		return nullptr;


	// 만약 ScriptID가 없다면 return
	auto LineIter = Iter->second.find(ScriptLineID);
	if (LineIter == Iter->second.end())
		return nullptr;

	return LineIter->second;

}

void CScriptManager::AddScript(int NPCID, int ScriptID, 
	std::string NPCName, std::string Script)
{
	// 만약 NPC아이디가 없다면
	// ScriptMap 생성
	auto Iter = mScriptMap.find(NPCID);
	if (Iter == mScriptMap.end())
	{
		mScriptMap[NPCID] = std::unordered_map<int, FScriptLine*>();
		Iter = mScriptMap.find(NPCID);
	}

	// 만약 같은 대사 ID가 있다면 return
	auto& InnerMap = Iter->second;
	if (InnerMap.find(ScriptID) != InnerMap.end())
		return;
	
	FScriptLine* NewLine = new FScriptLine;
	NewLine->CurrentLineID = ScriptID;
	NewLine->NPCName = NPCName;
	NewLine->Text = Script;
	NewLine->NextLineID = -1;


	InnerMap[ScriptID] = NewLine;
}

void CScriptManager::SetNextLine(int NPCID, int CurrentScriptID, int NextScriptID)
{
	// NPCID로 맵을 순회후 없으면 설정X
	auto Iter = mScriptMap.find(NPCID);
	if (Iter == mScriptMap.end())
		return;

	// ScriptMap 순회
	auto& InnerMap = Iter->second;

	// 만약 현재대사가 맵에 없으면 return
	auto Iter2 = InnerMap.find(CurrentScriptID);
	if (Iter2 == InnerMap.end())
		return;
	
	// 다음대사가 맵에 없어도 return
	Iter2 = InnerMap.find(NextScriptID);
	if (Iter2 == InnerMap.end())
		return;

	Iter2 = InnerMap.find(CurrentScriptID);
	Iter2->second->NextLineID = NextScriptID;
}