#include "ResourceInfo.h"
#include "../Common/Image.h"
#include "../Common/TextBlock.h"

#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/Scene.h"

CResourceInfo::CResourceInfo()
{
}

CResourceInfo::~CResourceInfo()
{
}

bool CResourceInfo::Init()
{

    mBack = mScene->GetUIManager()->CreateWidget<CImage>("InfoBack");
    mIcon = mScene->GetUIManager()->CreateWidget<CImage>("InfoIcon");
    
    mText = mScene->GetUIManager()->CreateWidget<CTextBlock>("ResourceText");
    mText->SetAlignH(ETextAlignH::Right);
    mText->SetTextColor(0, 0, 0, 255);

    mInfoText = mScene->GetUIManager()->CreateWidget<CTextBlock>("InfoText");
    mInfoText->SetAlignH(ETextAlignH::Center);
    mInfoText->SetTextColor(0, 0, 0, 255);

    AddWidget(mBack);
    AddWidget(mIcon);
    AddWidget(mText);
    AddWidget(mInfoText);

    return true;
}

void CResourceInfo::Update(float DeltaTime)
{
    CUserWidget::Update(DeltaTime);
}

void CResourceInfo::Render()
{
    CUserWidget::Render();
}

void CResourceInfo::Render(const FVector3D& Pos)
{
    CUserWidget::Render(Pos);
}

void CResourceInfo::SetBackTexture(const std::string& Name, const TCHAR* FileName)
{
    mBack->SetTexture(Name, FileName);
    mBack->SetOpacity(1.f);
}

void CResourceInfo::SetIconTexture(const std::string& Name, const TCHAR* FileName)
{
    mIcon->SetTexture(Name, FileName);
    mIcon->SetOpacity(1.f);
}

void CResourceInfo::SetText(const TCHAR* Name)
{
    mText->SetText(Name);
}

void CResourceInfo::SetInfoText(const TCHAR* Name)
{
    mInfoText->SetText(Name);
}

void CResourceInfo::SetBackSize(const FVector2D& Size)
{
    mBack->SetSize(Size);
}

void CResourceInfo::SetBackSize(float x, float y)
{
    mBack->SetSize(x, y);
}

void CResourceInfo::SetIconSize(const FVector2D& Size)
{
    mIcon->SetSize(Size);
}

void CResourceInfo::SetIconSize(float x, float y)
{
    mIcon->SetSize(x, y);
}

void CResourceInfo::SetTextSize(float Size)
{
    mText->SetFontSize(Size);
}

void CResourceInfo::SetTextBoxSize(const FVector2D& Size)
{
    mText->SetSize(Size);
}

void CResourceInfo::SetTextBoxSize(float x, float y)
{
    mText->SetSize(x, y);
}

void CResourceInfo::SetInfoTextSize(float Size)
{
    mInfoText->SetFontSize(Size);
}

void CResourceInfo::SetInfoTextBoxSize(const FVector2D& Size)
{
    mInfoText->SetSize(Size);
}

void CResourceInfo::SetInfoTextBoxSize(float x, float y)
{
    mInfoText->SetSize(x, y);
}

void CResourceInfo::SetBackPos(const FVector2D& Pos)
{
    mBack->SetPos(Pos);
}

void CResourceInfo::SetBackPos(float x, float y)
{
    mBack->SetPos(x, y);
}

void CResourceInfo::SetIconPos(const FVector2D& Pos)
{
    mIcon->SetPos(Pos);
}

void CResourceInfo::SetIconPos(float x, float y)
{
    mIcon->SetPos(x, y);
}

void CResourceInfo::SetTextPos(const FVector2D& Pos)
{
    mText->SetPos(Pos);
}

void CResourceInfo::SetTextPos(float x, float y)
{
    mText->SetPos(x, y);
}

void CResourceInfo::SetInfoTextPos(const FVector2D& Pos)
{
    mInfoText->SetPos(Pos);
}

void CResourceInfo::SetInfoTextPos(float x, float y)
{
    mInfoText->SetPos(x, y);
}
