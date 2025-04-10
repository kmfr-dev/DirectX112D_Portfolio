#pragma once
#include "Scene.h"

class CSceneBoss :
    public CScene
{
	friend class CSceneManager;

private:
	CSceneBoss();
	virtual ~CSceneBoss();
	CSharedPtr<class CPlayerObject> mPlayer;
	CSharedPtr<class CNueBossMonster> mBoss;
	CSharedPtr<class CCameraObject> mCameraObj;
	CSharedPtr<class CMainWidget> mWidget;

	// ������ �����ƴ��� �Ǵ��� ����
	bool		mCreate = false;
	
	// ���� �ƾ� ���� ���θ� �Ǵ��ϴ� 
	// ������ Ÿ�̸�
	bool		mCutScene = false;
	float		mCutSceneTimer = 0.f;

protected:
	virtual bool InitAsset() override;
	virtual bool InitObject() override;
	virtual bool InitWidget() override;

	virtual void Update(float DeltaTime) override;

private:
	void LoadMap();
};

