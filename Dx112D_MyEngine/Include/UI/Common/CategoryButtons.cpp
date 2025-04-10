#include "CategoryButtons.h"
#include "Button.h"
#include "TextBlock.h"
#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/Scene.h"

CCategoryButtons::CCategoryButtons()
{
}

CCategoryButtons::~CCategoryButtons()
{
}

bool CCategoryButtons::Init()
{
    mConsumable = mScene->GetUIManager()->CreateWidget<CButton>("CategoryConsumable");
    mConsumable->SetTexture(EButtonState::Normal, "ConsumBtn", TEXT("Texture\\UI\\Category_Eat.png"));
    mConsumable->SetTexture(EButtonState::Hovered, "ConsumBtn", TEXT("Texture\\UI\\Category_Eat.png"));
    mConsumable->SetTexture(EButtonState::Click, "ConsumBtn", TEXT("Texture\\UI\\Category_Eat.png"));
    mConsumable->SetSize(51.f, 57.f);

    AddWidget(mConsumable);

    mHat = mScene->GetUIManager()->CreateWidget<CButton>("CategoryHat");
    mHat->SetTexture(EButtonState::Normal, "HatBtn", TEXT("Texture\\UI\\Category_Hat.png"));
    mHat->SetTexture(EButtonState::Hovered, "HatBtn", TEXT("Texture\\UI\\Category_Hat.png"));
    mHat->SetTexture(EButtonState::Click, "HatBtn", TEXT("Texture\\UI\\Category_Hat.png"));
    mHat->SetSize(83.f, 57.f);
    mHat->SetPos(100.f, 0.f);
    AddWidget(mHat);

    mQuest = mScene->GetUIManager()->CreateWidget<CButton>("CategoryQuest");
    mQuest->SetTexture(EButtonState::Normal, "QuestBtn", TEXT("Texture\\UI\\Category_Quest.png"));
    mQuest->SetTexture(EButtonState::Hovered, "QuestBtn", TEXT("Texture\\UI\\Category_Quest.png"));
    mQuest->SetTexture(EButtonState::Click, "QuestBtn", TEXT("Texture\\UI\\Category_Quest.png"));
    mQuest->SetSize(62.f, 62.f);
    mQuest->SetPos(230.f, 0.f);
    AddWidget(mQuest);
    
    mGrass = mScene->GetUIManager()->CreateWidget<CButton>("CategoryGrass");
    mGrass->SetTexture(EButtonState::Normal, "GrassBtn", TEXT("Texture\\UI\\Category_Grass.png"));
    mGrass->SetTexture(EButtonState::Hovered, "GrassBtn", TEXT("Texture\\UI\\Category_Grass.png"));
    mGrass->SetTexture(EButtonState::Click, "GrassBtn", TEXT("Texture\\UI\\Category_Grass.png"));
    mGrass->SetSize(40.f, 57.f);
    mGrass->SetPos(350.f, 0.f);
    AddWidget(mGrass);



    return true;
}

void CCategoryButtons::Render()
{
    CUserWidget::Render();
}

void CCategoryButtons::Render(const FVector3D& Pos)
{
    CUserWidget::Render(Pos);
}
