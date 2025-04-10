#include "BulletObject.h"
#include "NueBossMonster.h"
#include "SpriteEffect.h"
#include "../Component/StaticMeshComponent.h"
#include "../Component/MovementComponent.h"
#include "../Component/ColliderSphere2D.h"
#include "../Component/SpriteComponent.h"
#include "../Scene/Scene.h"
#include "../Share/Log.h"

#include "../Scene/SceneAssetManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Asset/Sound/SoundManager.h"

CBulletObject::CBulletObject()
{
}

CBulletObject::CBulletObject(const CBulletObject& Obj)  :
    CSceneObject(Obj)
{
}

CBulletObject::CBulletObject(CBulletObject&& Obj) :
    CSceneObject(Obj)
{
}

CBulletObject::~CBulletObject()
{
}

void CBulletObject::SetBulletCollisionProfile(
    const std::string& Name)
{
    mBody->SetCollisionProfile(Name);
}

bool CBulletObject::Init()
{
    mRoot = CreateComponent<CSpriteComponent>();
    mBody = CreateComponent<CColliderSphere2D>();

    mMovement = CreateComponent<CMovementComponent>();

    /*mRoot->SetMesh("CenterRect");
    mRoot->SetShader("ColorMeshShader");*/

    mRoot->SetTexture("Bullet", TEXT("Texture\\Monster\\MageMonster_Bullet.png"));
    mRoot->SetPivot(0.5f, 0.5f);
   
    mRoot->SetWorldScale(50.f, 50.f, 1.f);

    SetRootComponent(mRoot);

    mBody->SetRadius(25.f);

    mBody->SetCollisionBeginFunc<CBulletObject>(this,
        &CBulletObject::CollisionBullet);

    mRoot->AddChild(mBody);

    mMovement->SetUpdateComponent(mRoot);

    return true;
}

void CBulletObject::Update(float DeltaTime)
{
    CSceneObject::Update(DeltaTime);

    FVector3D NewPos = GetWorldPosition() + mDirection * mSpeed * DeltaTime;
    SetWorldPos(NewPos);
}

void CBulletObject::CollisionBullet(const FVector3D& HitPoint,
    CColliderBase* Dest)
{
    FCollisionProfile* Profile = Dest->GetProfile();

    if (Profile->Channel == ECollisionChannel::PlayerAttack)
        return;

    //CLog::PrintLog("Collision");

    /*Dest->GetOwner()->Damage(1.f, this);

    CSpriteEffect* Effect = mScene->CreateObj<CSpriteEffect>("HitEffect");

    Effect->SetAnimation("Explosion");

    Effect->SetWorldPos(HitPoint);
    Effect->SetWorldScale(100.f, 100.f);*/
    Destroy();
}
