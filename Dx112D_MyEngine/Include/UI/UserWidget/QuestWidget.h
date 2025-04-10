#pragma once
#include "WindowWidget.h"

class CQuestWidget :
    public CWindowWidget
{
	friend class CSceneUIManager;

protected:
	CQuestWidget();
	virtual ~CQuestWidget();

protected:
	// 퀘스트 UI BackGround이미지
	CSharedPtr<class CImage>		mQuestBack;
	// 퀘스트 UI 제목 텍스트
	CSharedPtr<class CTextBlock>	mQuestText;
	// 오른쪽에 표시될 퀘스트 정보들(퀘스트 제목, 설명)
	CSharedPtr<class CQuestInfo>	mQuestInfo;
	// 왼쪽에 표시될 퀘스트 목록들
	std::vector<CSharedPtr<class CQuestTag>>	mQuestList;
	
	// 현재 선택된 퀘스트
	class CQuest*	mSelectedQuest = nullptr;
	// 현재 선택된 퀘스트 슬롯 Index
	int				mSelectedSlotIndex = -1;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();

public:
	// 현재 선택된 퀘스트를 반환한다.
	CQuest* GetSelectedQuest() const { return mSelectedQuest; }

public:
	// 수락버튼 비활성화 함수
	void SetQuestEnableBtn(bool Flag);
	void SetQuestTagEnable(bool Flag);
	// 선택된 퀘스트 세팅 함수
	void SetSelectedQuest(class CQuest* Quest) { mSelectedQuest = Quest; }
	// 퀘스트 슬롯 초기화 함수
	void ClearQuestSlots();

public:
	// 수락 버튼 활성화 체크 함수
	bool IsEnableAcceptedBtn();

public:
	// 퀘스트 UI를 활성화 시킬 때 호출해주는 함수
	void OpenQuestUI(const std::list<class CQuest*>& QuestList);
	// 퀘스트 태그를 클릭했을 때 호출될 콜백 함수
	void SelectQuest(int Index);
};

