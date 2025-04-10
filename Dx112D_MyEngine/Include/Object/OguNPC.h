#pragma once
#include "NPCObject.h"
class COguNPC :
    public CNPCObject
{
    friend class CScene;

protected:
    COguNPC();
    COguNPC(const COguNPC& Obj);
    COguNPC(COguNPC&& Obj);
    virtual ~COguNPC();

public:
    virtual bool    Init();
    virtual void    Update(float DeltaTime);
    virtual void    Render();
};

