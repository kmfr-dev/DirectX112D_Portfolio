#include "Slot.h"
#include "../Common/Image.h"
#include "../Common/TextBlock.h"
#include "../../Share/Log.h"
#include "../../Scene/Scene.h"
#include "../../Scene/Input.h"
#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneAssetManager.h"
#include "../../Asset/AssetManager.h"
#include "../../Asset/Texture/Texture.h"
#include "../../Asset/Texture/TextureManager.h"
#include "../../Asset/Mesh/Mesh.h"
#include "../../Shader/UICBuffer.h"
#include "../../Shader/TransformCBuffer.h"
#include "../../Shader/Shader.h"

CSlot::CSlot()
{
}

CSlot::~CSlot()
{
}

void CSlot::SetItemData(FItemData* Data)
{
    // 아이템 데이터에 인자로 넘어온 아이템을 넣어준다.
    mItemData = Data;


    // 만약 아이템이 있다면
    if(Data)
    {
        if (mSlotType == ESlotType::Inventory)
        {
            // 만약 슬롯 타입이 인벤토리라면
            // 아이템 갯수 텍스트를 설정해준다. 
            mInfoText->SetText(ToWString('x' + std::to_string(Data->Count)).c_str());
            SetInfoTextBoxSize(70.f, 30.f);
        }

        else if (mSlotType == ESlotType::Store)
        {
            // 만약 슬롯 타입이 상점이라면
            // 아이템 판매 금액 텍스트를 설정해준다.
            mInfoText->SetText(ToWString(std::to_string(Data->BuyPrice) + " Gold").c_str());
            SetInfoTextBoxSize(156.f, 45.f);
        }

        // 아이템 정보 활성화
        SetTextPos(2.f, 2.f);
        mInfoText->SetEnable(true);
        mInfoBack->SetEnable(true);
    }

    // 만약 아이템이 없다면
    else
    {
        // 슬롯에 Icon텍스쳐를 빈 텍스쳐로 설정한다.
        SetTextureIcon(nullptr);
        // 아이템 갯수정보 비활성화.
        mInfoText->SetText(TEXT(""));
        mInfoText->SetEnable(false);
        mInfoBack->SetEnable(false);
    }
}

#pragma region SlotSetFunction
void CSlot::SetTextureBack(const std::string& Name)
{
    mBack->SetTexture(Name);
}

void CSlot::SetTextureBack(const std::string& Name,
    const TCHAR* FileName)
{
    mBack->SetTexture(Name, FileName);
}

void CSlot::SetTextureBack(CTexture* Texture)
{
    mBack->SetTexture(Texture);
}

void CSlot::SetTintBack(float r, float g, float b)
{
    mBack->SetTint(r, g, b);
}

void CSlot::SetOpacityBack(float Opacity)
{
    mBack->SetOpacity(Opacity);
}

void CSlot::SetTextureIcon(const std::string& Name)
{
    mIcon->SetTexture(Name);

    mIcon->SetOpacity(1.f);
}

void CSlot::SetTextureIcon(const std::string& Name,
    const TCHAR* FileName)
{
    mIcon->SetTexture(Name, FileName);

    mIcon->SetOpacity(1.f);
}

void CSlot::SetTextureIcon(CTexture* Texture)
{
    mIcon->SetTexture(Texture);

    if (Texture)
        mIcon->SetOpacity(1.f);

    else
        mIcon->SetOpacity(0.f);
}

void CSlot::SetTextureInfo(const std::string& Name)
{
    mInfoBack->SetTexture(Name);

    mInfoBack->SetOpacity(1.f);
}

void CSlot::SetTextureInfo(const std::string& Name, 
    const TCHAR* FileName)
{
    mInfoBack->SetTexture(Name, FileName);

    mInfoBack->SetOpacity(1.f);
}

void CSlot::SetTextureInfo(CTexture* Texture)
{
    mInfoBack->SetTexture(Texture);

    if (Texture)
        mInfoBack->SetOpacity(1.f);

    else
        mInfoBack->SetOpacity(0.f);
}

void CSlot::SetTintInfo(float r, float g, float b)
{
    mInfoBack->SetTint(r, g, b);
}

void CSlot::SetOpacityInfo(float Opacity)
{
    mInfoBack->SetOpacity(Opacity);
}

void CSlot::SetInfoText(const TCHAR* Text)
{
    mInfoText->SetText(Text);
}

void CSlot::SetTintIcon(float r, float g, float b)
{
    mIcon->SetTint(r, g, b);
}

void CSlot::SetOpacityIcon(float Opacity)
{
    mIcon->SetOpacity(Opacity);
}

void CSlot::SetSelected(bool Select)
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

void CSlot::SetEnableInfoTag(bool Flag)
{
    mInfoBack->SetEnable(Flag);
    mInfoText->SetEnable(Flag);
}

void CSlot::SetSize(const FVector2D& Size)
{
    CUserWidget::SetSize(Size);

    mBack->SetSize(Size);
    mIcon->SetSize(Size - 8.f);
}

void CSlot::SetSize(float x, float y)
{
    CUserWidget::SetSize(x, y);

    mBack->SetSize(x, y);
    mIcon->SetSize(x - 8.f, y - 8.f);
}

void CSlot::SetIconSize(const FVector2D& Size)
{
    mIcon->SetSize(Size);
}

void CSlot::SetIconSize(float x, float y)
{
    mIcon->SetSize(x, y);
}

void CSlot::SetPos(const FVector2D& Pos)
{
    CUserWidget::SetPos(Pos);

    mIcon->SetPos(4.f, 4.f);
}

void CSlot::SetPos(float x, float y)
{
    CUserWidget::SetPos(x, y);

    mIcon->SetPos(4.f, 4.f);
}

void CSlot::SetIconPos(const FVector2D& Pos)
{
    mIcon->SetPos(Pos);
}

void CSlot::SetIconPos(float x, float y)
{
    mIcon->SetPos(x, y);
}

void CSlot::SetInfoTextBoxSize(const FVector2D& Size)
{
    mInfoBack->SetSize(Size);
    mInfoText->SetSize(Size);
}

void CSlot::SetInfoTextBoxSize(float x, float y)
{
    mInfoBack->SetSize(x, y);
    mInfoText->SetSize(x, y);
}

void CSlot::SetTextPos(const FVector2D& Pos)
{
    mInfoBack->SetPos(Pos);
    mInfoText->SetPos(Pos);
}

void CSlot::SetTextPos(float x, float y)
{
    mInfoBack->SetPos(x, y);
    mInfoText->SetPos(x, y);
}

#pragma endregion

bool CSlot::Init()
{
    CUserWidget::Init();

    mBack = mScene->GetUIManager()->CreateWidget<CImage>("Back");
    mBack->SetTexture("SlotBack", TEXT("Texture\\UI\\SlotBack.png"));

    AddWidget(mBack);

    mIcon = mScene->GetUIManager()->CreateWidget<CImage>("Icon");
    mIcon->SetSize(10.f, 10.f);
    mIcon->SetOpacity(0.f);

    AddWidget(mIcon);

    mInfoBack = mScene->GetUIManager()->CreateWidget<CImage>("SlotInfoBack");
    mInfoBack->SetSize(70.f, 40.f);
    mInfoBack->SetEnable(false);

    AddWidget(mInfoBack);

    mInfoText = mScene->GetUIManager()->CreateWidget<CTextBlock>("SlotInfoText");
    mInfoText->SetFontSize(20.f);
    mInfoText->SetTextColor(0, 0, 0, 255);
    mInfoText->SetSize(70.f, 40.f);
    mInfoText->SetAlignH(ETextAlignH::Center);
    mInfoText->SetEnable(false);
    AddWidget(mInfoText);

  
    return true;
}
void CSlot::Update(float DeltaTime)
{
    CUserWidget::Update(DeltaTime);


    //CLog::Debug("MouseOn : ", mMouseOn ? "True" : "False");
    if (mScene->GetInput()->GetMouseDown(EMouseButtonType::LButton) &&
        mMouseOn)
    {
        if (mClickCallback)
            mClickCallback();
    }
}
void CSlot::Render()
{
    CUserWidget::Render();
}

bool CSlot::CollisionMouse(CWidget** Result, const FVector2D& MousePos)
{
    return CWidget::CollisionMouse(Result, MousePos);
}
