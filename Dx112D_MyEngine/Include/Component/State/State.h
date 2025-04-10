#pragma once

#include "../../GameInfo.h"

class CState abstract
{
    friend class CStateMachineComponent;

protected:
    CState() {}
    virtual ~CState() {}

protected:
    class CSceneObject* mOwner; // 상태를 소유하는 오브젝트

public:
    void   SetOwner(CSceneObject* Owner) { mOwner = Owner; }

    virtual void Start() = 0;
    virtual void Update(float DeltaTime) = 0;
    virtual void End() = 0;
};

