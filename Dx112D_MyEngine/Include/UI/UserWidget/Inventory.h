#pragma once

#include "WindowWidget.h"
#include "Slot.h"

// 인벤토리 전체 위젯
class CInventory :
    public CWindowWidget
{
	friend class CSceneUIManager;

protected:
	CInventory();
	virtual ~CInventory();

protected:
	// 인벤토리 배경 이미지
	CSharedPtr<class CImage>		mInvenBack;
	// 골드 정보 위젯
	CSharedPtr<class CResourceInfo> mGoldInfo;
	// 아이템 정보 표시 위젯
	CSharedPtr<class CInvenItemInfo> mItemInfo;
	// 현재 카테고리
	EItemCategory mCurrentCategory = EItemCategory::Consumable;
	// 카테고리 버튼들(4개)
	CSharedPtr<class CCategoryButtons> mCategoryBtns;
	// 아이템 슬롯 목록
	std::vector<CSharedPtr<CSlot>>	mSlotList;

	FItemData* mSelectedItem = nullptr;
	int	mSelectedSlotIndex = -1;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
	virtual void Render() override;
	virtual void SetEnable(bool Enable) override;

	// 선택된 아이템 설정함수
	void SetSelectedItem(FItemData* Item) { mSelectedItem = Item; }
	// 골드 정보 설정 함수
	void		SetGoldInfo(int Gold);

	// 카테고리 설정 함수
	void		SetCategory(EItemCategory Category);
	// 카테고리 새로고침 함수
	void		RefreshCategory();

public:
	FItemData* GetSelectedItem() const
	{
		return mSelectedItem;
	}

	EItemCategory GetCurrentCategory() const
	{
		return mCurrentCategory;
	}

public:
	void InventoryChange(const FItemData* Item, int Index);

	// 버튼을 눌렀을 때 카테고리를 바꿔주는 함수.
	void ChangeConsumCategory();
	void ChangeHatCategory();
	void ChangeQuestCategory();
	void ChangeGrassCategory();

	// 슬롯을 클릭했을 때 호출해주는 함수.
	void SelectSlot(int Index);
};