#include "WindowWidget.h"
#include "Slot.h"

class CStoreWidget :
    public CWindowWidget
{
	friend class CSceneUIManager;

protected:
	CStoreWidget();
	virtual ~CStoreWidget();

protected:
	// 상점 배경 이미지
	CSharedPtr<class CImage>		mStoreBack;
	// 상점 텍스트
	CSharedPtr<class CTextBlock>	mStoreText;
	// 골드 정보 위젯
	CSharedPtr<class CResourceInfo> mGoldInfo;
	// 아이템 정보 표시 위젯
	CSharedPtr<class CStoreItemInfo> mItemInfo;
	// 판매할 아이템 슬롯 목록
	std::vector<CSharedPtr<CSlot>>	mSlotList;

	// 상점 컴포넌트
	class CStoreComponent* mStoreComponent = nullptr;

	// 현재 선택된 아이템
	FItemData*  mSelectedItem = nullptr;
	int			mSelectedSlotIndex = -1;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();

public:
	// 현재 선택된 아이템을 가져온다.
	FItemData* GetSelectedItem() const { return mSelectedItem; }
	class CStoreComponent* GetStoreComponent() const { return mStoreComponent; }
	
public:
	void SetStoreComponent(class CStoreComponent* Com) { mStoreComponent = Com; }
	// 선택된 아이템 설정함수
	void SetSelectedItem(FItemData* Item) { mSelectedItem = Item; }
	// 골드 정보 설정 함수
	void SetGoldInfo(int Gold);
	// 상점에 아이템목록을 등록해주는 함수.
	void SetItemList(const std::vector<FItemData*>& ItemList);

public:
	void StoreChange(const FItemData* Item, int Index);

	// 슬롯을 클릭했을 때 호출해주는 함수.
	void SelectSlot(int Index);
};
