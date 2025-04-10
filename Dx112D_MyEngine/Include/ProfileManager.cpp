#include "ProfileManager.h"

DEFINITION_SINGLE(CProfileManager)

CProfileManager::CProfileManager()
{
}

CProfileManager::~CProfileManager()
{
	auto	iter = mProfileMap.begin();
	auto	iterEnd = mProfileMap.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CProfileManager::Init()
{
	CreateProfile("Default", ECollisionChannel::Default, true,
		ECollisionInteraction::Collision);
	CreateProfile("Wall", ECollisionChannel::Wall, true,
		ECollisionInteraction::Collision);

	// 플레이어
	{
		CreateProfile("Player", ECollisionChannel::Player, true,
			ECollisionInteraction::Collision);
		SetInteraction("Player", ECollisionChannel::Player,
			ECollisionInteraction::Ignore);
		SetInteraction("Player", ECollisionChannel::PlayerAttack,
			ECollisionInteraction::Ignore);
	}

	// 플레이어 공격
	{
		CreateProfile("PlayerAttack", ECollisionChannel::PlayerAttack, true,
			ECollisionInteraction::Ignore);

		SetInteraction("PlayerAttack", ECollisionChannel::Default,
			ECollisionInteraction::Collision);
		SetInteraction("PlayerAttack", ECollisionChannel::Monster,
			ECollisionInteraction::Collision);
		SetInteraction("PlayerAttack", ECollisionChannel::BossMonster,
			ECollisionInteraction::Collision);
		SetInteraction("PlayerAttack", ECollisionChannel::BossAttack,
			ECollisionInteraction::Collision);
	}

	// 몬스터
	{
		CreateProfile("Monster", ECollisionChannel::Monster, true,
			ECollisionInteraction::Collision);
		SetInteraction("Monster", ECollisionChannel::Monster,
			ECollisionInteraction::Ignore);
		SetInteraction("Monster", ECollisionChannel::MonsterAttack,
			ECollisionInteraction::Ignore);
	}

	// 몬스터 공격
	{
		CreateProfile("MonsterAttack", ECollisionChannel::MonsterAttack, true,
			ECollisionInteraction::Ignore);
		SetInteraction("MonsterAttack", ECollisionChannel::Default,
			ECollisionInteraction::Collision);
		SetInteraction("MonsterAttack", ECollisionChannel::Player,
			ECollisionInteraction::Collision);

		// 몬스터 범위 충돌체 설정
		CreateProfile("MonsterDetect", ECollisionChannel::MonsterDetect, true,
			ECollisionInteraction::Ignore);
		SetInteraction("MonsterDetect", ECollisionChannel::Player,
			ECollisionInteraction::Collision);
	}

	// 보스 몬스터
	{
		CreateProfile("BossMonster", ECollisionChannel::BossMonster, true,
			ECollisionInteraction::Ignore);

		SetInteraction("BossMonster", ECollisionChannel::Player,
			ECollisionInteraction::Collision);
		SetInteraction("BossMonster", ECollisionChannel::PlayerAttack,
			ECollisionInteraction::Collision);
		SetInteraction("BossMonster", ECollisionChannel::BossAttack,
			ECollisionInteraction::Collision);
	}
	
	// 보스 공격
	{
		CreateProfile("BossAttack", ECollisionChannel::BossAttack, true,
			ECollisionInteraction::Ignore);

		SetInteraction("BossAttack", ECollisionChannel::Player,
			ECollisionInteraction::Collision);
		SetInteraction("BossAttack", ECollisionChannel::PlayerAttack,
			ECollisionInteraction::Collision);
		SetInteraction("BossAttack", ECollisionChannel::BossMonster,
			ECollisionInteraction::Ignore);
	}

	// NPC
	{
		CreateProfile("NPC", ECollisionChannel::NPC, true,
			ECollisionInteraction::Ignore);
		CreateProfile("NPCInterDetect", ECollisionChannel::NPC, true,
			ECollisionInteraction::Ignore);

		SetInteraction("NPC", ECollisionChannel::Player,
			ECollisionInteraction::Collision);
		SetInteraction("NPCInterDetect", ECollisionChannel::Player,
			ECollisionInteraction::Collision);
	}
	
	// 포탈
	{
		CreateProfile("Portal", ECollisionChannel::Portal, true,
			ECollisionInteraction::Ignore);
		SetInteraction("Portal", ECollisionChannel::Player,
			ECollisionInteraction::Collision);
	}

	return true;
}

bool CProfileManager::CreateProfile(const std::string& Name,
	ECollisionChannel::Type Channel,
	bool Enable, ECollisionInteraction::Type Interaction)
{
	FCollisionProfile* Profile = FindProfile(Name);

	if (Profile)
		return false;

	Profile = new FCollisionProfile;

	Profile->Name = Name;
	Profile->Channel = Channel;
	Profile->Enable = Enable;

	for (int i = 0; i < ECollisionChannel::End; ++i)
	{
		Profile->Interaction[i] = Interaction;
	}

	mProfileMap.insert(std::make_pair(Name, Profile));

	return true;
}

bool CProfileManager::SetInteraction(const std::string& Name,
	ECollisionChannel::Type Channel,
	ECollisionInteraction::Type Interaction)
{
	FCollisionProfile* Profile = FindProfile(Name);

	if (!Profile)
		return false;

	Profile->Interaction[Channel] = Interaction;

	return true;
}

bool CProfileManager::SetEnable(const std::string& Name,
	bool Enable)
{
	FCollisionProfile* Profile = FindProfile(Name);

	if (!Profile)
		return false;

	Profile->Enable = Enable;

	return true;
}

FCollisionProfile* CProfileManager::FindProfile(
	const std::string& Name)
{
	auto	iter = mProfileMap.find(Name);

	if (iter == mProfileMap.end())
		return nullptr;

	return iter->second;
}
