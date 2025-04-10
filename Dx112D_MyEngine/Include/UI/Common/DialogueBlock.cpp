#include "DialogueBlock.h"
#include "../Common/Image.h"
#include "../Common/TextBlock.h"

#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/Scene.h"

CDialogueBlock::CDialogueBlock()
{
}

CDialogueBlock::~CDialogueBlock()
{
}

bool CDialogueBlock::Init()
{
    mBack = mScene->GetUIManager()->CreateWidget<CImage>("DialogueBack");
    mBack->SetTexture("NameTag", TEXT("Texture\\UI\\Dialogue\\TextBox.png"));
    mBack->SetSize(1300.f, 438.f);

    mDialogueText = mScene->GetUIManager()->CreateWidget<CTextBlock>("NPCDialogue");
    mDialogueText->SetText(TEXT("대사"));
    mDialogueText->SetFontSize(30.f);
    mDialogueText->SetTextColor(255, 255, 255, 255);
    mDialogueText->SetAlignH(ETextAlignH::Center);
    mDialogueText->SetParent(this);
    mDialogueText->SetShadowOffset(2.f, 2.f);
    mDialogueText->SetSize(1300.f, 438.f);

    AddWidget(mBack);
    AddWidget(mDialogueText);

    return true;
}

void CDialogueBlock::Render()
{
    CUserWidget::Render();
}

void CDialogueBlock::Render(const FVector3D& Pos)
{
    CUserWidget::Render(Pos);
}

void CDialogueBlock::SetDialogueText(const TCHAR* Dialogue)
{
    mDialogueText->SetText(Dialogue);
}
