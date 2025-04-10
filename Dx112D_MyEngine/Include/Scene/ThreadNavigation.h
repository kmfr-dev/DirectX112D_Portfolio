#pragma once
#include "../Thread/ThreadBase.h"

class CThreadNavigation :
    public CThreadBase
{
public:
    CThreadNavigation();
    ~CThreadNavigation();

private:
    class CNavigation* mNavigation = nullptr;

public:
    virtual void Run();
};

