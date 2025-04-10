#pragma once
#include "../UserWidget/UserWidget.h"

class CStoreItemInfo :
    public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CStoreItemInfo();
	virtual ~CStoreItemInfo();

protected:
	// 아이템 정보 BackGround 이미지
	CSharedPtr<class CImage>	mBack;
	// 아이템을 보여줄 Slot
	CSharedPtr<class CSlot>		mSlot;
	// 아이템 설명 Text
	CSharedPtr<class CTextBlock> mItemInfo;
	// 아이템 구매 버튼
	CSharedPtr<class CButton> mBuyBtn;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();


public:
	// ������ ����â �����Լ�
	void SetItemInfo(FItemData* ItemData);
};

