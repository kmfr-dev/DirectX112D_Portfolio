#pragma once
#include "UserWidget.h"

class CQuestInfo :
    public CUserWidget
{
	friend class CQuestWidget;
	friend class CSceneUIManager;

protected:
	CQuestInfo();
	virtual ~CQuestInfo();

protected:
	// 퀘스트 정보 Background 이미지
	CSharedPtr<class CImage>	mBack;
	// 퀘스트 제목
	CSharedPtr<class CTextBlock> mQuestTitle;
	// 퀘스트 설명
	CSharedPtr<class CTextBlock> mQuestDesc;

	// 퀘스트 진행도
	CSharedPtr<class CQuestTag> mQuestTag;

	// 퀘스트 수락 버튼
	CSharedPtr<class CButton> mAcceptedBtn;
	// 퀘스트 수락버튼에 담길 텍스트
	CSharedPtr<class CTextBlock> mBtnText;
	

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();

public:
	void SetEnableBtn(bool Flag);
	void SetEnableProgress(bool Flag);
	void SetQuestProgress(int CurrentCount, int TargetCount);

public:
	// 퀘스트 정보를 바탕으로 UI 설정하는 함수
	void SetQuestInfo(class CQuest* Quest);
};