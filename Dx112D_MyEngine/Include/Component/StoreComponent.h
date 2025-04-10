#pragma once
#include "Component.h"
class CStoreComponent :
    public CComponent
{
    friend class CScene;
    friend class CSceneObject;

protected:
    CStoreComponent();
    virtual ~CStoreComponent();

private:
    std::vector<FItemData*> mStoreItems;
    bool        mStoreOpened = false;

public:
    virtual bool Init() override;

    // ���� ����
    void    OpenStore(class CPlayerObject* Player);
    // ���� �ݱ�
    void    CloseStore();

    // ������ ������ ��� ����
    void    SetItems(const std::vector<FItemData*>& Items);
    void    BuyItem(FItemData* Item);
    
};

