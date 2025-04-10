#pragma once

#include "../GameInfo.h"

namespace ENodeDir
{
	enum Type
	{
		T,
		RT,
		R,
		RB,
		B,
		LB,
		L,
		LT,
		End
	};
}

enum class ENavNodeType
{
	None,
	Open,
	Close
};

struct FCostCmp
{
	bool operator()(struct FNavNode* Src, struct FNavNode* Dest);
};

struct FNavNode
{
	FNavNode* Parent = nullptr;
	ENavNodeType	NodeType = ENavNodeType::None;
	FVector2D		Pos;
	FVector2D		Size;
	FVector2D		Center;
	int				IndexX = 0;
	int				IndexY = 0;
	int				Index = 0;
	float			Dist = FLT_MAX;
	float			Huristic = FLT_MAX;
	float			Total = FLT_MAX;
	std::list<ENodeDir::Type>	SearchDirList;
};

class CNavigation
{
	friend class CScene;

private:
	CNavigation();
	~CNavigation();

private:
	class CScene* mScene = nullptr;
	CSharedPtr<class CTileMapComponent>	mTileMap;
	ETileShape		mShape = ETileShape::Rect;
	std::vector<FNavNode*>	mNodeList;
	int		mCountX = 0;
	int		mCountY = 0;
	FVector2D	mTileSize;
	float	mDiagonalCost = 0.f;

	std::vector<FNavNode*>	mOpenList;
	std::vector<FNavNode*>	mUseList;

public:
	void SetTileMap(class CTileMapComponent* TileMap);

public:
	bool Init();
	bool FindPath(const FVector2D& Start,
		const FVector2D& End, 
		std::list<FVector2D>& PathList);

private:
	bool FindNode(FNavNode* Node, FNavNode* EndNode,
		const FVector2D& End,
		std::list<FVector2D>& PathList);
	void AddDir(ENodeDir::Type Dir, FNavNode* Node);
	FNavNode* GetCorner(ENodeDir::Type Dir,
		FNavNode* Node, FNavNode* EndNode);
	FNavNode* GetCornerRectTop(FNavNode* Node,
		FNavNode* EndNode);
	FNavNode* GetCornerRectBottom(FNavNode* Node,
		FNavNode* EndNode);
	FNavNode* GetCornerRectLeft(FNavNode* Node,
		FNavNode* EndNode);
	FNavNode* GetCornerRectRight(FNavNode* Node,
		FNavNode* EndNode);
	FNavNode* GetCornerRectRightTop(FNavNode* Node,
		FNavNode* EndNode);
	FNavNode* GetCornerRectRightBottom(FNavNode* Node,
		FNavNode* EndNode);
	FNavNode* GetCornerRectLeftTop(FNavNode* Node,
		FNavNode* EndNode);
	FNavNode* GetCornerRectLeftBottom(FNavNode* Node,
		FNavNode* EndNode);



private:
	static bool SortOpenList(FNavNode* Src, FNavNode* Dest);
};

