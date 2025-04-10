#pragma once

#include "Component.h"

class CInventoryComponent :
    public CComponent
{
	friend class CScene;
	friend class CSceneObject;
	friend class CInventory;

protected:
	CInventoryComponent();
	CInventoryComponent(const CInventoryComponent& Com);
	CInventoryComponent(CInventoryComponent&& Com);
	virtual ~CInventoryComponent();

protected:
	// 아이템 목록
	std::vector<FItemData*>	mItemList;
	std::function<void(const FItemData*, int)>	mChangeFunction;

public:
	// 아이템 목록 Get함수
	const std::vector<FItemData*>& GetItemList() const { return mItemList; }
	// 아이템을 아이템 목록에 추가하는 함수
	void AddItem(FItemData* NewItem);
	// 아이템을 아이템 목록에서 제거하는 함수
	void RemoveItem(int Index);
	// 아이템 사용하는 함수
	void UseItem(FItemData* Item);


public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CInventoryComponent* Clone();
	virtual void EndFrame();

public:
	template <typename T>
	void SetChangeFunction(T* Obj, 
		void(T::* Func)(const FItemData*, int))
	{
		mChangeFunction = std::bind(Func, Obj,
			std::placeholders::_1, std::placeholders::_2);
	}
};

