#include "PlayerData.h"
#include "../../Quest/Quest.h"
#include "../../Quest/QuestManager.h"
#include "../../Scene/SceneAssetManager.h"
#include "../../Asset/AssetManager.h"
#include "../../Asset/Texture/TextureManager.h"
#include "../../Asset/Texture/Texture.h"


CPlayerData::CPlayerData()
{
}
CPlayerData::CPlayerData(const CPlayerData& Data)
{
}
CPlayerData::CPlayerData(CPlayerData&& Data)
{
}
CPlayerData::~CPlayerData()
{
   
}

bool CPlayerData::LoadFromFile(const char* FileName)
{
    char FullPath[MAX_PATH] = {};
    strcpy_s(FullPath, gRootPathMultibyte);
    strcat_s(FullPath, "Asset\\Data\\PlayerData\\");
    strcat_s(FullPath, FileName);

    FILE* File = nullptr;
    fopen_s(&File, FullPath, "rb");

    if (!File)
    {
        Type = EObjectType::Player;
        CurrentScene = ESceneType::Scene1;
        Pos = FVector2D(300.f, 3796.f);
        Scale = FVector3D(150.f, 150.f, 1.f);
        Pivot = FVector2D(0.5f, 0.45f);
        Dir = EObjectDir::B;
        ColliderSize = FVector2D(70.f, 60.f);

        HP = 20;
        HPMax = 20;
        Attack = 1;
        Gold = 1000;
        Speed = 450.f;

        CanMove = true;
        Moving = false;
        Running = false;

        CanAttack = true;
        Attacking = false;

        Hitting = false;
        Invincible = false;
        CanSpeak = false;
        CurrentNPCID = -1;

        GameType = EGameType::NewGame;

        return false;
    }

    fread(&Type, sizeof(EObjectType), 1, File);
    fread(&CurrentScene, sizeof(ESceneType), 1, File);
    fread(&NextSpawnPos, sizeof(FVector3D), 1, File);
    fread(&Pos, sizeof(FVector2D), 1, File);
    fread(&Scale, sizeof(FVector3D), 1, File);
    fread(&Pivot, sizeof(FVector2D), 1, File);
    fread(&Dir, sizeof(EObjectDir), 1, File);

    fread(&HP, sizeof(int), 1, File);
    fread(&HPMax, sizeof(int), 1, File);
    fread(&Attack, sizeof(int), 1, File);
    fread(&Gold, sizeof(float), 1, File);
    fread(&Speed, sizeof(float), 1, File);

    fread(&CanMove, sizeof(bool), 1, File);
    fread(&Moving, sizeof(bool), 1, File);
    fread(&Running, sizeof(bool), 1, File);

    fread(&CanAttack, sizeof(bool), 1, File);
    fread(&Attacking, sizeof(bool), 1, File);

    fread(&Hitting, sizeof(bool), 1, File);
    fread(&Invincible, sizeof(bool), 1, File);
    fread(&CanSpeak, sizeof(bool), 1, File);
    fread(&CurrentNPCID, sizeof(int), 1, File);

    int ItemCount = 0;
    fread(&ItemCount, sizeof(int), 1, File);

    for (int i = 0; i < ItemCount; ++i)
    {
        size_t NameLength = 0;
        fread(&NameLength, sizeof(size_t), 1, File);

        char NameBuf[256] = {};
        fread(NameBuf, sizeof(char), NameLength, File);

        int Count = 0;
        fread(&Count, sizeof(int), 1, File);

        FInventorySaveData LoadData;
        LoadData.ItemName = std::string(NameBuf, NameLength);
        LoadData.Count = Count;

        mInventorySaveList.push_back(LoadData);
    }

    // 퀘스트 수 불러오기
    int QuestCount = 0;
    fread(&QuestCount, sizeof(int), 1, File);

    // 퀘스트 정보 불러오기
    for (int i = 0; i < QuestCount; ++i)
    {
        int QuestID, QuestState, CurrentCount;
        fread(&QuestID, sizeof(int), 1, File);
        fread(&QuestState, sizeof(int), 1, File);
        fread(&CurrentCount, sizeof(int), 1, File);

        // 퀘스트 원본을 매니저에서 불러온다.
        CQuest* Quest = CQuestManager::GetInst()->FindQuest(QuestID);
        // 없으면 continue
        if (!Quest)
            continue;

        Quest->SetQuestState((EQuestState)QuestState);
        Quest->SetCurrentCount(CurrentCount);

        mQuestList.push_back(Quest);
    }

    // 완료한 퀘스트 불러오기
    int CompletedCount = 0;
    fread(&CompletedCount, sizeof(int), 1, File);

    mCompletedQuestIDs.clear();
    for (int i = 0; i < CompletedCount; ++i)
    {
        int ID = 0;
        fread(&ID, sizeof(int), 1, File);
        mCompletedQuestIDs.push_back(ID);
    }

    fclose(File);

    GameType = EGameType::LoadGame;

    return true;
}
bool CPlayerData::SaveToFile(const char* FileName)
{
    char FullPath[MAX_PATH] = {};
    strcpy_s(FullPath, gRootPathMultibyte);
    strcat_s(FullPath, "Asset\\Data\\PlayerData\\");
    strcat_s(FullPath, FileName);

    FILE* File = nullptr;
    fopen_s(&File, FullPath, "wb");

    if (!File)
        return false;

    // 플레이어 정보 저장
    fwrite(&Type, sizeof(EObjectType), 1, File);
    fwrite(&CurrentScene, sizeof(ESceneType), 1, File);
    fwrite(&NextSpawnPos, sizeof(FVector3D), 1, File);
    fwrite(&Pos, sizeof(FVector2D), 1, File);
    fwrite(&Scale, sizeof(FVector3D), 1, File);
    fwrite(&Pivot, sizeof(FVector2D), 1, File);
    fwrite(&Dir, sizeof(EObjectDir), 1, File);

    fwrite(&HP, sizeof(int), 1, File);
    fwrite(&HPMax, sizeof(int), 1, File);
    fwrite(&Attack, sizeof(int), 1, File);
    fwrite(&Gold, sizeof(float), 1, File);
    fwrite(&Speed, sizeof(float), 1, File);

    fwrite(&CanMove, sizeof(bool), 1, File);
    fwrite(&Moving, sizeof(bool), 1, File);
    fwrite(&Running, sizeof(bool), 1, File);

    fwrite(&CanAttack, sizeof(bool), 1, File);
    fwrite(&Attacking, sizeof(bool), 1, File);

    fwrite(&Hitting, sizeof(bool), 1, File);
    fwrite(&Invincible, sizeof(bool), 1, File);
    fwrite(&CanSpeak, sizeof(bool), 1, File);
    fwrite(&CurrentNPCID, sizeof(int), 1, File);

    // 아이템 정보 저장
    int ItemCount = static_cast<int>(mInventorySaveList.size());
    fwrite(&ItemCount, sizeof(int), 1, File);

    for (const auto& Item : mInventorySaveList)
    {
        size_t NameLength = Item.ItemName.size();
        fwrite(&NameLength, sizeof(size_t), 1, File);
        fwrite(Item.ItemName.c_str(), sizeof(char), NameLength, File);
        fwrite(&Item.Count, sizeof(int), 1, File);
    }

    // 퀘스트 수 저장
    int QuestCount = (int)mQuestList.size();
    fwrite(&QuestCount, sizeof(int), 1, File);

    // 각각의 퀘스트 저장
    for (CQuest* Quest : mQuestList)
    {
        int QuestID = Quest->GetQuestID();
        int QuestState = (int)Quest->GetQuestState();
        int CurrentCount = Quest->GetQuestGoal().CurrentCount;

        fwrite(&QuestID, sizeof(int), 1, File);
        fwrite(&QuestState, sizeof(int), 1, File);
        fwrite(&CurrentCount, sizeof(int), 1, File);
    }

    // 완료한 퀘스트 개수 저장
    int CompletedCount = (int)mCompletedQuestIDs.size();
    fwrite(&CompletedCount, sizeof(int), 1, File);

    // 각각의 퀘스트 ID 저장
    for (int ID : mCompletedQuestIDs)
        fwrite(&ID, sizeof(int), 1, File);

    fclose(File);

    return true;
}

void CPlayerData::SetInventoryItems(const std::vector<FItemData*>& Items)
{
    mInventorySaveList.clear();

    for (FItemData* Item : Items)
    {
        if (!Item) 
            continue;

        mInventorySaveList.push_back({Item->Name, Item->Count});
    }
}
