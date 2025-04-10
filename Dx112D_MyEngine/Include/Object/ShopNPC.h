#pragma once
#include "NPCObject.h"
class CShopNPC :
    public CNPCObject
{
    friend class CScene;

protected:
    CShopNPC();
    CShopNPC(const CShopNPC& Obj);
    CShopNPC(CShopNPC&& Obj);
    virtual ~CShopNPC();

private:
    // 상점 기능을하는 컴포넌트
    CSharedPtr<class CStoreComponent> mStoreComponent;
    // 아이템 목록을 들고있는다
    std::vector<FItemData*> mItemList;

public:
    virtual bool    Init();
    virtual void    Update(float DeltaTime);
    virtual void    Render();

public:
    class CStoreComponent* GetStoreComponent() const { return mStoreComponent; }

};

