#pragma once
#include "../UserWidget/UserWidget.h"
class CRewardInfo :
    public CUserWidget
{
	friend class CQuestWidget;
	friend class CSceneUIManager;

protected:
	CRewardInfo();
	virtual ~CRewardInfo();

protected:
	// 보상 위젯 Back 이미지
	CSharedPtr<class CImage>	 mBack;
	// 보상 위젯 Title
	CSharedPtr<class CTextBlock> mRewardTitle;
	// 보상 슬롯 (Icon 출력용)
	CSharedPtr<class CSlot>     mRewardSlot;
	// 보상 내역 Text
	CSharedPtr<class CTextBlock> mRewardDesc;

	class CQuest* mRewardQuest = nullptr;

	bool	mRewardedGold = false;
	bool	mRewardedItem = false;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();

public:
	void SetRewardQuest(class CQuest* Quest) { mRewardQuest = Quest; }
	void SetRewardText(const TCHAR* Text);
	void SetTextureIcon(class CTexture* Texture);
	void SetTextureIcon(const std::string& Name);
	void SetTextureIcon(const std::string& Name, const TCHAR* FileName);

public:
	void OpenRewardUI(class CQuest* Quest);
	void ShowNextReward();
};

