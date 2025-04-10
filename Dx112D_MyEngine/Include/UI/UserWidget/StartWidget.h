#pragma once

#include "UserWidget.h"

class CStartWidget :
    public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CStartWidget();
	virtual ~CStartWidget();

protected:
	CSharedPtr<class CButton>	mStartButton;
	CSharedPtr<class CButton>	mEditorButton;
	CSharedPtr<class CButton>	mExitButton;

	CSharedPtr<class CImage>	mStartTitle;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
private:
	void StartButtonClick();
	void EditorButtonClick();
	void ExitButtonClick();
};

