#pragma once
#include "../UserWidget/UserWidget.h"

class CQuestTag :
    public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CQuestTag();
	virtual ~CQuestTag();
	
protected:
	CSharedPtr<class CImage>			mBack;
	CSharedPtr<class CTextBlock>		mNameText;

	// 퀘스트 목록 인덱스
	int		mIndex = 0;
	
	// 퀘스트 슬롯 마다 들고 있을 포인터
	class CQuest* mQuest = nullptr;

	// 퀘스트 목록 클릭했을 때 호출할 콜백
	std::function<void()> mClickCallback;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
	virtual void Render() override;

public:
	class CQuest* GetQuest() const { return mQuest; }

public:
	virtual void SetNameTag(const TCHAR* Name);
	void SetClickCallback(const std::function<void()>& Callback)
	{
		mClickCallback = Callback;
	}

	void			SetSize(const FVector2D& Size);
	void			SetSize(float x, float y);
	
	void			SetFontSize(float Size);
	void			SetText(const TCHAR* Text);

	void			SetQuest(class CQuest* Quest) { mQuest = Quest; }
	void			SetIndex(int Index) { mIndex = Index; }

	void			SetSelected(bool Select);
	void			SetTint(float r, float g, float b);

	virtual bool CollisionMouse(CWidget** Result, const FVector2D& MousePos);
};

