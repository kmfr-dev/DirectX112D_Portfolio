#pragma once
#include "UserWidget.h"
class CDialogueWidget :
    public CUserWidget
{
    friend class CSceneUIManager;

protected:
    CDialogueWidget();
    virtual ~CDialogueWidget();

protected:
    CSharedPtr<class CDialogueBlock>     mDialogue;
    CSharedPtr<class CNameTag> mNameTag;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();
    virtual void Render(const FVector3D& Pos);

public:
    void SetDialogueText(const TCHAR* DialougeText);
    void SetNameTagText(const TCHAR* NameTagText);
};

