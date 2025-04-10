#include "PlayerMovement.h"
#include "SceneComponent.h"
#include "NavAgent.h"
#include "TileMapComponent.h"
#include "ColliderBase.h"
#include "ColliderAABB2D.h"

#include "../Object/PlayerObject.h"
#include "../Object/TileMapObj.h"
#include "../Scene/Scene.h"

CPlayerMovement::CPlayerMovement()
{
}
CPlayerMovement::CPlayerMovement(const CPlayerMovement& Com) :
	CMovementComponent(Com)

{
}
CPlayerMovement::CPlayerMovement(CPlayerMovement&& Com) :
	CMovementComponent(Com)
{
}
CPlayerMovement::~CPlayerMovement()
{
}

bool CPlayerMovement::Init()
{
	if (!CMovementComponent::Init())
		return false;

	return true;
}

bool CPlayerMovement::Init(const char* FileName)
{
	if (!CMovementComponent::Init(FileName))
		return false;

	return true;
}

void CPlayerMovement::PreUpdate(float DeltaTime)
{
	CMovementComponent::PreUpdate(DeltaTime);
}

void CPlayerMovement::Update(float DeltaTime)
{
	CMovementComponent::Update(DeltaTime);
}

void CPlayerMovement::PostUpdate(float DeltaTime)
{
	CMovementComponent::PostUpdate(DeltaTime);
}

CPlayerMovement* CPlayerMovement::Clone()
{
	return new CPlayerMovement(*this);
}

void CPlayerMovement::EndFrame()
{
	CMovementComponent::EndFrame();
}

bool CPlayerMovement::CheckBlockedByTile()
{
    CPlayerObject* Owner = dynamic_cast<CPlayerObject*>
        (mUpdateComponent->GetOwner());

    if (!Owner)
        return false;

    CColliderBase* Collider = Owner->GetCollider();

    if (EColliderShape::AABB2D == Collider->GetColliderShape())
    {
        FVector3D Min = Collider->GetMin() + mMoveStep;
        FVector3D Max = Collider->GetMax() + mMoveStep;

        CTileMapComponent* TileMap = mScene->GetMapObj()->GetTileMap();

        if (!TileMap)
            return false;

        // 4. Min/Max의 중심 기준 타일 인덱스
        FVector3D Center = (Min + Max) * 0.5f;

        int TileX = TileMap->GetTileIndexX(Center);
        int TileY = TileMap->GetTileIndexY(Center);

        if (TileX == -1 || TileY == -1)
            return false;

        // 5. 주변 3x3 타일만 검사
        for (int y = TileY - 1; y <= TileY + 1; ++y)
        {
            for (int x = TileX - 1; x <= TileX + 1; ++x)
            {
                if (x < 0 || x >= TileMap->GetTileCountX() ||
                    y < 0 || y >= TileMap->GetTileCountY())
                    continue;

                int Index = y * TileMap->GetTileCountX() + x;
                const CTile* Tile = TileMap->GetTile(Index);

                if (!Tile || Tile->GetType() != ETileType::UnableToMove)
                    continue;

                // 타일의 AABB 계산
                FVector2D TileMin = Tile->GetPos();
                FVector2D TileMax = TileMin + Tile->GetSize();

                // Collider AABB와 타일 AABB 충돌 검사
                if (!(Max.x < TileMin.x || Min.x > TileMax.x ||
                    Max.y < TileMin.y || Min.y > TileMax.y))
                {
                    // 충돌 발생 → 이동 차단
                    return true;
                }
            }
        }
        // 이동 가능
        return false;
    }
    // 이동 가능
    return false;
}
