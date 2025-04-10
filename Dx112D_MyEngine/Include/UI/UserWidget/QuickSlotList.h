#pragma once
#include "UserWidget.h"
class CQuickSlotList :
    public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CQuickSlotList();
	virtual ~CQuickSlotList();

protected:
	CSharedPtr<class CSlot> mQuickSlot1;
	CSharedPtr<class CSlot> mQuickSlot2;
	CSharedPtr<class CSlot> mQuickSlot3;
	CSharedPtr<class CSlot> mQuickSlot4;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();

public:
	void SetSlotIcon(int Index, class CTexture* Icon);

	class CSlot* GetSlotByIndex(int Index);
};

