#include "MouseWidget.h"
#include "../Common/Image.h"
#include "../../Scene/Scene.h"
#include "../../Scene/Input.h"
#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneManager.h"

CMouseWidget::CMouseWidget()
{
}

CMouseWidget::~CMouseWidget()
{
}

bool CMouseWidget::Init()
{
    CUserWidget::Init();

    mImage = CSceneUIManager::CreateWidgetStatic<CImage>("Mouse");

    mImage->SetTexture("MouseDefault", 
        TEXT("Texture\\UI\\Mouse\\Mouse1.png"));

    mImage->SetSize(54.5f, 53.5f);

    mImage->SetPos(-5.f, -13.f);

    AddWidget(mImage);

    return true;
}

void CMouseWidget::Update(float DeltaTime)
{
    CUserWidget::Update(DeltaTime);

	CInput* Input = 
		CSceneManager::GetInst()->GetCurrentScene()->GetInput();

	FVector2D MousePos = Input->GetMousePos();

	SetPos(MousePos.x, MousePos.y - 32.f);
}
