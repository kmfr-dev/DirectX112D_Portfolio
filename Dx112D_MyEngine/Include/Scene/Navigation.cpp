#include "Navigation.h"
#include "../Component/TileMapComponent.h"

bool FCostCmp::operator()(FNavNode* Src, FNavNode* Dest)
{
    return Src->Total < Dest->Total;
}

CNavigation::CNavigation()
{
}

CNavigation::~CNavigation()
{
    size_t  Size = mNodeList.size();

    for (size_t i = 0; i < Size; ++i)
    {
        SAFE_DELETE(mNodeList[i]);
    }
}

void CNavigation::SetTileMap(CTileMapComponent* TileMap)
{
    mTileMap = TileMap;

    mShape = mTileMap->GetTileShape();
    mCountX = mTileMap->GetTileCountX();
    mCountY = mTileMap->GetTileCountY();
    mTileSize = mTileMap->GetTileSize();
    mDiagonalCost = mTileSize.Length();

    int Count = mCountX * mCountY;

    mUseList.reserve(Count);
    mOpenList.reserve(Count);

    for (int i = 0; i < Count; ++i)
    {
        FNavNode* Node = new FNavNode;

        const CTile* Tile = mTileMap->GetTile(i);

        Node->Pos = mTileMap->GetTilePos(i);
        Node->Size = mTileSize;
        Node->Center = mTileMap->GetTileCenter(i);
        Node->IndexX = i % mCountX;
        Node->IndexY = i / mCountX;
        Node->Index = i;

        mNodeList.emplace_back(Node);
    }
}

bool CNavigation::Init()
{
    return true;
}

bool CNavigation::FindPath(const FVector2D& Start,
    const FVector2D& End, 
    std::list<FVector2D>& PathList)
{
    if (!mTileMap)
    {   
        PathList.clear();
        PathList.emplace_back(End);
        return false;
    }

    int StartIndex = mTileMap->GetTileIndex(Start);
    if (StartIndex == -1)
        return false;

    int EndIndex = mTileMap->GetTileIndex(End);
    if (EndIndex == -1)
        return false;

    if (mTileMap->GetTileType(EndIndex) == 
        ETileType::UnableToMove)
        return false;

    PathList.clear();

    // 기존에 사용했던 정보를 초기화해준다.
    size_t  Size = mUseList.size();

    for (size_t i = 0; i < Size; ++i)
    {
        mUseList[i]->NodeType = ENavNodeType::None;
        mUseList[i]->Dist = FLT_MAX;
        mUseList[i]->Huristic = FLT_MAX;
        mUseList[i]->Total = FLT_MAX;
        mUseList[i]->Parent = nullptr;
        mUseList[i]->SearchDirList.clear();
    }

    mUseList.clear();

    FNavNode* StartNode = mNodeList[StartIndex];
    FNavNode* EndNode = mNodeList[EndIndex];

    if (StartNode == EndNode)
    {
        PathList.emplace_back(End);
        return true;
    }

    StartNode->NodeType = ENavNodeType::Open;
    StartNode->Dist = 0.f;
    StartNode->Huristic = StartNode->Center.Distance(End);
    StartNode->Total = StartNode->Huristic;

    for (int i = 0; i < ENodeDir::End; ++i)
    {
        StartNode->SearchDirList.emplace_back((ENodeDir::Type)i);
    }

    // 시작노드를 열린목록에 추가한다.
    mOpenList.emplace_back(StartNode);

    mUseList.emplace_back(StartNode);

    while (!mOpenList.empty())
    {
        // 비용에 따라 내림차순 정렬을 해두게 되면
        // 가장 뒤의 노드가 최소 비용을 가지기 때문에
        // 뒤의 노드를 얻어오고 뒤의 노드를 제거한다.
        FNavNode* Node = mOpenList.back();
        mOpenList.pop_back();

        // 사용된 노드는 닫힌노드로 변경한다.
        Node->NodeType = ENavNodeType::Close;

        // SearchDirList 에 탐색할 방향이 들어가있다.
        // 해당 방향으로 탐색하여 노드를 찾아낸다.
        if (FindNode(Node, EndNode, End, PathList))
        {
            break;
        }

        // 열린목록을 정렬한다.
        if (mOpenList.size() >= 2)
        {
            std::sort(mOpenList.begin(), 
                mOpenList.end(),
                CNavigation::SortOpenList);
        }
    }
    
    mOpenList.clear();

    return !PathList.empty();
}

bool CNavigation::FindNode(FNavNode* Node,
    FNavNode* EndNode, const FVector2D& End, 
    std::list<FVector2D>& PathList)
{
    auto    iter = Node->SearchDirList.begin();
    auto    iterEnd = Node->SearchDirList.end();

    for (; iter != iterEnd; ++iter)
    {
        FNavNode* Corner = GetCorner(*iter, Node,
            EndNode);

        if (!Corner)
            continue;

        if (Corner == EndNode)
        {
            mUseList.emplace_back(Corner);
            PathList.emplace_back(End);

            FNavNode* PathNode = Node;

            while (PathNode)
            {
                PathList.push_front(PathNode->Center);
                PathNode = PathNode->Parent;
            }

            // 시작노드의 노드 위치는 제거한다.
            PathList.pop_front();

            return true;
        }

        float   Dist = 0.f;

        switch (*iter)
        {
        case ENodeDir::T:
        case ENodeDir::B:
            Dist = Node->Dist + Node->Size.y;
            break;
        case ENodeDir::R:
        case ENodeDir::L:
            Dist = Node->Dist + Node->Size.x;
            break;
        case ENodeDir::RT:
        case ENodeDir::RB:
        case ENodeDir::LB:
        case ENodeDir::LT:
            Dist = Node->Dist + mDiagonalCost;
            break;
        }

        if (Corner->NodeType == ENavNodeType::Open)
        {
            if (Corner->Dist > Dist)
            {
                Corner->Dist = Dist;
                Corner->Total = Corner->Dist + Corner->Huristic;
                Corner->Parent = Node;

                AddDir(*iter, Corner);
            }
        }

        else
        {
            Corner->NodeType = ENavNodeType::Open;
            Corner->Parent = Node;
            Corner->Dist = Dist;
            Corner->Huristic = Corner->Center.Distance(End);
            Corner->Total = Dist + Corner->Huristic;

            mOpenList.emplace_back(Corner);

            mUseList.emplace_back(Corner);

            AddDir(*iter, Corner);
        }
    }

    return false;
}

void CNavigation::AddDir(ENodeDir::Type Dir,
    FNavNode* Node)
{
    Node->SearchDirList.clear();

    switch (mShape)
    {
    case ETileShape::Rect:
        switch (Dir)
        {
        case ENodeDir::T:
            Node->SearchDirList.emplace_back(ENodeDir::T);
            Node->SearchDirList.emplace_back(ENodeDir::RT);
            Node->SearchDirList.emplace_back(ENodeDir::LT);
            break;
        case ENodeDir::RT:
            Node->SearchDirList.emplace_back(ENodeDir::RT);
            Node->SearchDirList.emplace_back(ENodeDir::T);
            Node->SearchDirList.emplace_back(ENodeDir::R);
            Node->SearchDirList.emplace_back(ENodeDir::LT);
            Node->SearchDirList.emplace_back(ENodeDir::RB);
            break;
        case ENodeDir::R:
            Node->SearchDirList.emplace_back(ENodeDir::R);
            Node->SearchDirList.emplace_back(ENodeDir::RT);
            Node->SearchDirList.emplace_back(ENodeDir::RB);
            break;
        case ENodeDir::RB:
            Node->SearchDirList.emplace_back(ENodeDir::RB);
            Node->SearchDirList.emplace_back(ENodeDir::B);
            Node->SearchDirList.emplace_back(ENodeDir::R);
            Node->SearchDirList.emplace_back(ENodeDir::RT);
            Node->SearchDirList.emplace_back(ENodeDir::LB);
            break;
        case ENodeDir::B:
            Node->SearchDirList.emplace_back(ENodeDir::B);
            Node->SearchDirList.emplace_back(ENodeDir::LB);
            Node->SearchDirList.emplace_back(ENodeDir::RB);
            break;
        case ENodeDir::LB:
            Node->SearchDirList.emplace_back(ENodeDir::LB);
            Node->SearchDirList.emplace_back(ENodeDir::B);
            Node->SearchDirList.emplace_back(ENodeDir::L);
            Node->SearchDirList.emplace_back(ENodeDir::LT);
            Node->SearchDirList.emplace_back(ENodeDir::RB);
            break;
        case ENodeDir::L:
            Node->SearchDirList.emplace_back(ENodeDir::L);
            Node->SearchDirList.emplace_back(ENodeDir::LT);
            Node->SearchDirList.emplace_back(ENodeDir::LB);
            break;
        case ENodeDir::LT:
            Node->SearchDirList.emplace_back(ENodeDir::LT);
            Node->SearchDirList.emplace_back(ENodeDir::T);
            Node->SearchDirList.emplace_back(ENodeDir::L);
            Node->SearchDirList.emplace_back(ENodeDir::LB);
            Node->SearchDirList.emplace_back(ENodeDir::RT);
            break;
        }
        break;
    case ETileShape::Isometric:
        break;
    }
}

FNavNode* CNavigation::GetCorner(ENodeDir::Type Dir,
    FNavNode* Node, FNavNode* EndNode)
{
    switch (mShape)
    {
    case ETileShape::Rect:
        switch (Dir)
        {
        case ENodeDir::T:
            return GetCornerRectTop(Node, EndNode);
        case ENodeDir::RT:
            return GetCornerRectRightTop(Node, EndNode);
        case ENodeDir::R:
            return GetCornerRectRight(Node, EndNode);
        case ENodeDir::RB:
            return GetCornerRectRightBottom(Node, EndNode);
        case ENodeDir::B:
            return GetCornerRectBottom(Node, EndNode);
        case ENodeDir::LB:
            return GetCornerRectLeftBottom(Node, EndNode);
        case ENodeDir::L:
            return GetCornerRectLeft(Node, EndNode);
        case ENodeDir::LT:
            return GetCornerRectLeftTop(Node, EndNode);
        }
        break;
    case ETileShape::Isometric:
        switch (Dir)
        {
        case ENodeDir::T:
            break;
        case ENodeDir::RT:
            break;
        case ENodeDir::R:
            break;
        case ENodeDir::RB:
            break;
        case ENodeDir::B:
            break;
        case ENodeDir::LB:
            break;
        case ENodeDir::L:
            break;
        case ENodeDir::LT:
            break;
        }
        break;
    }

    return nullptr;
}

FNavNode* CNavigation::GetCornerRectTop(
    FNavNode* Node, FNavNode* EndNode)
{
    int IndexY = Node->IndexY;

    while (true)
    {
        ++IndexY;

        if (IndexY >= mCountY)
            return nullptr;

        int Index = IndexY * mCountX + Node->IndexX;

        FNavNode* CornerNode = mNodeList[Index];

        if (CornerNode == EndNode)
            return CornerNode;

        // 탐색하는 노드가 닫힌목록에 들어간 노드일경우
        else if (CornerNode->NodeType ==
            ENavNodeType::Close)
            return nullptr;

        else if (mTileMap->GetTileType(Index) ==
            ETileType::UnableToMove)
            return nullptr;

        int CornerX = Node->IndexX + 1;
        int CornerY = IndexY;

        if (CornerX < mCountX && CornerY + 1 < mCountY)
        {
            if (mTileMap->GetTileType(CornerY * mCountX + CornerX) == ETileType::UnableToMove &&
                mTileMap->GetTileType((CornerY + 1) * mCountX + CornerX) == ETileType::Normal)
                return CornerNode;
        }

        CornerX = Node->IndexX - 1;
        CornerY = IndexY;

        if (CornerX >= 0 && CornerY + 1 < mCountY)
        {
            if (mTileMap->GetTileType(CornerY * mCountX + CornerX) == ETileType::UnableToMove &&
                mTileMap->GetTileType((CornerY + 1) * mCountX + CornerX) == ETileType::Normal)
                return CornerNode;
        }
    }

    return nullptr;
}

FNavNode* CNavigation::GetCornerRectBottom(
    FNavNode* Node, FNavNode* EndNode)
{
    int IndexY = Node->IndexY;

    while (true)
    {
        --IndexY;

        if (IndexY < 0)
            return nullptr;

        int Index = IndexY * mCountX + Node->IndexX;

        FNavNode* CornerNode = mNodeList[Index];

        if (CornerNode == EndNode)
            return CornerNode;

        // 탐색하는 노드가 닫힌목록에 들어간 노드일경우
        else if (CornerNode->NodeType ==
            ENavNodeType::Close)
            return nullptr;

        else if (mTileMap->GetTileType(Index) ==
            ETileType::UnableToMove)
            return nullptr;

        int CornerX = Node->IndexX + 1;
        int CornerY = IndexY;

        if (CornerX < mCountX && CornerY - 1 >= 0)
        {
            if (mTileMap->GetTileType(CornerY * mCountX + CornerX) == ETileType::UnableToMove &&
                mTileMap->GetTileType((CornerY - 1) * mCountX + CornerX) == ETileType::Normal)
                return CornerNode;
        }

        CornerX = Node->IndexX - 1;
        CornerY = IndexY;

        if (CornerX >= 0 && CornerY - 1 >= 0)
        {
            if (mTileMap->GetTileType(CornerY * mCountX + CornerX) == ETileType::UnableToMove &&
                mTileMap->GetTileType((CornerY - 1) * mCountX + CornerX) == ETileType::Normal)
                return CornerNode;
        }
    }

    return nullptr;
}

FNavNode* CNavigation::GetCornerRectLeft(
    FNavNode* Node, FNavNode* EndNode)
{
    int IndexX = Node->IndexX;

    while (true)
    {
        --IndexX;

        if (IndexX < 0)
            return nullptr;

        int Index = Node->IndexY * mCountX + IndexX;

        FNavNode* CornerNode = mNodeList[Index];

        if (CornerNode == EndNode)
            return CornerNode;

        // 탐색하는 노드가 닫힌목록에 들어간 노드일경우
        else if (CornerNode->NodeType ==
            ENavNodeType::Close)
            return nullptr;

        else if (mTileMap->GetTileType(Index) ==
            ETileType::UnableToMove)
            return nullptr;

        int CornerX = IndexX;
        int CornerY = Node->IndexY + 1;

        if (CornerX - 1 >= 0 && CornerY < mCountY)
        {
            if (mTileMap->GetTileType(CornerY * mCountX + CornerX) == ETileType::UnableToMove &&
                mTileMap->GetTileType(CornerY * mCountX + CornerX - 1) == ETileType::Normal)
                return CornerNode;
        }

        CornerX = IndexX;
        CornerY = Node->IndexY - 1;

        if (CornerX - 1 >= 0 && CornerY >= 0)
        {
            if (mTileMap->GetTileType(CornerY * mCountX + CornerX) == ETileType::UnableToMove &&
                mTileMap->GetTileType(CornerY * mCountX + CornerX - 1) == ETileType::Normal)
                return CornerNode;
        }
    }

    return nullptr;
}

FNavNode* CNavigation::GetCornerRectRight(
    FNavNode* Node, FNavNode* EndNode)
{
    int IndexX = Node->IndexX;

    while (true)
    {
        ++IndexX;

        if (IndexX >= mCountX)
            return nullptr;

        int Index = Node->IndexY * mCountX + IndexX;

        FNavNode* CornerNode = mNodeList[Index];

        if (CornerNode == EndNode)
            return CornerNode;

        // 탐색하는 노드가 닫힌목록에 들어간 노드일경우
        else if (CornerNode->NodeType ==
            ENavNodeType::Close)
            return nullptr;

        else if (mTileMap->GetTileType(Index) ==
            ETileType::UnableToMove)
            return nullptr;

        int CornerX = IndexX;
        int CornerY = Node->IndexY + 1;

        if (CornerX + 1 < mCountX && CornerY < mCountY)
        {
            if (mTileMap->GetTileType(CornerY * mCountX + CornerX) == ETileType::UnableToMove &&
                mTileMap->GetTileType(CornerY * mCountX + CornerX + 1) == ETileType::Normal)
                return CornerNode;
        }

        CornerX = IndexX;
        CornerY = Node->IndexY - 1;

        if (CornerX + 1 < mCountX && CornerY >= 0)
        {
            if (mTileMap->GetTileType(CornerY * mCountX + CornerX) == ETileType::UnableToMove &&
                mTileMap->GetTileType(CornerY * mCountX + CornerX + 1) == ETileType::Normal)
                return CornerNode;
        }
    }

    return nullptr;
}

FNavNode* CNavigation::GetCornerRectRightTop(
    FNavNode* Node, FNavNode* EndNode)
{
    int IndexY = Node->IndexY;
    int IndexX = Node->IndexX;

    while (true)
    {
        ++IndexX;
        ++IndexY;

        if (IndexX >= mCountX || IndexY >= mCountY)
            return nullptr;

        int Index = IndexY * mCountX + IndexX;

        FNavNode* CornerNode = mNodeList[Index];

        if (CornerNode == EndNode)
            return CornerNode;

        // 탐색하는 노드가 닫힌목록에 들어간 노드일경우
        else if (CornerNode->NodeType ==
            ENavNodeType::Close)
            return nullptr;

        else if (mTileMap->GetTileType(Index) ==
            ETileType::UnableToMove)
            return nullptr;

        int CornerX = IndexX - 1;
        int CornerY = IndexY;

        if (CornerX >= 0 && CornerY + 1 < mCountY)
        {
            if (mTileMap->GetTileType(CornerY * mCountX + CornerX) == ETileType::UnableToMove &&
                mTileMap->GetTileType((CornerY + 1) * mCountX + CornerX) == ETileType::Normal)
                return CornerNode;
        }

        CornerX = IndexX;
        CornerY = IndexY - 1;

        if (CornerX + 1 < mCountX && CornerY >= 0)
        {
            if (mTileMap->GetTileType(CornerY * mCountX + CornerX) == ETileType::UnableToMove &&
                mTileMap->GetTileType(CornerY * mCountX + CornerX + 1) == ETileType::Normal)
                return CornerNode;
        }

        FNavNode* Find = GetCornerRectTop(CornerNode, EndNode);

        if (Find)
            return CornerNode;

        Find = GetCornerRectRight(CornerNode, EndNode);

        if (Find)
            return CornerNode;
    }

    return nullptr;
}

FNavNode* CNavigation::GetCornerRectRightBottom(
    FNavNode* Node, FNavNode* EndNode)
{
    int IndexY = Node->IndexY;
    int IndexX = Node->IndexX;

    while (true)
    {
        ++IndexX;
        --IndexY;

        if (IndexX >= mCountX || IndexY < 0)
            return nullptr;

        int Index = IndexY * mCountX + IndexX;

        FNavNode* CornerNode = mNodeList[Index];

        if (CornerNode == EndNode)
            return CornerNode;

        // 탐색하는 노드가 닫힌목록에 들어간 노드일경우
        else if (CornerNode->NodeType ==
            ENavNodeType::Close)
            return nullptr;

        else if (mTileMap->GetTileType(Index) ==
            ETileType::UnableToMove)
            return nullptr;

        int CornerX = IndexX - 1;
        int CornerY = IndexY;

        if (CornerX >= 0 && CornerY - 1 >= 0)
        {
            if (mTileMap->GetTileType(CornerY * mCountX + CornerX) == ETileType::UnableToMove &&
                mTileMap->GetTileType((CornerY - 1) * mCountX + CornerX) == ETileType::Normal)
                return CornerNode;
        }

        CornerX = IndexX;
        CornerY = IndexY + 1;

        if (CornerX + 1 < mCountX && CornerY < mCountY)
        {
            if (mTileMap->GetTileType(CornerY * mCountX + CornerX) == ETileType::UnableToMove &&
                mTileMap->GetTileType(CornerY * mCountX + CornerX + 1) == ETileType::Normal)
                return CornerNode;
        }

        FNavNode* Find = GetCornerRectBottom(CornerNode, EndNode);

        if (Find)
            return CornerNode;

        Find = GetCornerRectRight(CornerNode, EndNode);

        if (Find)
            return CornerNode;
    }

    return nullptr;
}

FNavNode* CNavigation::GetCornerRectLeftTop(
    FNavNode* Node, FNavNode* EndNode)
{
    int IndexY = Node->IndexY;
    int IndexX = Node->IndexX;

    while (true)
    {
        --IndexX;
        ++IndexY;

        if (IndexX < 0 || IndexY >= mCountY)
            return nullptr;

        int Index = IndexY * mCountX + IndexX;

        FNavNode* CornerNode = mNodeList[Index];

        if (CornerNode == EndNode)
            return CornerNode;

        // 탐색하는 노드가 닫힌목록에 들어간 노드일경우
        else if (CornerNode->NodeType ==
            ENavNodeType::Close)
            return nullptr;

        else if (mTileMap->GetTileType(Index) ==
            ETileType::UnableToMove)
            return nullptr;

        int CornerX = IndexX + 1;
        int CornerY = IndexY;

        if (CornerX < mCountX && CornerY + 1 < mCountY)
        {
            if (mTileMap->GetTileType(CornerY * mCountX + CornerX) == ETileType::UnableToMove &&
                mTileMap->GetTileType((CornerY + 1) * mCountX + CornerX) == ETileType::Normal)
                return CornerNode;
        }

        CornerX = IndexX;
        CornerY = IndexY - 1;

        if (CornerX - 1 >= 0 && CornerY >= 0)
        {
            if (mTileMap->GetTileType(CornerY * mCountX + CornerX) == ETileType::UnableToMove &&
                mTileMap->GetTileType(CornerY * mCountX + CornerX - 1) == ETileType::Normal)
                return CornerNode;
        }

        FNavNode* Find = GetCornerRectTop(CornerNode, EndNode);

        if (Find)
            return CornerNode;

        Find = GetCornerRectLeft(CornerNode, EndNode);

        if (Find)
            return CornerNode;
    }

    return nullptr;
}

FNavNode* CNavigation::GetCornerRectLeftBottom(
    FNavNode* Node, FNavNode* EndNode)
{
    int IndexY = Node->IndexY;
    int IndexX = Node->IndexX;

    while (true)
    {
        --IndexX;
        --IndexY;

        if (IndexX < 0 || IndexY < 0)
            return nullptr;

        int Index = IndexY * mCountX + IndexX;

        FNavNode* CornerNode = mNodeList[Index];

        if (CornerNode == EndNode)
            return CornerNode;

        // 탐색하는 노드가 닫힌목록에 들어간 노드일경우
        else if (CornerNode->NodeType ==
            ENavNodeType::Close)
            return nullptr;

        else if (mTileMap->GetTileType(Index) ==
            ETileType::UnableToMove)
            return nullptr;

        int CornerX = IndexX + 1;
        int CornerY = IndexY;

        if (CornerX < mCountX && CornerY - 1 >= 0)
        {
            if (mTileMap->GetTileType(CornerY * mCountX + CornerX) == ETileType::UnableToMove &&
                mTileMap->GetTileType((CornerY - 1) * mCountX + CornerX) == ETileType::Normal)
                return CornerNode;
        }

        CornerX = IndexX;
        CornerY = IndexY + 1;

        if (CornerX - 1 >= 0 && CornerY < mCountY)
        {
            if (mTileMap->GetTileType(CornerY * mCountX + CornerX) == ETileType::UnableToMove &&
                mTileMap->GetTileType(CornerY * mCountX + CornerX - 1) == ETileType::Normal)
                return CornerNode;
        }

        FNavNode* Find = GetCornerRectBottom(CornerNode, EndNode);

        if (Find)
            return CornerNode;

        Find = GetCornerRectLeft(CornerNode, EndNode);

        if (Find)
            return CornerNode;
    }

    return nullptr;
}

bool CNavigation::SortOpenList(FNavNode* Src, 
    FNavNode* Dest)
{
    return Src->Total > Dest->Total;
}
