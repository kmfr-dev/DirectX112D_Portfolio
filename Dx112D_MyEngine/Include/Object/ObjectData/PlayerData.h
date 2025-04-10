#pragma once

#include "BattleObjectData.h"

struct FInventorySaveData
{
	std::string ItemName;
	int Count = 1;
};

enum class EGameType
{
	NewGame,
	LoadGame,
	MoveNextScene,
	End
};

class CPlayerData :
    public CBattleObjectData
{
	friend class CPlayerManager;
	friend class CPlayerObject;
public:
	CPlayerData();
	CPlayerData(const CPlayerData& Data);
	CPlayerData(CPlayerData&& Data);
	virtual ~CPlayerData();

private:

	FVector3D		NextSpawnPos = FVector3D::Zero;
	ESceneType		CurrentScene = ESceneType::Scene1;
	FVector2D		ColliderSize = FVector2D(70.f, 60.f);

	// 퀵슬롯에 저장될 아이템
	FItemData*		QuickSlots[4] = { nullptr, nullptr, nullptr, nullptr };

	// 퀘스트 목록
	std::list<class CQuest*> mQuestList;
	// 완료된 퀘스트 목록 ID
	std::vector<int> mCompletedQuestIDs;

	// 저장하고 불러올 인벤토리 아이템 목록
	// 이름과 갯수만 저장하고 불러올때 
	// 아이템 매니저에서 클론해서 불러온다.

	std::vector<FInventorySaveData> mInventorySaveList;

	std::vector<std::pair<int, std::string>> mQuickItems;

	int				Gold = 5000;

	bool            Running = false;
	bool			CanSpeak = false;
	bool			IsTalking = false;
	int				CurrentNPCID = -1;

	EGameType		GameType = EGameType::NewGame;

public:
	virtual bool LoadFromFile(const char* FileName) override;
	virtual bool SaveToFile(const char* FileName) override;

public:
	ESceneType		 GetCurrentScene() const { return CurrentScene; }
	const FVector3D& GetNextSpawnPos() const { return NextSpawnPos; }
	const std::vector<FInventorySaveData>& GetInvenSaveList() const { return mInventorySaveList; }

public:
	void	SetCurrentScene(ESceneType Scene) { CurrentScene = Scene; }
	void	SetNextSpawnPos(const FVector3D& NextPos) { NextSpawnPos = NextPos; }
	void	SetInventoryItems(const std::vector<FItemData*>& Items);
	void	SetGameType(EGameType Type) { GameType = Type; }

};

