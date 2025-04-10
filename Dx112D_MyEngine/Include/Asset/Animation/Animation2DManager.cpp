#include "Animation2DManager.h"
#include "Animation2DData.h"
#include "../../Animation/Animation2D.h"

CAnimation2DManager::CAnimation2DManager()
{
}

CAnimation2DManager::~CAnimation2DManager()
{
    CAnimation2D::DestroyCBuffer();
}

bool CAnimation2DManager::Init()
{
    CAnimation2D::CreateCBuffer();

    // 시작씬 캐릭터 이미지 애니메이션
    CreateAnimation("StartOguIdle");
    SetAnimationTextureType("StartOguIdle",
        EAnimationTextureType::SpriteSheet);
    SetTexture("StartOguIdle", "StartOguSprite",
        TEXT("Texture\\StartScene\\StartOgu.png"));

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            AddFrame("StartOguIdle", j * 510.f, i * 510.f, 510.f, 510.f);
        }
    }

    PlayerAnimationInit();
    MonsterAnimationInit();
    BossAnimationInit();
    NPCAnimationInit();

    return true;
}

void CAnimation2DManager::PlayerAnimationInit()
{
    // Player Idle 모션 8방향
    {
        // 아래 Idle
        CreateAnimation("Player_IdleDown");
        SetAnimationTextureType("Player_IdleDown",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_IdleDown", "PlayerIdle1Sprite",
            TEXT("Texture\\Player\\Player_Idle1.png"));

        for (int i = 0; i < 7; ++i)
        {
            AddFrame("Player_IdleDown", i * 360.f, 0.f, 360.f, 360.f);
        }

        // 왼쪽 대각선 아래 Idle
        CreateAnimation("Player_IdleDownLeft");
        SetAnimationTextureType("Player_IdleDownLeft",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_IdleDownLeft", "PlayerIdle2Sprite",
            TEXT("Texture\\Player\\Player_Idle2.png"));

        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 5; ++j)
            {
                AddFrame("Player_IdleDownLeft", j * 340.f, i * 340.f, 360.f, 360.f);

                if (i == 1 && j == 1)
                    break;
            }
        }

        // 오른쪽 대각선 아래 Idle
        CreateAnimation("Player_IdleDownRight");
        SetAnimationTextureType("Player_IdleDownRight",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_IdleDownRight", "PlayerIdle2Sprite",
            TEXT("Texture\\Player\\Player_Idle2.png"));

        for (int i = 1; i < 3; ++i)
        {
            for (int j = 2; j < 5; ++j)
            {
                int frameIndex = j;

                if (i == 3)
                    frameIndex = i - 2;

                AddFrame("Player_IdleDownRight", frameIndex * 340.f, i * 340.f, 360.f, 360.f);

                if (i == 2 && j == 3)
                    break;
            }
        }

        // 위 Idle
        CreateAnimation("Player_IdleUp");
        SetAnimationTextureType("Player_IdleUp",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_IdleUp", "PlayerIdle1Sprite",
            TEXT("Texture\\Player\\Player_Idle1.png"));

        for (int i = 0; i < 7; ++i)
        {
            AddFrame("Player_IdleUp", i * 360.f, 360.f, 360.f, 360.f);
        }

        // 오른쪽 위 Idle
        CreateAnimation("Player_IdleUpRight");
        SetAnimationTextureType("Player_IdleUpRight",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_IdleUpRight", "PlayerIdle2Sprite",
            TEXT("Texture\\Player\\Player_Idle2.png"));

        for (int i = 2; i < 5; ++i)
        {
            for (int j = 0; j < 5; ++j)
            {
                if (i == 2)
                    j = 4;

                AddFrame("Player_IdleUpRight", j * 340.f, i * 340.f, 360.f, 360.f);

                if (i == 4 && j == 0)
                    break;
            }
        }

        // 왼쪽 위 Idle
        CreateAnimation("Player_IdleUpLeft");
        SetAnimationTextureType("Player_IdleUpLeft",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_IdleUpLeft", "PlayerIdle2Sprite",
            TEXT("Texture\\Player\\Player_Idle2.png"));

        for (int i = 4; i < 5; ++i)
        {
            for (int j = 1; j < 5; ++j)
            {
                int frameIndex = j;

                if (i == 5)
                    frameIndex = j - 1;

                AddFrame("Player_IdleUpLeft", frameIndex * 340.f, i * 340.f, 360.f, 360.f);

                if (i == 5 && j == 1)
                    break;
            }
        }

        // 왼쪽 Idle
        CreateAnimation("Player_IdleLeft");
        SetAnimationTextureType("Player_IdleLeft",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_IdleLeft", "PlayerIdle1Sprite",
            TEXT("Texture\\Player\\Player_Idle1.png"));

        for (int i = 0; i < 7; ++i)
        {
            AddFrame("Player_IdleLeft", i * 360.f, 720.f, 360.f, 360.f);
        }

        // 오른쪽 Idle
        CreateAnimation("Player_IdleRight");
        SetAnimationTextureType("Player_IdleRight",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_IdleRight", "PlayerIdle1Sprite",
            TEXT("Texture\\Player\\Player_Idle1.png"));

        for (int i = 0; i < 7; ++i)
        {
            AddFrame("Player_IdleRight", i * 360.f, 1080.f, 360.f, 360.f);
        }
    };
    
    // Player 걷는 모션 8방향
    {
        // 아래
        CreateAnimation("Player_WalkDown");
        SetAnimationTextureType("Player_WalkDown",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_WalkDown", "PlayerMovement1Sprite",
            TEXT("Texture\\Player\\Player_MoveMent1.png"));

        for (int i = 0; i < 10; ++i)
        {
            AddFrame("Player_WalkDown", i * 360.f, 0.f, 360.f, 360.f);
        }

        // 왼쪽 아래
        CreateAnimation("Player_WalkDownLeft");
        SetAnimationTextureType("Player_WalkDownLeft",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_WalkDownLeft", "PlayerMovement2Sprite",
            TEXT("Texture\\Player\\Player_MoveMent2.png"));

        for (int i = 0; i < 10; ++i)
        {
            AddFrame("Player_WalkDownLeft", i * 360.f, 0.f, 360.f, 360.f);
        }

        // 오른쪽 아래
        CreateAnimation("Player_WalkDownRight");
        SetAnimationTextureType("Player_WalkDownRight",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_WalkDownRight", "PlayerMovement2Sprite",
            TEXT("Texture\\Player\\Player_MoveMent2.png"));

        for (int i = 0; i < 10; ++i)
        {
            AddFrame("Player_WalkDownRight", i * 360.f, 720.f, 360.f, 360.f);
        }

        // 위
        CreateAnimation("Player_WalkUp");
        SetAnimationTextureType("Player_WalkUp",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_WalkUp", "PlayerMovement1Sprite",
            TEXT("Texture\\Player\\Player_MoveMent1.png"));

        for (int i = 0; i < 10; ++i)
        {
            AddFrame("Player_WalkUp", i * 360.f, 360.f, 360.f, 360.f);
        }

        // 왼쪽 위
        CreateAnimation("Player_WalkUpLeft");
        SetAnimationTextureType("Player_WalkUpLeft",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_WalkUpLeft", "PlayerMovement2Sprite",
            TEXT("Texture\\Player\\Player_MoveMent2.png"));

        for (int i = 0; i < 10; ++i)
        {
            AddFrame("Player_WalkUpLeft", i * 360.f, 360.f, 360.f, 360.f);
        }

        // 오른쪽 위
        CreateAnimation("Player_WalkUpRight");
        SetAnimationTextureType("Player_WalkUpRight",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_WalkUpRight", "PlayerMovement2Sprite",
            TEXT("Texture\\Player\\Player_MoveMent2.png"));

        for (int i = 0; i < 10; ++i)
        {
            AddFrame("Player_WalkUpRight", i * 360.f, 1080.f, 360.f, 360.f);
        }

        // 왼쪽
        CreateAnimation("Player_WalkLeft");
        SetAnimationTextureType("Player_WalkLeft",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_WalkLeft", "PlayerMovement1Sprite",
            TEXT("Texture\\Player\\Player_MoveMent1.png"));

        for (int i = 0; i < 9; ++i)
        {
            AddFrame("Player_WalkLeft", i * 360.f, 720.f, 360.f, 360.f);
        }

        // 오른쪽
        CreateAnimation("Player_WalkRight");
        SetAnimationTextureType("Player_WalkRight",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_WalkRight", "PlayerMovement1Sprite",
            TEXT("Texture\\Player\\Player_MoveMent1.png"));

        for (int i = 0; i < 9; ++i)
        {
            AddFrame("Player_WalkRight", i * 360.f, 1080.f, 360.f, 360.f);
        }
    }

    // Player 뛰는 모션 8방향
    {
        // 아래
        CreateAnimation("Player_RunDown");
        SetAnimationTextureType("Player_RunDown",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_RunDown", "PlayerMovement1Sprite",
            TEXT("Texture\\Player\\Player_MoveMent1.png"));

        for (int i = 0; i < 8; ++i)
            AddFrame("Player_RunDown", i * 360.f, 1800.f, 360.f, 360.f);

        // 위
        CreateAnimation("Player_RunUp");
        SetAnimationTextureType("Player_RunUp",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_RunUp", "PlayerMovement1Sprite",
            TEXT("Texture\\Player\\Player_MoveMent1.png"));

        for (int i = 0; i < 8; ++i)
            AddFrame("Player_RunUp", i * 360.f, 2160.f, 360.f, 360.f);

        // 왼쪽
        CreateAnimation("Player_RunLeft");
        SetAnimationTextureType("Player_RunLeft",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_RunLeft", "PlayerMovement1Sprite",
            TEXT("Texture\\Player\\Player_MoveMent1.png"));

        for (int i = 0; i < 8; ++i)
            AddFrame("Player_RunLeft", i * 360.f, 2520.f, 360.f, 360.f);

        // 오른쪽
        CreateAnimation("Player_RunRight");
        SetAnimationTextureType("Player_RunRight",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_RunRight", "PlayerMovement1Sprite",
            TEXT("Texture\\Player\\Player_MoveMent1.png"));

        for (int i = 0; i < 8; ++i)
            AddFrame("Player_RunRight", i * 360.f, 2880.f, 360.f, 360.f);
         
        // 왼쪽 아래
        CreateAnimation("Player_RunDownLeft");
        SetAnimationTextureType("Player_RunDownLeft",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_RunDownLeft", "PlayerMovement2Sprite",
            TEXT("Texture\\Player\\Player_MoveMent2.png"));

        for (int i = 0; i < 7; ++i)
            AddFrame("Player_RunDownLeft", i * 360.f, 1800.f, 360.f, 360.f);

        // 왼쪽 위
        CreateAnimation("Player_RunUpLeft");
        SetAnimationTextureType("Player_RunUpLeft",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_RunUpLeft", "PlayerMovement2Sprite",
            TEXT("Texture\\Player\\Player_MoveMent2.png"));

        for (int i = 0; i < 7; ++i)
            AddFrame("Player_RunUpLeft", i * 360.f, 2160.f, 360.f, 360.f);

        // 오른쪽 아래
        CreateAnimation("Player_RunDownRight");
        SetAnimationTextureType("Player_RunDownRight",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_RunDownRight", "PlayerMovement2Sprite",
            TEXT("Texture\\Player\\Player_MoveMent2.png"));

        for (int i = 0; i < 7; ++i)
            AddFrame("Player_RunDownRight", i * 360.f, 2520.f, 360.f, 360.f);

        // 오른쪽 위
        CreateAnimation("Player_RunUpRight");
        SetAnimationTextureType("Player_RunUpRight",
            EAnimationTextureType::SpriteSheet);
        SetTexture("Player_RunUpRight", "PlayerMovement2Sprite",
            TEXT("Texture\\Player\\Player_MoveMent2.png"));

        for (int i = 0; i < 7; ++i)
            AddFrame("Player_RunUpRight", i * 360.f, 2880.f, 360.f, 360.f);

    }

    // Player 공격 모션 8방향
    {
        std::vector<std::string> AnimationNames =
        {
            "Player_AttackDown",
            "Player_AttackDownLeft",
            "Player_AttackLeft",
            "Player_AttackUpLeft",
            "Player_AttackUp",
            "Player_AttackUpRight",
            "Player_AttackRight",
            "Player_AttackDownRight"
        };

        // 시작 위치와 프레임 개수 
        // 각 방향별 첫 프레임 인덱스
        int startIndices[8] = { 0, 2, 4, 0, 2, 4, 0, 2 };

        // 시작 줄
        int rowStart[8] = { 0, 2, 4, 0, 2, 4, 0, 2 };

        // 각 방향별 프레임 개수
        int framePerRow = 9;

        std::string SpriteName = "Player_AttackSprite1";
        const TCHAR* TextureName = TEXT("Texture\\Player\\Player_Attack1.png");

        for (int dir = 0; dir < 8; ++dir)
        {
            CreateAnimation(AnimationNames[dir]);
            SetAnimationTextureType(AnimationNames[dir], EAnimationTextureType::SpriteSheet);

            if (dir == 3)
            {
                SpriteName = "Player_AttackSprite2";
                TextureName = TEXT("Texture\\Player\\Player_Attack2.png");
            }
            else if (dir == 6)
            {
                SpriteName = "Player_AttackSprite3";
                TextureName = TEXT("Texture\\Player\\Player_Attack3.png");
            }

            SetTexture(AnimationNames[dir], SpriteName , TextureName);

            int frameCount = 0; // 프레임 개수 카운트

            for (int i = rowStart[dir]; i < 7; ++i)
            {
                for (int j = (i == rowStart[dir] ? startIndices[dir] : 0); j < 8; ++j)
                {
                    AddFrame(AnimationNames[dir], j * 256.f, i * 256.f, 256.f, 256.f);
                    frameCount++;

                    if (frameCount >= framePerRow)
                        break;
                }

                if (frameCount >= framePerRow) 
                    break;
            }
        }
    }

    // Player 맞는 애니메이션
    {
        std::vector<std::string> AnimationNames =
        {
            "Player_HurtDown",
            "Player_HurtDownLeft",
            "Player_HurtLeft",
            "Player_HurtUpLeft",
            "Player_HurtUp",
            "Player_HurtUpRight",
            "Player_HurtRight",
            "Player_HurtDownRight"
        };

        // 시작 위치와 프레임 개수 
        // 각 방향별 첫 프레임 인덱스
        int startIndices[8] = { 0, 3, 1, 4, 2, 0, 3, 1 };

        // 시작 줄
        int rowStart[8] = { 0, 0, 1, 1, 2, 3, 3, 4 };

        // 각 방향별 프레임 개수
        int framePerRow = 3;

        std::string SpriteName = "Player_HurtSprite";
        const TCHAR* TextureName = TEXT("Texture\\Player\\Player_Hurt.png");

        for (int dir = 0; dir < 8; ++dir)
        {
            CreateAnimation(AnimationNames[dir]);
            SetAnimationTextureType(AnimationNames[dir], EAnimationTextureType::SpriteSheet);

            SetTexture(AnimationNames[dir], SpriteName, TextureName);

            int frameCount = 0; // 프레임 개수 카운트

            for (int i = rowStart[dir]; i < 5; ++i)
            {
                for (int j = (i == rowStart[dir] ? startIndices[dir] : 0); j < 5; ++j)
                {
                    AddFrame(AnimationNames[dir], j * 360.f, i * 360.f, 360.f, 360.f);
                    frameCount++;

                    if (frameCount >= framePerRow)
                        break;
                }

                if (frameCount >= framePerRow)
                    break;
            }
        }
    }
}

void CAnimation2DManager::MonsterAnimationInit()
{
    std::vector<std::string> AnimationNames = 
    {
        "NearingMonster_IdleDown",
        "NearingMonster_IdleDownLeft",
        "NearingMonster_IdleLeft",
        "NearingMonster_IdleUpLeft",
        "NearingMonster_IdleUp",
        "NearingMonster_IdleUpRight",
        "NearingMonster_IdleRight",
        "NearingMonster_IdleDownRight"
    };

    // 시작 위치와 프레임 개수 
    // 각 방향별 첫 프레임 인덱스
    int startIndices[8] = {0, 6, 5, 4, 3, 2, 1, 0};

    // 시작 줄
    int rowStart[8] = {0, 0, 1, 2, 3, 4, 5, 6};

    // 각 방향별 프레임 개수
    int framePerRow[8] = {6, 6, 6, 6, 6, 6, 6, 6}; 

    // 애니메이션 설정 루프
    for (int dir = 0; dir < 8; ++dir)
    {
        CreateAnimation(AnimationNames[dir]);
        SetAnimationTextureType(AnimationNames[dir], EAnimationTextureType::SpriteSheet);
        SetTexture(AnimationNames[dir], "NearingMonster_IdleSprite", TEXT("Texture\\Monster\\NearingMonster_Idle.png"));

        int frameCount = 0; // 프레임 개수 카운트

        for (int i = rowStart[dir]; i < 7; ++i)
        {
            for (int j = (i == rowStart[dir] ? startIndices[dir] : 0); j < 7; ++j)
            {
                AddFrame(AnimationNames[dir], j * 360.f, i * 360.f, 360.f, 360.f);
                frameCount++;

                if (frameCount >= framePerRow[dir]) break;
            }
            if (frameCount >= framePerRow[dir]) break;
        }
    }

    AnimationNames.clear();

    AnimationNames =
    {
        "NearingMonster_AttackDown",
        "NearingMonster_AttackDownLeft",
        "NearingMonster_AttackLeft",
        "NearingMonster_AttackUpLeft",
        "NearingMonster_AttackUp",
        "NearingMonster_AttackUpRight",
        "NearingMonster_AttackRight",
        "NearingMonster_AttackDownRight"
    };

    for (int dir = 0; dir < 8; ++dir)
    {
        CreateAnimation(AnimationNames[dir]);
        SetAnimationTextureType(AnimationNames[dir], EAnimationTextureType::SpriteSheet);
        SetTexture(AnimationNames[dir], "NearingMonster_AttackSprite", TEXT("Texture\\Monster\\NearingMonster_Attack.png"));

        int frameCount = 0; 

        for (int i = rowStart[dir]; i < 7; ++i)
        {
            for (int j = (i == rowStart[dir] ? startIndices[dir] : 0); j < 7; ++j)
            {
                AddFrame(AnimationNames[dir], j * 360.f, i * 360.f, 360.f, 360.f);
                frameCount++;

                if (frameCount >= framePerRow[dir]) break;
            }
            if (frameCount >= framePerRow[dir]) break;
        }
    }

    AnimationNames.clear();

    AnimationNames =
    {
        "MageMonster_IdleDown",
        "MageMonster_IdleDownLeft",
        "MageMonster_IdleLeft",
        "MageMonster_IdleUpLeft",
        "MageMonster_IdleUp",
        "MageMonster_IdleUpRight",
        "MageMonster_IdleRight",
        "MageMonster_IdleDownRight"
    };

    for (int i = 0; i < 8; ++i)
    {
        CreateAnimation(AnimationNames[i]);
        SetAnimationTextureType(AnimationNames[i], EAnimationTextureType::SpriteSheet);
        SetTexture(AnimationNames[i], "MageMonster_IdleSprite", TEXT("Texture\\Monster\\MageMonster_Idle.png"));

        for (int j = 0; j < 6; ++j)
        {
            AddFrame(AnimationNames[i], j * 256.f, i * 256.f, 256.f, 256.f);
        }
    }

    AnimationNames.clear();

    AnimationNames =
    {
        "MageMonster_AttackDown",
        "MageMonster_AttackDownLeft",
        "MageMonster_AttackLeft",
        "MageMonster_AttackUpLeft",
        "MageMonster_AttackUp",
        "MageMonster_AttackUpRight",
        "MageMonster_AttackRight",
        "MageMonster_AttackDownRight"
    };

    for (int i = 0; i < 8; ++i)
    {
        CreateAnimation(AnimationNames[i]);
        SetAnimationTextureType(AnimationNames[i], EAnimationTextureType::SpriteSheet);
        SetTexture(AnimationNames[i], "MageMonster_AttackSprite", TEXT("Texture\\Monster\\MageMonster_Attack.png"));

        for (int j = 0; j < 6; ++j)
        {
            AddFrame(AnimationNames[i], j * 256.f, i * 256.f, 256.f, 256.f);
        }
    }

    CreateAnimation("NormalMonster_Death");
    SetAnimationTextureType("NormalMonster_Death", EAnimationTextureType::SpriteSheet);
    SetTexture("NormalMonster_Death", "NormalMonster_DeathSprite", TEXT("Texture\\Monster\\NormalMonster_Death.png"));

    for (int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            AddFrame("NormalMonster_Death", j * 516.f, i * 516.f, 516.f, 516.f);
        }
    }
    
}

void CAnimation2DManager::BossAnimationInit()
{
    // 보스 상태 애니메이션
    {
        std::vector<std::string> AnimationNames =
        {
            "NueBossMonster_Ready",
            "NueBossMonster_Idle",
            "NueBossMonster_Swing",
            "NueBossMonster_SpecialPatternReady",
            "NueBossMonster_SpecialPatternReady2",
            "NueBossMonster_SpecialPattern",
            "NueBossMonster_SpecialPatternWait",
            "NueBossMonster_Grogy",
            "NueBossMonster_Death",
        };

        // 시작 위치와 프레임 개수 
        // 각 방향별 첫 프레임 인덱스

        // 0 Ready          / 처음 등장 애니메이션
        // 1 Idle           / 보스 기본 Idle
        // 2 Swing          / 기본 공격 애니메이션
        
        // 3 SpecialPatternReady / 특수 패턴 전 Ready 애니메이션
        // 4 SpecialPatternReady2 / 특수 패턴 전 Ready 애니메이션
        // 5 SpecialPattern / 특수패턴 애니메이션
        // 6 SpecialPatternWait   / 기본 패턴전 Ready 애니메이션
        
        // 7 Grogy          / 그로기 애니메이션
        // 8 Death          / 죽는 애니메이션
        
        
        // 시작 인덱스
        int startIndices[9] = { 0, 1, 2, 0, 2, 0, 1, 0, 3};// 0, 0, 1, 0, 0 };
        // 0, 1, 2, 3, ok
        // 시작 줄
        int rowStart[9] = { 0, 1, 2, 0, 0, 2, 3, 0, 1}; // 0, 2, 3, 0, 2 };

        // 각 프레임 개수
        int framePerRow[9] = { 5, 5, 6, 8, 6, 5, 3, 7, 7 };// 8, 5, 3, 7, 6 };

        std::string SpriteName = "Boss_Pattern1";
        const TCHAR* TextureName = TEXT("Texture\\Monster\\Boss\\BossPattern1.png");

        for (int Pattern = 0; Pattern < 9; ++Pattern)
        {
            CreateAnimation(AnimationNames[Pattern]);
            SetAnimationTextureType(AnimationNames[Pattern], EAnimationTextureType::SpriteSheet);

            if (Pattern == 3)
            {
                SpriteName = "Boss_Pattern2";
                TextureName = TEXT("Texture\\Monster\\Boss\\BossPattern2.png");
            }
            else if (Pattern == 7)
            {
                SpriteName = "Boss_Pattern3";
                TextureName = TEXT("Texture\\Monster\\Boss\\BossPattern3.png");
            }


            SetTexture(AnimationNames[Pattern], SpriteName, TextureName);

            int frameCount = 0; // 프레임 개수 카운트

            for (int i = rowStart[Pattern]; i < 4; ++i)
            {
                for (int j = (i == rowStart[Pattern] ? startIndices[Pattern] : 0); j < 4; ++j)
                {
                    if (Pattern == 8 && frameCount == 4)
                    {
                        ++frameCount;
                        continue;
                    }

                    AddFrame(AnimationNames[Pattern], j * 512.f, i * 512.f, 512.f, 512.f);
                    ++frameCount;



                    if (frameCount >= framePerRow[Pattern])
                        break;
                }

                if (frameCount >= framePerRow[Pattern])
                {
                    break;
                }

            }
        }

       
    }


    CreateAnimation("NueBoss_AttackBall");
    SetAnimationTextureType("NueBoss_AttackBall",
        EAnimationTextureType::SpriteSheet);

    SetTexture("NueBoss_AttackBall", "NueBoss_AttackSprite",
        TEXT("Texture\\Monster\\Boss\\Boss_AttackBall.png"));

    for (int i = 0; i < 4; ++i)
        AddFrame("NueBoss_AttackBall", i * 256.f, 0.f, 256.f, 256.f);
}

void CAnimation2DManager::NPCAnimationInit()
{
    // BigOguNPC Animation
    {
        CreateAnimation("BigOguNPC");
        SetAnimationTextureType("BigOguNPC",
            EAnimationTextureType::SpriteSheet);
        SetTexture("BigOguNPC", "BigOguNPC",
            TEXT("Texture\\NPC\\BigOguNPC.png"));

        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 5; ++j)
            {
                AddFrame("BigOguNPC", j * 360.f, i * 360.f, 410.f, 410.f);
            }
        }
    }

    // ShopNPC Animation
    {
        CreateAnimation("ShopNPC");
        SetAnimationTextureType("ShopNPC",
            EAnimationTextureType::SpriteSheet);
        SetTexture("ShopNPC", "NPC1Sprite",
            TEXT("Texture\\NPC\\NPC1.png"));

       
        for (int i = 0; i < 6; ++i)
        {
            AddFrame("ShopNPC", i * 256.f, 0.f, 256.f, 256.f);
        }
        
    }

    // QuestMark Animation
    {
        CreateAnimation("QuestMark1");
        SetAnimationTextureType("QuestMark1",
            EAnimationTextureType::SpriteSheet);
        SetTexture("QuestMark1", "QuestMark1",
            TEXT("Texture\\UI\\QuestMark.png"));

        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 4; ++j)
                AddFrame("QuestMark1", j * 256.f, i * 256.f, 256.f, 256.f);
        }

        CreateAnimation("QuestMark2");
        SetAnimationTextureType("QuestMark2",
            EAnimationTextureType::SpriteSheet);
        SetTexture("QuestMark2", "QuestMark2",
            TEXT("Texture\\UI\\QuestMark.png"));

        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 4; ++j)
                AddFrame("QuestMark2", j * 256.f, 512.f + i * 256.f, 256.f, 256.f);
        }
    }
   
}

bool CAnimation2DManager::CreateAnimation(
    const std::string& Name)
{
    if (FindAnimation(Name))
        return false;

    CAnimation2DData* Animation = new CAnimation2DData;

    Animation->SetName(Name);

    mAnimationMap.insert(std::make_pair(Name, Animation));

    return true;
}

bool CAnimation2DManager::SetAnimationTextureType(
    const std::string& Name, EAnimationTextureType Type)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->SetAnimationTextureType(Type);

    return true;
}

bool CAnimation2DManager::SetTexture(
    const std::string& Name, 
    CTexture* Texture)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->SetTexture(Texture);

    return true;
}

bool CAnimation2DManager::SetTexture(const std::string& Name, 
    const std::string& TextureName)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->SetTexture(TextureName);

    return true;
}

bool CAnimation2DManager::SetTexture(
    const std::string& Name, 
    const std::string& TextureName, const TCHAR* FileName)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->SetTexture(TextureName, FileName);

    return true;
}

bool CAnimation2DManager::SetTexture(
    const std::string& Name,
    const std::string& TextureName,
    std::vector<const TCHAR*> FileName)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->SetTexture(TextureName, FileName);

    return true;
}

bool CAnimation2DManager::SetTexture(
    const std::string& Name, 
    const std::string& TextureName, const TCHAR* FileName,
    const TCHAR* Ext, int Count)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->SetTexture(TextureName, FileName,
        Ext, Count);

    return true;
}

bool CAnimation2DManager::SetTextureFullPath(
    const std::string& Name, 
    const std::string& TextureName, const TCHAR* FullPath)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->SetTextureFullPath(TextureName, FullPath);

    return true;
}

bool CAnimation2DManager::SetTextureFullPath(
    const std::string& Name, 
    const std::string& TextureName,
    std::vector<const TCHAR*> FullPath)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->SetTextureFullPath(TextureName, FullPath);

    return true;
}

bool CAnimation2DManager::AddFrame(const std::string& Name,
    const FVector2D& Start, const FVector2D& Size)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->AddFrame(Start, Size);

    return true;
}

bool CAnimation2DManager::AddFrame(const std::string& Name,
    float StartX, float StartY, float SizeX, float SizeY)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->AddFrame(StartX, StartY, SizeX, SizeY);

    return true;
}

bool CAnimation2DManager::AddFrameCount(
    const std::string& Name, int Count, 
    const FVector2D& Start, const FVector2D& Size)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->AddFrameCount(Count, Start, Size);

    return true;
}

bool CAnimation2DManager::AddFrameCount(
    const std::string& Name, int Count,
    float StartX, float StartY, float SizeX, float SizeY)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->AddFrameCount(Count,
        StartX, StartY, SizeX, SizeY);

    return true;
}

CAnimation2DData* CAnimation2DManager::FindAnimation(
    const std::string& Name)
{
    auto    iter = mAnimationMap.find(Name);

    if (iter == mAnimationMap.end())
        return nullptr;

    return iter->second;
}

void CAnimation2DManager::ReleaseAnimation(
    CAsset* Animation)
{
    auto    iter = mAnimationMap.find(Animation->GetName());

    if (iter != mAnimationMap.end())
    {
        // 다른곳에서 가지고 있는게 없기 때문에 제거한다.
        if (iter->second->GetRefCount() == 1)
        {
            mAnimationMap.erase(iter);
        }
    }
}
