#pragma once
#include "BulletObject.h"

class CBossBullet :
    public CBulletObject
{
    friend class CScene;
public:
    CBossBullet();
    CBossBullet(const CBossBullet& Obj);
    CBossBullet(CBossBullet&& Obj);
    virtual ~CBossBullet();

private:
    class CAnimation2D* mAnimation = nullptr;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);


    void    ChangeAnimation(const std::string Name);
};

