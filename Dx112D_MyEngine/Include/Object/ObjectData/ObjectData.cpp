#include "ObjectData.h"

CObjectData::CObjectData()
{
}
CObjectData::CObjectData(const CObjectData& Data)
{
}
CObjectData::CObjectData(CObjectData&& Data)
{
}
CObjectData::~CObjectData()
{
}

bool CObjectData::LoadFromFile(const char* FileName)
{
	return true;
}

bool CObjectData::SaveToFile(const char* FileName)
{
	return true;
}
