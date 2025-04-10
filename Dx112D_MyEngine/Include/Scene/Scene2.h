#pragma once
#include "Scene.h"
class CScene2 :
    public CScene
{
	friend class CSceneManager;

private:
	CScene2();
	virtual ~CScene2();

	CSharedPtr<class CCameraObject> mCameraObj;
protected:
	virtual bool InitAsset();
	virtual bool InitObject();
	virtual bool InitWidget();

private:
	void LoadMap();
};

