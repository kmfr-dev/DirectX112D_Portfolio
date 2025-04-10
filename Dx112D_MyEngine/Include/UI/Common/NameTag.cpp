#include "NameTag.h"
#include "../Common/Image.h"
#include "../Common/TextBlock.h"

#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/Scene.h"

CNameTag::CNameTag()
{
}

CNameTag::~CNameTag()
{
}

bool CNameTag::Init()
{
    mBack = mScene->GetUIManager()->CreateWidget<CImage>("NameTagBack");
    mBack->SetTexture("NameTag", TEXT("Texture\\UI\\Dialogue\\NameTag.png"));
    mBack->SetTint(255.f, 255.f, 255.f);
    mBack->SetSize(200.f, 70.f);


    mNameText = mScene->GetUIManager()->CreateWidget<CTextBlock>("NPCName");
    

    mNameText->SetText(TEXT("이름"));
    mNameText->SetFontSize(20.f);
    mNameText->SetTextColor(0, 0, 0, 255);
    mNameText->SetAlignH(ETextAlignH::Center);
    mNameText->SetParent(this);
    mNameText->SetShadowOffset(2.f, 2.f);
    mNameText->SetSize(200.f, 70.f);


    AddWidget(mBack);
    AddWidget(mNameText);

    return true;
}

void CNameTag::Update(float DeltaTime)
{
    CUserWidget::Update(DeltaTime);
}

void CNameTag::Render()
{
    CUserWidget::Render();
}

void CNameTag::SetNameTag(const TCHAR* Name)
{
    mNameText->SetText(Name);
}
