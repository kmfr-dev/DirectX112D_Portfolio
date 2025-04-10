#pragma once
#include "ObjectData.h"

class CNPCData :
    public CObjectData
{
public:
	CNPCData();
	CNPCData(const CNPCData& Data);
	CNPCData(CNPCData&& Data);
	virtual ~CNPCData();

public:
	std::list<int> mQuestIDList;
public:
	virtual bool LoadFromFile(const char* FileName) override;
	virtual bool SaveToFile(const char* FileName) override;
};

