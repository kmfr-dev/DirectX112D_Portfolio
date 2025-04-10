#pragma once

#include "Scene.h"

class CSceneMain :
	public CScene
{
	friend class CSceneManager;

private:
	CSceneMain();
	virtual ~CSceneMain();

	CSharedPtr<class CCameraObject> mCameraObj;

protected:
	virtual bool InitAsset();
	virtual bool InitObject();
	virtual bool InitWidget();
	
	virtual void LoadMapObjects() override;

private:
	void LoadMap();
};

