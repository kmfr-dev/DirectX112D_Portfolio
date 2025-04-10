#pragma once
#include "../UserWidget/UserWidget.h"

class CNameTag :
    public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CNameTag();
	virtual ~CNameTag();

protected:
	CSharedPtr<class CImage>			mBack;
	CSharedPtr<class CTextBlock>		mNameText;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();

public:
	virtual void SetNameTag(const TCHAR* Name);
};

