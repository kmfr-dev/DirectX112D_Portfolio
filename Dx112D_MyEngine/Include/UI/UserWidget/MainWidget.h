#pragma once

#include "UserWidget.h"

class CMainWidget :
    public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CMainWidget();
	virtual ~CMainWidget();

protected:
	// 플레이어, 보스
	CSharedPtr<class CPlayerObject> mPlayer;
	CSharedPtr<class CNueBossMonster> mBoss;
	// 플레이어 HPBar UI
	CSharedPtr<class CImage>		mHPBarSymbol;
	CSharedPtr<class CProgressBar>	mHPBar;
	// 보스 HPBar UI
	CSharedPtr<class CHeadInfo>		mBossHPUI;
	// 퀵슬롯 UI
	CSharedPtr<class CQuickSlotList> mQuickSlots;
	// 인벤토리 UI
	CSharedPtr<class CInventory>	mInventory;
	// 대화 UI
	CSharedPtr<class CDialogueWidget> mDialogueUI;
	// 상점 UI
	CSharedPtr<class CStoreWidget>	mStore;
	// 퀘스트 UI
	CSharedPtr<class CQuestWidget> mQuestUI;
	// 보상 UI
	CSharedPtr<class CRewardInfo> mRewardUI;

	bool mTalking = false;
	int mCurrnetNPCID = -1;
	int mCurrentScriptID = -1;


public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();


public:
	virtual void SetPlayerHPPerCent();
	virtual void SetBossHPPerCent();

	void		 SetEnableBossUI(bool Enable);
	void		 SetEnableDialogueUI(bool Enable);
	void		 SetDialgoueText(const TCHAR* Text);
	void		 SetDialgoueNameTag(const TCHAR* Name);

	void		ShowScript(const class FScriptLine* Line);
	void		ShowScript(int NPCID, int ScriptID);

public:
	class CQuickSlotList* GetQuickSlot() const;
	class CInventory* GetInventoryUI() const;
	class CStoreWidget* GetStoreUI() const;
	class CQuestWidget* GetQuestUI() const;
	class CRewardInfo* GetRewardUI() const;
	class CDialogueWidget* GetDialogueUI() const;

public:
	bool IsAnyUIOpened() const;

};

