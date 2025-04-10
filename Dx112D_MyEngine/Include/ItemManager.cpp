#include "ItemManager.h"

#include "Asset/AssetManager.h"
#include "Asset/Texture/TextureManager.h"
#include "Asset/Texture/Texture.h"

#include "Scene/SceneAssetManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"


DEFINITION_SINGLE(CItemManager);

CItemManager::CItemManager()
{

}

CItemManager::~CItemManager()
{
	auto iter = mItemPrototype.begin();
	auto iterEnd = mItemPrototype.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	mItemPrototype.clear();
}

bool CItemManager::Init()
{
	CAssetManager::GetInst()->GetTextureManager()->
		LoadTexture("LemonFruit", TEXT("Texture\\UI\\Item\\Item_LemonFruit.PNG"));
	CAssetManager::GetInst()->GetTextureManager()->
		LoadTexture("HappyFruit",TEXT("Texture\\UI\\Item\\Item_HappyFruit.PNG"));
	CAssetManager::GetInst()->GetTextureManager()->
		LoadTexture("SadFruit", TEXT("Texture\\UI\\Item\\Item_SadFruit.PNG"));
	CAssetManager::GetInst()->GetTextureManager()->
		LoadTexture("SleepFruit", TEXT("Texture\\UI\\Item\\Item_SleepFruit.PNG"));
	CAssetManager::GetInst()->GetTextureManager()->
		LoadTexture("PumpkinFruit", TEXT("Texture\\UI\\Item\\Item_PumpkinFruit.PNG"));
	CAssetManager::GetInst()->GetTextureManager()->
		LoadTexture("OnionFruit", TEXT("Texture\\UI\\Item\\Item_OnionFruit.PNG"));
	CAssetManager::GetInst()->GetTextureManager()->
		LoadTexture("MelonFruit", TEXT("Texture\\UI\\Item\\Item_MelonFruit.PNG"));
	CAssetManager::GetInst()->GetTextureManager()->
		LoadTexture("PotatoFruit", TEXT("Texture\\UI\\Item\\Item_PotatoFruit.PNG"));
	CAssetManager::GetInst()->GetTextureManager()->
		LoadTexture("CubeFruit", TEXT("Texture\\UI\\Item\\Item_CubeFruit.PNG"));

	CreateItem("레몬", "레몬 이지만 레몬이 아닌 과일이다. 먹으면 체력을 3 올려준다.",
		EItemType::Food, EItemCategory::Consumable, "LemonFruit",
		1, { {EItemStatType::HP, 3} }, 0.f, 300, 500, true, false);
	CreateItem("신비한 과일", "왠지 모르게 먹으면 행복해지는 과일이다. 먹으면 체력을 3 올려준다.",
		EItemType::Food, EItemCategory::Consumable, "HappyFruit",
		1, { {EItemStatType::HP, 3} }, 0.f, 300, 500, true, false);
	CreateItem("슬픈 과일", "왠지 모르게 먹으면 슬퍼지는 과일이다. 먹으면 체력을 3 올려준다.",
		EItemType::Food, EItemCategory::Consumable, "SadFruit",
		1, { {EItemStatType::HP, 3} }, 0.f, 300, 500, true, false);
	CreateItem("졸린 과일", "먹으면 잠이오는 과일. 먹으면 체력을 3 올려준다.",
		EItemType::Food, EItemCategory::Consumable, "SleepFruit",
		1, { {EItemStatType::HP, 3} }, 0.f, 300, 500, true, false);

	CreateItem("기운이 솟는 호박", "먹으면 기운이 난다. 먹으면 체력을 5 올려준다.",
		EItemType::Food, EItemCategory::Consumable, "PumpkinFruit",
		1, { {EItemStatType::HP, 5} }, 0.f, 500, 800, true, false);
	CreateItem("영문을 모르겠는 과일", "양파 같아보이는 과일이다. 먹으면 체력을 5 올려준다.",
		EItemType::Food, EItemCategory::Consumable, "OnionFruit",
		1, { {EItemStatType::HP, 5} }, 0.f, 500, 800, true, false);
	CreateItem("메론", "그냥 메론. 먹으면 체력을 5 올려준다.",
		EItemType::Food, EItemCategory::Consumable, "MelonFruit",
		1, { {EItemStatType::HP, 5} }, 0.f, 500, 800, true, false);

	CreateItem("?", "5강의실 [0, 2]의 기운이 담긴 과일이다. 먹으면 체력을 10 올려준다. 맛은 없다고 한다...",
		EItemType::Food, EItemCategory::Consumable, "PotatoFruit",
		1, { {EItemStatType::HP, 10} }, 0.f, 1000, 1300, true, false);

	CreateItem("큐브 젤리", "오구 마을에서 유명한 가장 맛있는 젤리. 먹으면 체력을 10 올려준다.",
		EItemType::Food, EItemCategory::Consumable, "CubeFruit",
		1, { {EItemStatType::HP, 10} }, 0.f, 1000, 1300, true, false);

	return true;
}

void CItemManager::CreateItem(const std::string Name, 
	const std::string& ItemDesc, EItemType ItemType, 
	EItemCategory Category, const std::string& TextureName, 
	int Count, const std::vector<FItemStatInfo>& StatInfos,
	float UseTime, int SellPrice, int BuyPrice, bool Useable,
	bool Equipable)
{
	FItemData* NewItem = new FItemData;
	NewItem->Name = Name;
	NewItem->Desc = ItemDesc;
	NewItem->TextureName = TextureName;
	NewItem->Type = ItemType;
	NewItem->Category = Category;
	NewItem->Icon = CAssetManager::GetInst()->GetTextureManager()->FindTexture(TextureName);
	NewItem->Count = Count;
	NewItem->StatInfo = StatInfos;
	NewItem->UseTime = UseTime;
	NewItem->SellPrice = SellPrice;
	NewItem->BuyPrice = BuyPrice;
	NewItem->Useable = Useable;
	NewItem->Equipable = Equipable;

	mItemPrototype[Name] = NewItem;
}

FItemData* CItemManager::FindItem(const std::string ItemName)
{
	auto iter = mItemPrototype.find(ItemName);

	if (iter == mItemPrototype.end())
		return nullptr;

	return iter->second;
}

FItemData* CItemManager::CloneItem(const std::string ItemName)
{
	FItemData* OriginItem = FindItem(ItemName);

	if (!OriginItem)
		return nullptr;

	FItemData* CloneItem = new FItemData;
	CloneItem->Name = OriginItem->Name;
	CloneItem->Desc = OriginItem->Desc;
	CloneItem->TextureName = OriginItem->TextureName;
	CloneItem->Type = OriginItem->Type;
	CloneItem->Category = OriginItem->Category;
	CloneItem->Icon = CAssetManager::GetInst()->
		GetTextureManager()->FindTexture(CloneItem->TextureName);
	CloneItem->Count = OriginItem->Count;
	CloneItem->StatInfo = OriginItem->StatInfo;
	CloneItem->UseTime = OriginItem->UseTime;
	CloneItem->SellPrice = OriginItem->SellPrice;
	CloneItem->BuyPrice = OriginItem->BuyPrice;
	CloneItem->Useable = OriginItem->Useable;
	CloneItem->Equipable = OriginItem->Equipable;

	
	return CloneItem;
}
