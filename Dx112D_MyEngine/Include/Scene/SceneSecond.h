#pragma once
#include "Scene.h"

class CSceneSecond :
    public CScene
{
	friend class CSceneManager;

private:
	CSceneSecond();
	virtual ~CSceneSecond();

	CSharedPtr<class CTileMapObj>	mTileMapObj;

protected:
	virtual bool InitAsset();
	virtual bool InitObject();
	virtual bool InitWidget();
};

