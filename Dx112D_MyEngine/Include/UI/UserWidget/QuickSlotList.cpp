#include "QuickSlotList.h"
#include "Slot.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"

CQuickSlotList::CQuickSlotList()
{
}

CQuickSlotList::~CQuickSlotList()
{
}

bool CQuickSlotList::Init()
{
    mQuickSlot1 = mScene->GetUIManager()->CreateWidget<CSlot>("QuickSlot1");
    mQuickSlot1->SetTextureBack("QuickSlot", TEXT("Texture\\UI\\QuickSlot.png"));
    
    mQuickSlot1->SetSize(70.f, 70.f);
    mQuickSlot1->SetIconSize(50.f, 50.f);
    mQuickSlot1->SetIconPos(10.f, 10.f);
    AddWidget(mQuickSlot1);

    mQuickSlot2 = mScene->GetUIManager()->CreateWidget<CSlot>("QuickSlot2");
    mQuickSlot2->SetTextureBack("QuickSlot", TEXT("Texture\\UI\\QuickSlot.png"));
    mQuickSlot2->SetSize(70.f, 70.f);
    mQuickSlot2->SetPos(75.f, 0.f);
    mQuickSlot2->SetIconSize(50.f, 50.f);
    mQuickSlot2->SetIconPos(10.f, 10.f);

    AddWidget(mQuickSlot2);

    mQuickSlot3 = mScene->GetUIManager()->CreateWidget<CSlot>("QuickSlot3");
    mQuickSlot3->SetTextureBack("QuickSlot", TEXT("Texture\\UI\\QuickSlot.png"));

    mQuickSlot3->SetSize(70.f, 70.f);
    mQuickSlot3->SetPos(150.f, 0.f);
    mQuickSlot3->SetIconSize(50.f, 50.f);
    mQuickSlot3->SetIconPos(10.f, 10.f);
    AddWidget(mQuickSlot3);

    mQuickSlot4 = mScene->GetUIManager()->CreateWidget<CSlot>("QuickSlot4");
    mQuickSlot4->SetTextureBack("QuickSlot", TEXT("Texture\\UI\\QuickSlot.png"));

    mQuickSlot4->SetSize(70.f, 70.f);
    mQuickSlot4->SetPos(225.f, 0.f);
    mQuickSlot4->SetIconSize(50.f, 50.f);
    mQuickSlot4->SetIconPos(10.f, 10.f);
    AddWidget(mQuickSlot4);

    return true;
}

void CQuickSlotList::Update(float DeltaTime)
{
    CUserWidget::Update(DeltaTime);
}

void CQuickSlotList::Render()
{
    CUserWidget::Render();
}

void CQuickSlotList::SetSlotIcon(int Index, CTexture* Icon)
{
    if (CSlot* Slot = GetSlotByIndex(Index))
        Slot->SetTextureIcon(Icon);
}

CSlot* CQuickSlotList::GetSlotByIndex(int Index)
{
    switch (Index)
    {
    case 0: 
        return mQuickSlot1.Get();
    case 1: 
        return mQuickSlot2.Get();
    case 2: 
        return mQuickSlot3.Get();
    case 3: 
        return mQuickSlot4.Get();
    }

    return nullptr;
}
