#pragma once

#include "UserWidget.h"

enum class ESlotType
{
	None,
	Inventory,
	Store,

};

class CSlot :
    public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CSlot();
	virtual ~CSlot();

protected:
	// 슬롯 타입
	ESlotType mSlotType = ESlotType::None;
	// 슬롯 Back 이미지
	CSharedPtr<class CImage>	mBack;
	// 슬롯에 들어갈 아이템 Icon 이미지
	CSharedPtr<class CImage>	mIcon;
	// 아이템 갯수 텍스트 뒤에 출력될 이미지
	CSharedPtr<class CImage>	mInfoBack;
	// 아이템 갯수를 표시해줄 텍스트
	CSharedPtr<class CTextBlock> mInfoText;
	// 슬롯 셀렉션 이미지

	// 슬롯 인덱스
	int				mIndex = 0;

	// 아이템 데이터
	FItemData*		mItemData = nullptr;

	// 슬롯을 클릭했을 때 호출될 Callback
	std::function<void()> mClickCallback;

public:
	
	void SetItemData(FItemData* Data);
	FItemData* GetItemData() { return mItemData; }

	void SetSlotType(ESlotType Type)
	{
		mSlotType = Type;
	}

	void SetIndex(int Index)
	{
		mIndex = Index;
	}
	void SetTextureBack(const std::string& Name);
	void SetTextureBack(const std::string& Name,
		const TCHAR* FileName);
	void SetTextureBack(class CTexture* Texture);
	void SetTintBack(float r, float g, float b);
	void SetOpacityBack(float Opacity);
	void SetTextureIcon(const std::string& Name);
	void SetTextureIcon(const std::string& Name,
		const TCHAR* FileName);
	void SetTextureIcon(class CTexture* Texture);

	void SetTextureInfo(const std::string& Name);
	void SetTextureInfo(const std::string& Name,
		const TCHAR* FileName);
	void SetTextureInfo(class CTexture* Texture);
	void SetTintInfo(float r, float g, float b);
	void SetOpacityInfo(float Opacity);

	void SetInfoText(const TCHAR* Text);

	void SetTintIcon(float r, float g, float b);
	void SetOpacityIcon(float Opacity);

	void SetSelected(bool Select);

	void SetEnableInfoTag(bool Flag);
public:
	virtual void SetSize(const FVector2D& Size);
	virtual void SetSize(float x, float y);
	virtual void SetIconSize(const FVector2D& Size);
	virtual void SetIconSize(float x, float y);
	virtual void SetPos(const FVector2D& Pos);
	virtual void SetPos(float x, float y);

	virtual void SetIconPos(const FVector2D& Pos);
	virtual void SetIconPos(float x, float y);

	virtual void SetInfoTextBoxSize(const FVector2D& Size);
	virtual void SetInfoTextBoxSize(float x, float y);

	virtual void SetTextPos(const FVector2D& Pos);
	virtual void SetTextPos(float x, float y);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();

	void SetClickCallback(const std::function<void()>& Callback)
	{
		mClickCallback = Callback;
	}

	virtual bool CollisionMouse(CWidget** Result, const FVector2D& MousePos);
};

