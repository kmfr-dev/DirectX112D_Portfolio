#pragma once

#include "SceneComponent.h"

enum class eCollisionType
{
	Block,
	Overlapped,
	Release,
	End
};

class CColliderBase abstract :
	public CSceneComponent
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CColliderBase();
	CColliderBase(const CColliderBase& Com);
	CColliderBase(CColliderBase&& Com);
	virtual ~CColliderBase();

protected:
	//std::unordered_map<CColliderBase*, bool>	mCollisionObjects;		// 나와 충돌한 충돌체들을 담아둘 unordered_map
	std::unordered_map<CColliderBase*, eCollisionType>	mCollisionObjects;		// 나와 충돌한 충돌체들을 담아둘 unordered_map


	EColliderType	mColliderType;				// 충돌 타입 (2D, 3D)
	EColliderShape	mColliderShape;				// 충돌체 도형 (AABB, 구, OBB, 선)
	FVector3D		mMin;						// LB 좌표
	FVector3D		mMax;						// RT 좌표
	FCollisionProfile* mProfile = nullptr;		// 충돌 프로파일
	eCollisionType mCollision = eCollisionType::End; //충돌 상태 확인
	//bool			mCollision = false;			// 충돌여부
	std::function<void(const FVector3D&, CColliderBase*)>	mCollisionBeginFunc;	// 충돌시 호출 될 함수 포인터
	class CSceneObject* mBeginObj;
	std::function<void(CColliderBase*)>	mCollisionEndFunc;	// 충돌을 벗어날때 호출 될 함수 포인터
	class CSceneObject* mEndObj;
	std::function<void(CColliderBase*)> mCollisionOverlapFunc;
	class CSceneObject* mOverlapObj;

#ifdef _DEBUG

	class CColliderCBuffer* mCBuffer = nullptr;
	CSharedPtr<class CMesh> mMesh;
	CSharedPtr<class CShader> mShader;
	bool		mEnableRotation = false;

#endif // _DEBUG


public:
	FCollisionProfile* GetProfile()	const
	{
		return mProfile;
	}

	EColliderShape GetColliderShape()	const
	{
		return mColliderShape;
	}

	EColliderType GetColliderType()	const
	{
		return mColliderType;
	}

	const FVector3D& GetMin()
	{
		return mMin;
	}

	const FVector3D& GetMax()
	{
		return mMax;
	}

public:
	void SetCollisionProfile(const std::string& Name);
	void CallCollisionBegin(const FVector3D& HitPoint,
		CColliderBase* Dest);
	void CallCollisionEnd(CColliderBase* Dest);
	void CallCollisionOverlap(CColliderBase* Dest);
	bool CheckCollisionObject(CColliderBase* Collider);
	void AddCollisionObject(CColliderBase* Collider);
	void ColliionObjectOverlap(CColliderBase* Collider);
	void EraseCollisionObject(CColliderBase* Collider);

public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
	virtual CColliderBase* Clone();
	virtual void EraseOwner();
	virtual bool Collision(FVector3D& HitPoint, CColliderBase* Dest) = 0;


public:
	template <typename T>
	void SetCollisionBeginFunc(T* Obj,
		void (T::* Func)(const FVector3D&, CColliderBase*))
	{
		mBeginObj = Obj;
		mCollisionBeginFunc = std::bind(Func, Obj,
			std::placeholders::_1, std::placeholders::_2);
	}

	void ClearBeginFunction()
	{
		mCollisionBeginFunc = nullptr;
	}

	template <typename T>
	void SetCollisionEndFunc(T* Obj,
		void (T::* Func)(CColliderBase*))
	{
		mEndObj = Obj;
		mCollisionEndFunc = std::bind(Func, Obj,
			std::placeholders::_1);
	}

	template <typename T>
	void SetCollisionOverlapFunc(T* Obj,
		void (T::* Func)(CColliderBase*))
	{
		mOverlapObj = Obj;
		mCollisionOverlapFunc = std::bind(Func, Obj,
			std::placeholders::_1);
	}

	void ClearEndFunction()
	{
		mCollisionEndFunc = nullptr;
	}

	virtual void SetEnable(bool Enable);
	virtual void Refresh();
};

