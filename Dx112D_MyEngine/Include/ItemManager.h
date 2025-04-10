#pragma once

#include "GameInfo.h"

class CItemManager
{
private:
	std::unordered_map<std::string, FItemData*> mItemPrototype;

public:
	bool Init();

public:
	void CreateItem(const std::string Name, const std::string& ItemDesc,
		EItemType ItemType, EItemCategory Category, const std::string& TextureName,
		int Count, const std::vector<FItemStatInfo>& StatInfos, float UseTime,
		int SellPrice, int BuyPrice, bool Useable, bool Equipable);

	FItemData* FindItem(const std::string ItemName);
	FItemData* CloneItem(const std::string ItemName);

	DECLARE_SINGLE(CItemManager)
};

