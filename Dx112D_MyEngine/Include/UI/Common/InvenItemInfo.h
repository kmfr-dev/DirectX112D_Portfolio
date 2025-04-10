#pragma once
#include "../UserWidget/UserWidget.h"

class CInvenItemInfo :
    public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CInvenItemInfo();
	virtual ~CInvenItemInfo();

protected:

	// 아이템 정보 위젯 Back 이미지
	CSharedPtr<class CImage>	mBack;
	// 아이템이 들어갈 슬롯
	CSharedPtr<class CSlot>		mSlot;
	// 아이템 설명
	CSharedPtr<class CTextBlock> mItemInfo;
	// 퀵슬롯 등록 버튼
	CSharedPtr<class CButton> mQuick1;
	CSharedPtr<class CButton> mQuick2;
	CSharedPtr<class CButton> mQuick3;
	CSharedPtr<class CButton> mQuick4;
	// 아이템 사용 버튼
	CSharedPtr<class CButton> mItemUse;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();

public:
	// 아이템 정보창 설정함수
	void SetItemInfo(FItemData* ItemData);
};

