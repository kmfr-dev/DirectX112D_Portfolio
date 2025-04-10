#include "BossBullet.h"


#include "../Component/SpriteComponent.h"
#include "../Animation/Animation2D.h"

CBossBullet::CBossBullet()
{
}

CBossBullet::CBossBullet(const CBossBullet& Obj)
{
}

CBossBullet::CBossBullet(CBossBullet&& Obj)
{
}

CBossBullet::~CBossBullet()
{
}

bool CBossBullet::Init()
{
	CBulletObject::Init();

	SetBulletCollisionProfile("BossAttack");
    mAnimation = mRoot->CreateAnimation2D<CAnimation2D>();
    mAnimation->AddSequence("NueBoss_AttackBall", 0.1f, 0.1f, true, false);
	return true;
}

void CBossBullet::Update(float DeltaTime)
{
	CBulletObject::Update(DeltaTime);
}

void CBossBullet::ChangeAnimation(const std::string Name)
{
	mAnimation->ChangeAnimation(Name);
}
