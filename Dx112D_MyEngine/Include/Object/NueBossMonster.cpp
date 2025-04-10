#include "NueBossMonster.h"
#include "BulletObject.h"
#include "PlayerObject.h"
#include "TileMapObj.h"
#include "../Scene/SceneAssetManager.h"
#include "../Asset/Sound/SoundManager.h"

#include "../Share/Timer.h"
#include "../Scene/Scene.h"

#include "../Component/SpriteComponent.h"
#include "../Component/ColliderSphere2D.h"
#include "../Component/StateMachineComponent.h"
#include "../Component/WidgetComponent.h"

#include "../Animation/Animation2D.h"
#include "../UI/Common/ProgressBar.h"

#include "../Quest/QuestManager.h"


CNueBossMonster::CNueBossMonster()
{
}

CNueBossMonster::CNueBossMonster(const CNueBossMonster& Obj) :
    CMonsterObject(Obj)
{
}

CNueBossMonster::CNueBossMonster(CNueBossMonster&& Obj) :
    CMonsterObject(Obj)
{
}

CNueBossMonster::~CNueBossMonster()
{
}

bool CNueBossMonster::Init()
{
    CMonsterObject::Init();

    mRoot->SetWorldScale(500.f, 500.f, 1.f);
    mRoot->SetPivot(0.5f, 0.5f);
    mBody->SetRadius(100.f);

    mBody->SetCollisionProfile("BossMonster");


    mAnimation = mRoot->CreateAnimation2D<CAnimation2D>();

    mBody->SetCollisionBeginFunc<CNueBossMonster>(this,
        &CNueBossMonster::CollisionMonster);
    mBody->SetCollisionEndFunc<CNueBossMonster>(this,
        &CNueBossMonster::CollisionMonsterEnd);

    // 1. 처음 보스가 나왔을 시 출력될 Ready 상태 애니메이션
    mAnimation->AddSequence("NueBossMonster_Ready", 1.5f, 1.5f, false, false);
    // 2. 보스의 기본 Idle 상태 애니메이션
    mAnimation->AddSequence("NueBossMonster_Idle", 1.5f, 1.5f, true, false);
    // 3. 보스의 기본 Attack 상태 애니메이션
    mAnimation->AddSequence("NueBossMonster_Swing", 1.f, 1.f, false, false);
    // 4. 보스가 특수패턴을 실행하기 전 출력될 1번째 애니메이션
    // 공중으로 날아오르는 애니메이션
    mAnimation->AddSequence("NueBossMonster_SpecialPatternReady", 1.5f, 1.5f, false, false);
    // 5. 보스가 특수패턴을 실행하기 전 출력될 2번째 애니메이션
    // 공중에서 계속 날개짓을 하는 애니메이션
    mAnimation->AddSequence("NueBossMonster_SpecialPatternReady2", 1.5f, 1.5f, true, false);
    // 6. 보스가 특수패턴을 실행하기 바로 전 애니메이션
    // 공중에서 칼을 들어올리는 애니메이션
    mAnimation->AddSequence("NueBossMonster_SpecialPatternWait", 0.5f, 0.5f, false, false);
    // 7. 보스가 특수패턴을 실행할 때 출력할 애니메이션
    mAnimation->AddSequence("NueBossMonster_SpecialPattern", 1.f, 1.f, false, false);
    // 8. 보스가 그로기 상태일 때 출력될 애니메이션
    mAnimation->AddSequence("NueBossMonster_Grogy", 1.f, 1.f, false, false);
    // 9. 보스가 죽었을 때 출력될 애니메이션
    mAnimation->AddSequence("NueBossMonster_Death", 1.f, 1.f, false, false);

    
    mMonsterType = EMonsterType::Boss;
    mStateMachine->InitStates(EObjectType::BossMonster);

    mStateMachine->ChangeStateBossMonster(EBossMonsterAIState::Start);

    mHP = 30;
    mMaxHP = mHP;

    return true;
}

void CNueBossMonster::Update(float DeltaTime)
{
    CMonsterObject::Update(DeltaTime);

    if (mTarget)
    {
        if (!mTarget->IsActive())
        {
            mTarget = nullptr;
        }
    }

    mStateMachine->Update(DeltaTime);
}



void CNueBossMonster::AttackNotify()
{

}
void CNueBossMonster::AttackEnd()
{

}

void CNueBossMonster::CollisionMonster(const FVector3D& HitPoint, 
    CColliderBase* Dest)
{
    FCollisionProfile* DestProfile = Dest->GetProfile();

    if (ECollisionChannel::PlayerAttack == DestProfile->Channel ||
        ECollisionChannel::BossAttack == DestProfile->Channel)
    {
        Damage(1.f, Dest->GetOwner());
        PlusPatternCount();
    }
}

void CNueBossMonster::CollisionMonsterEnd(CColliderBase* Dest)
{
}

void CNueBossMonster::AIDeath()
{
    if (mTarget)
        mTarget = nullptr;

    mBody->SetEnable(false);
}

float CNueBossMonster::Damage(float Attack, CSceneObject* Obj)
{
    float Dmg = CSceneObject::Damage(Attack, Obj);

    mHP -= (int)Dmg;

    mScene->GetAssetManager()->FindSound("NueBossHit")->Play();

    if (mHP <= 0)
    {
        mDead = true;
        mHP = 0;
        mStateMachine->ChangeStateBossMonster(EBossMonsterAIState::Death);
        CQuestManager::GetInst()->QuestNotify(mName);
    }

    return Dmg;
}

void CNueBossMonster::CollisionAttackDetect(const FVector3D& HitPoint,
    CColliderBase* Dest)
{
    /*CPlayerObject* Player = dynamic_cast<CPlayerObject*>(Dest->GetOwner());

    if (!Player)
        return;


    mStateMachine->ChangeStateMonster(EMonsterAIState::Attack, mMonsterDir);*/
}

void CNueBossMonster::CollisionAttackDetectEnd(CColliderBase* Dest)
{

   // mStateMachine->ChangeStateMonster(EMonsterAIState::Trace, mMonsterDir);
}

void CNueBossMonster::CollisionMonsterDetect(const FVector3D& HitPoint,
    CColliderBase* Dest)
{
    
}

void CNueBossMonster::CollisionMonsterDetectEnd(CColliderBase* Dest)
{
 

}
