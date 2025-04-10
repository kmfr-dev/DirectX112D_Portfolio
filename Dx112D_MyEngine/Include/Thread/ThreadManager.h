#pragma once

#include "ThreadBase.h"

class CThreadManager
{
private:
	std::unordered_map<std::string, CThreadBase*> mThreadMap;

public:
	bool Init();

public:
	template<typename T>
	T* FindThread(const std::string& Name)
	{
		auto iter = mThreadMap.find(Name);

		if (iter == mThreadMap.end())
			return nullptr;

		return dynamic_cast<T*>(iter->second);
	}

public:
	template <typename T>
	T* Create(const std::string& Name,
		bool Pause = false)
	{
		T* Thread = FindThread(Name);

		if (Thread)
			return Thread;

		Thread = new T;

		Thread->Init(Pause);
	
		mThreadMap.insert(std::make_pair(Name, Thread));

		return Thread;
	}

	DECLARE_SINGLE(CThreadManager)
};

