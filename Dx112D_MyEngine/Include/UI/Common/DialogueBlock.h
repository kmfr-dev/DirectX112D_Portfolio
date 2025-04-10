#pragma once
#include "../UserWidget/UserWidget.h"

class CDialogueBlock :
    public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CDialogueBlock();
	virtual ~CDialogueBlock();

protected:
	CSharedPtr<class CImage>			mBack;
	CSharedPtr<class CTextBlock>		mDialogueText;

public:
	virtual bool Init();
	virtual void Render();
	virtual void Render(const FVector3D& Pos);

public:
	// 대사 텍스트 설정 함수
	void		SetDialogueText(const TCHAR* Dialogue);
};

