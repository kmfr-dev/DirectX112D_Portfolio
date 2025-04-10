#include "QuestTag.h"

#include "../../Scene/Scene.h"
#include "../../Scene/Input.h"
#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneManager.h"

#include "../Common/Image.h"
#include "../Common/TextBlock.h"

#include "../../Quest/Quest.h"

#include "../../Share/Log.h"
#include "../../Asset/Texture/Texture.h"

CQuestTag::CQuestTag()
{
}

CQuestTag::~CQuestTag()
{
}

bool CQuestTag::Init()
{
    CUserWidget::Init();

    mBack = mScene->GetUIManager()->CreateWidget<CImage>("QuestTagBack");
    mBack->SetTexture("NameTag", TEXT("Texture\\UI\\ItemInfoBack.png"));

    mNameText = mScene->GetUIManager()->CreateWidget<CTextBlock>("QuestTagText");
    mNameText->SetText(TEXT(""));
    mNameText->SetFontSize(27.f);
    mNameText->SetTextColor(0, 0, 0, 255);
    mNameText->SetAlignH(ETextAlignH::Center);

    AddWidget(mBack);
    AddWidget(mNameText);

    return true;
}

void CQuestTag::Update(float DeltaTime)
{
    CUserWidget::Update(DeltaTime);

    if (mScene->GetInput()->GetMouseDown(EMouseButtonType::LButton) &&
        mMouseOn)
    {
        if (mClickCallback)
            mClickCallback();
    }
}

void CQuestTag::Render()
{
    CUserWidget::Render();
}

void CQuestTag::SetNameTag(const TCHAR* Name)
{
    mNameText->SetText(Name);  
}

void CQuestTag::SetSize(const FVector2D& Size)
{
    CUserWidget::SetSize(Size);
    mBack->SetSize(Size);
    mNameText->SetSize(Size);
}

void CQuestTag::SetSize(float x, float y)
{
    CUserWidget::SetSize(x, y);
    mBack->SetSize(x, y);
    mNameText->SetSize(x, y);
}

void CQuestTag::SetFontSize(float Size)
{
    mNameText->SetFontSize(Size);
}

void CQuestTag::SetText(const TCHAR* Text)
{
    mNameText->SetText(Text);
}

void CQuestTag::SetSelected(bool Select)
{
    if (Select)
    {
        mBack->SetTint(0.f, 255.f, 0.f);
    }
    else
    {
        mBack->SetTint(255.f, 255.f, 255.f);
    }
}

void CQuestTag::SetTint(float r, float g, float b)
{
    mBack->SetTint(r, g, b);
}

bool CQuestTag::CollisionMouse(CWidget** Result, const FVector2D& MousePos)
{
    return CWidget::CollisionMouse(Result, MousePos);
}
