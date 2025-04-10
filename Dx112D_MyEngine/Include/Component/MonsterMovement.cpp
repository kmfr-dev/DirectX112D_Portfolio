#include "MonsterMovement.h"
#include "SceneComponent.h"
#include "NavAgent.h"
#include "TileMapComponent.h"
#include "ColliderBase.h"
#include "ColliderSphere2D.h"

#include "../Object/MonsterObject.h"
#include "../Object/TileMapObj.h"
#include "../Scene/Scene.h"

CMonsterMovement::CMonsterMovement()
{
}
CMonsterMovement::CMonsterMovement(const CMonsterMovement& Com) :
    CMovementComponent(Com)

{
}
CMonsterMovement::CMonsterMovement(CMonsterMovement&& Com) :
    CMovementComponent(Com)
{
}
CMonsterMovement::~CMonsterMovement()
{
}

bool CMonsterMovement::Init()
{
    if (!CMovementComponent::Init())
        return false;

    return true;
}

bool CMonsterMovement::Init(const char* FileName)
{
    if (!CMovementComponent::Init(FileName))
        return false;

    return true;
}

void CMonsterMovement::PreUpdate(float DeltaTime)
{
    CMovementComponent::PreUpdate(DeltaTime);
}

void CMonsterMovement::Update(float DeltaTime)
{
    CMovementComponent::Update(DeltaTime);
}

void CMonsterMovement::PostUpdate(float DeltaTime)
{
    CMovementComponent::PostUpdate(DeltaTime);
}

CMonsterMovement* CMonsterMovement::Clone()
{
    return new CMonsterMovement(*this);
}

void CMonsterMovement::EndFrame()
{
    CMovementComponent::EndFrame();
}

bool CMonsterMovement::CheckBlockedByTile()
{
    CMonsterObject* Owner = dynamic_cast<CMonsterObject*>(mUpdateComponent->GetOwner());

    if (!Owner)
        return false;

    CColliderBase* Collider = Owner->GetCollider();

    if (Collider->GetColliderShape() == EColliderShape::Sphere2D)
    {
        FVector3D WorldPos = Collider->GetWorldPosition();
        FVector3D MoveTargetPos = WorldPos + mMoveStep;

        FVector2D Center;
        Center.x = MoveTargetPos.x;
        Center.y = MoveTargetPos.y;

        float Radius = static_cast<CColliderSphere2D*>(Collider)->GetRadius();

        FVector2D Min = Center - FVector2D(Radius, Radius);
        FVector2D Max = Center + FVector2D(Radius, Radius);

        CTileMapComponent* TileMap = mScene->GetMapObj()->GetTileMap();
        if (!TileMap)
            return false;

        FVector2D TileCenter = (Min + Max) * 0.5f;

        int TileX = TileMap->GetTileIndexX(TileCenter);
        int TileY = TileMap->GetTileIndexY(TileCenter);

        if (TileX == -1 || TileY == -1)
            return false;

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

                FVector2D TileMin = Tile->GetPos();
                FVector2D TileMax = TileMin + Tile->GetSize();

                // 원과 타일 AABB 충돌 검사
                FVector2D ClosestPoint;
                ClosestPoint.x = Clamp(Center.x, TileMin.x, TileMax.x);
                ClosestPoint.y = Clamp(Center.y, TileMin.y, TileMax.y);

                FVector2D Diff = ClosestPoint - Center;
                float DistSq = Diff.x * Diff.x + Diff.y * Diff.y;

                if (DistSq <= Radius * Radius)
                {
                    return true; // 충돌 발생
                }
            }
        }

        return false; // 이동 가능
    }

    return false; // 충돌체가 Sphere2D가 아님
}
