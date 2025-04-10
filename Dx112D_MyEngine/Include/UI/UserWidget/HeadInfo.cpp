#include "HeadInfo.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"

CHeadInfo::CHeadInfo()
{
}

CHeadInfo::~CHeadInfo()
{
}

bool CHeadInfo::Init()
{
    mHPBar = mScene->GetUIManager()->CreateWidget<CProgressBar>("HPBar");
    mNameText = mScene->GetUIManager()->CreateWidget<CTextBlock>("NameText");

    //mHPBar->SetPos(50.f, 500.f);
    mHPBar->SetSize(800.f, 20.f);
    mHPBar->SetTint(EProgressBarImageType::Back, 0.f, 0.f, 0.f);
    mHPBar->SetTint(EProgressBarImageType::Fill, 200.f, 0.f, 0.f);

    //mHPBar->SetPercent(0.7f);

    AddWidget(mHPBar);

    mNameText->SetText(TEXT("타락한 누에 용사"));
    mNameText->SetTextColor(255, 255, 255, 255);
    mNameText->SetAlignH(ETextAlignH::Center);
    mNameText->SetFontSize(20.f);
    mNameText->SetShadowEnable(true);
    mNameText->SetShadowOffset(2.f, 2.f);
    mNameText->SetPos(mHPBar->GetSize().x * 0.5f - 100.f, 30.f);
    mNameText->SetSize(200.f, 40.f);

    AddWidget(mNameText);

    return true;
}

void CHeadInfo::Render()
{
    CUserWidget::Render();
}

void CHeadInfo::Render(const FVector3D& Pos)
{
    CUserWidget::Render(Pos);
}
