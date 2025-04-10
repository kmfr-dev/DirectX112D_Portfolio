#include "DialogueWidget.h"

#include "../Common/DialogueBlock.h"
#include "../Common/NameTag.h"

#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/Scene.h"

CDialogueWidget::CDialogueWidget()
{
}

CDialogueWidget::~CDialogueWidget()
{
}

bool CDialogueWidget::Init()
{
    mDialogue = mScene->GetUIManager()->CreateWidget<CDialogueBlock>("Dialogue");
    mNameTag = mScene->GetUIManager()->CreateWidget<CNameTag>("NPCNameTag");
   
    mNameTag->SetPos(50.f, 370.f);
    //mDialogue->SetPos(100.f, 30.f);

    AddWidget(mDialogue);
    AddWidget(mNameTag);

    return true;
}

void CDialogueWidget::Update(float DeltaTime)
{
    CUserWidget::Update(DeltaTime);
}

void CDialogueWidget::Render()
{
    CUserWidget::Render();
}

void CDialogueWidget::Render(const FVector3D& Pos)
{
    CUserWidget::Render(Pos);
}

void CDialogueWidget::SetDialogueText(const TCHAR* DialougeText)
{
    mDialogue->SetDialogueText(DialougeText);
}

void CDialogueWidget::SetNameTagText(const TCHAR* NameTagText)
{
    mNameTag->SetNameTag(NameTagText);
}
