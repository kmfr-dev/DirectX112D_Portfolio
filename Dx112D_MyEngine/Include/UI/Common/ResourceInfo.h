#pragma once
#include "../UserWidget/UserWidget.h"

class CResourceInfo :
    public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CResourceInfo();
	virtual ~CResourceInfo();

protected:
	CSharedPtr<class CImage>			mBack;
	CSharedPtr<class CImage>			mIcon;
	CSharedPtr<class CTextBlock>		mText;
	CSharedPtr<class CTextBlock>		mInfoText;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();
	virtual void Render(const FVector3D& Pos);

public:
	void		SetBackTexture(const std::string& Name,
		const TCHAR* FileName);
	void		SetIconTexture(const std::string& Name,
		const TCHAR* FileName);
	void		SetText(const TCHAR* Name);
	void		SetInfoText(const TCHAR* Name);

public:
	virtual void SetBackSize(const FVector2D& Size);
	virtual void SetBackSize(float x, float y);
 
	virtual void SetIconSize(const FVector2D& Size);
	virtual void SetIconSize(float x, float y);

	virtual void SetTextSize(float Size);
	virtual void SetTextBoxSize(const FVector2D& Size);
	virtual void SetTextBoxSize(float x, float y);

	virtual void SetInfoTextSize(float Size);
	virtual void SetInfoTextBoxSize(const FVector2D& Size);
	virtual void SetInfoTextBoxSize(float x, float y);

public:
	virtual void SetBackPos(const FVector2D& Pos);
	virtual void SetBackPos(float x, float y);
	
	virtual void SetIconPos(const FVector2D& Pos);
	virtual void SetIconPos(float x, float y);

	virtual void SetTextPos(const FVector2D& Pos);
	virtual void SetTextPos(float x, float y);
	virtual void SetInfoTextPos(const FVector2D& Pos);
	virtual void SetInfoTextPos(float x, float y);
};

