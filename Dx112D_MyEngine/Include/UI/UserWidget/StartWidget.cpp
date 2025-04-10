#include "StartWidget.h"
#include "../Common/Button.h"
#include "../Common/Image.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/SceneMain.h"
#include "../../Scene/Scene2.h"
#include "../../Scene/SceneBoss.h"
#include "../../Scene/SceneEditor.h"
#include "../../Device.h"
#include "../../GameManager.h"
#include "../Common/TextBlock.h"
#include "../../PlayerManager.h"
#include "../../Object/ObjectData/PlayerData.h"


CStartWidget::CStartWidget()
{
}

CStartWidget::~CStartWidget()
{
}

bool CStartWidget::Init()
{
    CUserWidget::Init();

    FResolution RS = CDevice::GetInst()->GetResolution();

    mStartButton = mScene->GetUIManager()->CreateWidget<CButton>("StartButton");
    mEditorButton = mScene->GetUIManager()->CreateWidget<CButton>("EditorButton");
    mExitButton = mScene->GetUIManager()->CreateWidget<CButton>("ExitButton");
    mStartTitle = mScene->GetUIManager()->CreateWidget<CImage>("StartTitle");
    mStartTitle->SetTexture("StartTitle", TEXT("Texture\\StartScene\\StartTitle.png"));

  
    mStartButton->SetSize(200.f, 100.f);
    mEditorButton->SetSize(200.f, 100.f);
    mExitButton->SetSize(200.f, 100.f);
    mStartTitle->SetSize(481.f, 281.f);

    mStartButton->SetPos(70.f,
        50.f);
    mEditorButton->SetPos(370.f,
        50.f);
    mExitButton->SetPos(670.f,
        50.f);

    mStartTitle->SetPos(50.f,
        RS.Height * 0.5f + 50.f);

    CSharedPtr<CTextBlock> StartBtnText = mScene->GetUIManager()->CreateWidget<CTextBlock>("StartBtnText");
    CSharedPtr<CTextBlock> EditorBtnText = mScene->GetUIManager()->CreateWidget<CTextBlock>("StartBtnText");
    CSharedPtr<CTextBlock> ExitBtnText = mScene->GetUIManager()->CreateWidget<CTextBlock>("ExitBtnText");

    StartBtnText->SetText(TEXT("GameStart"));
    StartBtnText->SetFontSize(25.f);
    StartBtnText->SetTextColor(FVector4D::Black);
    StartBtnText->SetAlignH(ETextAlignH::Center);
    mStartButton->SetChild(StartBtnText);

    EditorBtnText->SetText(TEXT("Editor"));
    EditorBtnText->SetFontSize(25.f);
    EditorBtnText->SetTextColor(FVector4D::Black);
    EditorBtnText->SetAlignH(ETextAlignH::Center);
    mEditorButton->SetChild(EditorBtnText);

    ExitBtnText->SetText(TEXT("Exit"));
    ExitBtnText->SetFontSize(25.f);
    ExitBtnText->SetTextColor(FVector4D::Black);
    ExitBtnText->SetAlignH(ETextAlignH::Center);
    mExitButton->SetChild(ExitBtnText);

    AddWidget(mStartButton);
    AddWidget(mExitButton);
    AddWidget(mEditorButton);
    AddWidget(mStartTitle);

    mStartButton->SetEventCallback<CStartWidget>(
        EButtonEventState::Click,
        this, &CStartWidget::StartButtonClick);

    mEditorButton->SetEventCallback<CStartWidget>(
        EButtonEventState::Click,
        this, &CStartWidget::EditorButtonClick);

    mExitButton->SetEventCallback<CStartWidget>(
        EButtonEventState::Click,
        this, &CStartWidget::ExitButtonClick);

    return true;
}

void CStartWidget::Update(float DeltaTime)
{
    CUserWidget::Update(DeltaTime);
}

void CStartWidget::StartButtonClick()
{
    if (!CPlayerManager::GetInst()->Init())
        return;

    CPlayerManager::GetInst()->Load();
    
    ESceneType PlayerScene = CPlayerManager::GetInst()->
        GetPlayerData()->GetCurrentScene();

    switch (PlayerScene)
    {
    case ESceneType::Scene1:
        CSceneManager::GetInst()->CreateLoadScene<CSceneMain>();
        break;
    case ESceneType::Scene2:
        CSceneManager::GetInst()->CreateLoadScene<CScene2>();
        break;
    case ESceneType::SceneBoss:
        CSceneManager::GetInst()->CreateLoadScene<CSceneBoss>();
        break;
    }

}

void CStartWidget::EditorButtonClick()
{
    CSceneManager::GetInst()->CreateLoadScene<CSceneEditor>();
}

void CStartWidget::ExitButtonClick()
{
    CGameManager::GetInst()->ExitGame();
}
