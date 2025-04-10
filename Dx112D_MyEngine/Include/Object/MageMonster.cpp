#include "MageMonster.h"
#include "BulletObject.h"
#include "PlayerObject.h"

#include "../Asset/Sound/SoundManager.h"
#include "../Asset/Sound/Sound.h"
#include "../Asset/Texture/Texture.h"
#include "../Scene/SceneAssetManager.h"
#include "../Scene/SceneUIManager.h"
#include "../Scene/Scene.h"

#include "../Component/SpriteComponent.h"
#include "../Component/ColliderSphere2D.h"
#include "../Component/StateMachineComponent.h"
#include "../Component/WidgetComponent.h"
#include "../Component/MonsterMovement.h"
#include "../Animation/Animation2D.h"
#include "../UI/Common/ProgressBar.h"

#include "../Quest/QuestManager.h"
#include "../Share/Timer.h"




CMageMonster::CMageMonster()
{
}

CMageMonster::CMageMonster(const CMageMonster& Obj) :
    CMonsterObject(Obj)
{
}

CMageMonster::CMageMonster(CMageMonster&& Obj) :
    CMonsterObject(Obj)
{
}

CMageMonster::~CMageMonster()
{
}

bool CMageMonster::Init()
{
    CMonsterObject::Init();

    mRoot->SetWorldScale(150.f, 150.f, 1.f);
    mRoot->SetPivot(0.5f, 0.5f);
    mBody->SetRadius(40.f);

    mDetect->SetRadius(600.f);
    mDetect->SetCollisionBeginFunc<CMageMonster>(this,
        &CMageMonster::CollisionMonsterDetect);
    mDetect->SetCollisionEndFunc<CMageMonster>(this,
        &CMageMonster::CollisionMonsterDetectEnd);

    mAttackRange = CreateComponent<CColliderSphere2D>();
    mAttackRange->SetCollisionProfile("MonsterDetect");

    mAttackRange->SetRadius(350.f);


    mAttackRange->SetCollisionBeginFunc<CMageMonster>(this,
        &CMageMonster::CollisionAttackDetect);
    mAttackRange->SetCollisionEndFunc<CMageMonster>(this,
        &CMageMonster::CollisionAttackDetectEnd);

    mRoot->AddChild(mAttackRange);

    mMovement->SetUpdateComponent(mRoot);

    mMovement->SetMoveSpeed(200.f);
    mAnimation = mRoot->CreateAnimation2D<CAnimation2D>();

    // 몬스터 Idle 애니메이션
    mAnimation->AddSequence("MageMonster_IdleDown", 1.f, 1.f, true, false);
    mAnimation->AddSequence("MageMonster_IdleDownLeft", 1.f, 1.f, true, false);
    mAnimation->AddSequence("MageMonster_IdleLeft", 1.f, 1.f, true, false);
    mAnimation->AddSequence("MageMonster_IdleUpLeft", 1.f, 1.f, true, false);
    mAnimation->AddSequence("MageMonster_IdleUp", 1.f, 1.f, true, false);
    mAnimation->AddSequence("MageMonster_IdleUpRight", 1.f, 1.f, true, false);
    mAnimation->AddSequence("MageMonster_IdleRight", 1.f, 1.f, true, false);
    mAnimation->AddSequence("MageMonster_IdleDownRight", 1.f, 1.f, true, false);

    // 몬스터 Attack 애니메이션
    mAnimation->AddSequence("MageMonster_AttackDown", 1.f, 1.f, false, false);
    mAnimation->AddSequence("MageMonster_AttackDownLeft", 1.f, 1.f, false, false);
    mAnimation->AddSequence("MageMonster_AttackLeft", 1.f, 1.f, false, false);
    mAnimation->AddSequence("MageMonster_AttackUpLeft", 1.f, 1.f, false, false);
    mAnimation->AddSequence("MageMonster_AttackUp", 1.f, 1.f, false, false);
    mAnimation->AddSequence("MageMonster_AttackUpRight", 1.f, 1.f, false, false);
    mAnimation->AddSequence("MageMonster_AttackRight", 1.f, 1.f, false, false);
    mAnimation->AddSequence("MageMonster_AttackDownRight", 1.f, 1.f, false, false);

    mAnimation->AddSequence("NormalMonster_Death", 2.f, 1.f, false, false);

    mHP = 5;
    mMaxHP = mHP;

    CProgressBar* HPBar = mScene->GetUIManager()->CreateWidget<CProgressBar>("MonsterHPBar");

    float HPRatio = (float)mHP / (float)mMaxHP;

    HPBar->SetPercent(HPRatio);
    HPBar->SetPos(-40.f, -60.f);
    HPBar->SetSize(80.f, 20.f);
    HPBar->SetTexture(EProgressBarImageType::Frame, "PlayerHPBarFrame",
        TEXT("Texture\\UI\\HPBar\\Player_HPFrame.png"));
    HPBar->SetTexture(EProgressBarImageType::Back, "PlayerHPBarBack",
        TEXT("Texture\\UI\\HPBar\\Player_HPBarBack.png"));
    HPBar->SetTexture(EProgressBarImageType::Fill, "PlayerHPBarFill",
        TEXT("Texture\\UI\\HPBar\\Player_HPBarFill.png"));
    HPBar->SetTint(EProgressBarImageType::Fill, 0.f, 160.f, 0.f);

    mHPWidget->SetWidget(HPBar);

    mHPWidget->SetEnable(false);

    mMonsterType = EMonsterType::Mage;

    mStateMachine->InitStates(EObjectType::Monster);
    mStateMachine->ChangeStateMonster(EMonsterAIState::Idle, mMonsterDir);

    return true;
}

void CMageMonster::Update(float DeltaTime)
{
    CMonsterObject::Update(DeltaTime);

    if (mTarget)
    {
        if (!mTarget->IsActive())
        {
            mTarget = nullptr;
        }
    }

    if (mIsHit)
    {
        mHitTimer -= DeltaTime;
        if (mHitTimer <= 0.f)
        {
            mIsHit = false;
            mDead = true;
            mHPWidget->SetEnable(false);
            mHitTimer = 3.f;
        }
    }

    mStateMachine->Update(DeltaTime);
}

float CMageMonster::Damage(float Attack, CSceneObject* Obj)
{
    float Dmg = CSceneObject::Damage(Attack, Obj);

    mHP -= (int)Dmg;

    mScene->GetAssetManager()->FindSound("PlayerHit")->Play();

    CProgressBar* HPBar = dynamic_cast<CProgressBar*>(mHPWidget->GetWidget());
    if (!HPBar)
        return Dmg;

    mHPWidget->SetEnable(true);
    mIsHit = true;

    HPBar->SetPercent((float)mHP / (float)mMaxHP);

    if (mHP <= 0)
    {
        CQuestManager::GetInst()->QuestNotify(mName);
        mHP = 0;
        mDead = true;
        mStateMachine->ChangeStateMonster(EMonsterAIState::Death, EObjectDir::B);
    }

    return Dmg;
}

void CMageMonster::AIDeath()
{

    if (mTarget)
        mTarget = nullptr;

    mAttackRange->SetEnable(false);
    mBody->SetEnable(false);
    mDetect->SetEnable(false);
}

void CMageMonster::CollisionAttackDetect(const FVector3D& HitPoint,
    CColliderBase* Dest)
{
    CPlayerObject* Player = dynamic_cast<CPlayerObject*>(Dest->GetOwner());

    if (!Player)
        return;

   
    mStateMachine->ChangeStateMonster(EMonsterAIState::Attack, mMonsterDir);
}

void CMageMonster::CollisionAttackDetectEnd(CColliderBase* Dest)
{
 
    mStateMachine->ChangeStateMonster(EMonsterAIState::Trace, mMonsterDir);
}

void CMageMonster::CollisionMonsterDetect(const FVector3D& HitPoint,
    CColliderBase* Dest)
{
    // Dest가 인지반경에 들어왔을 경우
    // 몬스터 타겟 설정.
    mTarget = Dest->GetOwner();
    
    if (!mTarget)
        return;

    // 인지범위를 늘려준다.
    mDetect->SetRadius(800.f);
    // 추격상태로 변경.
    mStateMachine->ChangeStateMonster(EMonsterAIState::Trace, mMonsterDir);
}

void CMageMonster::CollisionMonsterDetectEnd(CColliderBase* Dest)
{
    // 인지반경을 벗어났을 경우
    // 타겟을 없애고 인지반경을 줄인다.
    mTarget = nullptr;
    mDetect->SetRadius(600.f);
    // Idle 상태로 변경
    mStateMachine->ChangeStateMonster(EMonsterAIState::Idle, mMonsterDir);

}