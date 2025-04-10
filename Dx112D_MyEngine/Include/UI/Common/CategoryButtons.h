#pragma once
#include "../UserWidget/UserWidget.h"

class CCategoryButtons :
    public CUserWidget
{
	friend class CSceneUIManager;
	friend class CInventory;

protected:
	CCategoryButtons();
	virtual ~CCategoryButtons();

protected:
	CSharedPtr<class CButton> mConsumable;
	CSharedPtr<class CButton> mHat;
	CSharedPtr<class CButton> mQuest;
	CSharedPtr<class CButton> mGrass;

public:
	virtual bool Init();
	virtual void Render();
	virtual void Render(const FVector3D& Pos);

};

