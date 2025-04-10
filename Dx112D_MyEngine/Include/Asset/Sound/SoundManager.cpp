#include "SoundManager.h"

CSoundManager::CSoundManager()
{
}

CSoundManager::~CSoundManager()
{
	mSoundMap.clear();

	auto	iter = mChannelGroupMap.begin();
	auto	iterEnd = mChannelGroupMap.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->release();
	}

	if (mSystem)
	{
		mSystem->release();
		mSystem->close();
	}
}

bool CSoundManager::Init()
{
	// FMOD System객체를 생성한다.
	FMOD_RESULT result = FMOD::System_Create(&mSystem);

	if (result != FMOD_OK)
		return false;

	result = mSystem->init(256, FMOD_INIT_NORMAL, nullptr);

	if (result != FMOD_OK)
		return false;

	result = mSystem->getMasterChannelGroup(&mMasterGroup);

	if (result != FMOD_OK)
		return false;

	mChannelGroupMap.insert(std::make_pair("Master", mMasterGroup));

	CreateChannelGroup("BGM");
	CreateChannelGroup("Effect");
	CreateChannelGroup("SFX");
	CreateChannelGroup("UI");

	LoadSound("Scene1BGM", "BGM", true, "Sound\\BGM\\Scene1_BGM.wav");
	LoadSound("Scene2BGM", "BGM", true, "Sound\\BGM\\Scene2_BGM.wav");
	LoadSound("SceneBossBGM", "BGM", true, "Sound\\BGM\\SceneBoss_BGM.wav");

	LoadSound("AddItem", "UI", false, "Sound\\SFX\\ItemAdd.wav");
	LoadSound("QuickSlotAdd", "SFX", false, "Sound\\SFX\\UI_QuickSlotAdd.wav");

	LoadSound("UIOpen", "UI", false, "Sound\\SFX\\UI_Open.wav");
	LoadSound("UIClose", "UI", false, "Sound\\SFX\\UI_Close.wav");

	LoadSound("PlayerEat", "SFX", false, "Sound\\SFX\\Player_Eat.wav");
	LoadSound("PlayerWalk", "SFX", true, "Sound\\SFX\\Player_Walk.wav");
	LoadSound("PlayerHit", "SFX", false, "Sound\\SFX\\Player_Hit.wav");
	LoadSound("PlayerAttack", "SFX", false, "Sound\\SFX\\Player_Attack.wav");
	
	LoadSound("NueBossAttack", "SFX", false, "Sound\\SFX\\NueBoss_Attack.wav");
	LoadSound("NueBossHit", "SFX", false, "Sound\\SFX\\NueBoss_Hit.wav");

	LoadSound("NueBossDeath", "SFX", false, "Sound\\SFX\\NueBoss_Death.wav");
	LoadSound("NueBossWait", "SFX", false, "Sound\\SFX\\NueBoss_Soar.wav");
	LoadSound("NueBossPattern", "SFX", false, "Sound\\SFX\\NueBoss_Fly.wav");

	LoadSound("BulletHit", "SFX", false, "Sound\\SFX\\Bullet_Hit.wav");

	
	SetGroupVolume("BGM", 10);
	SetGroupVolume("Effect", 5);
	SetGroupVolume("SFX", 10);
	SetGroupVolume("UI", 10);

	return true;
}

void CSoundManager::Update()
{
	// 오디오 시스템 갱신. 매 프레임마다 실행.
	mSystem->update();
}

bool CSoundManager::CreateChannelGroup(const std::string& Name)
{
	FMOD::ChannelGroup* Group = FindChannelGroup(Name);

	if (Group)
		return true;

	FMOD_RESULT result = mSystem->createChannelGroup(Name.c_str(),
		&Group);

	if (result != FMOD_OK)
		return false;

	// 생성된 채널을 마스터그룹에 등록한다.
	mMasterGroup->addGroup(Group, false);

	mChannelGroupMap.insert(std::make_pair(Name, Group));

	return true;
}

bool CSoundManager::LoadSound(const std::string& Name, 
	const std::string& GroupName, bool Loop, const char* FileName)
{
	CSound* Sound = FindSound(Name);

	if (Sound)
		return true;

	FMOD::ChannelGroup* Group = FindChannelGroup(GroupName);

	if (!Group)
		Group = mMasterGroup;

	Sound = new CSound;

	if (!Sound->LoadSound(mSystem, Group, Loop, FileName))
	{
		SAFE_DELETE(Sound);
		return false;
	}

	mSoundMap.insert(std::make_pair(Name, Sound));

	return true;
}

bool CSoundManager::LoadSoundFullPath(const std::string& Name,
	const std::string& GroupName, bool Loop, const char* FullPath)
{
	CSound* Sound = FindSound(Name);

	if (Sound)
		return true;

	FMOD::ChannelGroup* Group = FindChannelGroup(GroupName);

	if (!Group)
		Group = mMasterGroup;

	Sound = new CSound;

	if (!Sound->LoadSoundFullPath(mSystem, Group, Loop, FullPath))
	{
		SAFE_DELETE(Sound);
		return false;
	}

	mSoundMap.insert(std::make_pair(Name, Sound));

	return true;
}

void CSoundManager::Play(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return;

	Sound->Play();
}

void CSoundManager::Stop(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return;

	Sound->Stop();
}

void CSoundManager::Pause(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return;

	Sound->Pause();
}

void CSoundManager::Resume(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return;

	Sound->Resume();
}

void CSoundManager::SetMasterVolume(int Volume)
{
	Volume = Clamp(Volume, 0, 100);

	mMasterGroup->setVolume(Volume / 100.f);
}

void CSoundManager::SetGroupVolume(const std::string& GroupName, int Volume)
{
	FMOD::ChannelGroup* Group = FindChannelGroup(GroupName);

	if (!Group)
		return;

	Volume = Clamp(Volume, 0, 100);

	Group->setVolume(Volume / 100.f);
}

FMOD::ChannelGroup* CSoundManager::FindChannelGroup(const std::string& Name)
{
	auto	iter = mChannelGroupMap.find(Name);

	if (iter == mChannelGroupMap.end())
		return nullptr;

	return iter->second;
}

CSound* CSoundManager::FindSound(const std::string& Name)
{
	auto	iter = mSoundMap.find(Name);

	if (iter == mSoundMap.end())
		return nullptr;

	return iter->second;
}

void CSoundManager::ReleaseSound(CAsset* Sound)
{
	auto    iter = mSoundMap.find(Sound->GetName());

	if (iter != mSoundMap.end())
	{
		// 다른곳에서 가지고 있는게 없기 때문에 제거한다.
		if (iter->second->GetRefCount() == 1)
		{
			mSoundMap.erase(iter);
		}
	}
}
