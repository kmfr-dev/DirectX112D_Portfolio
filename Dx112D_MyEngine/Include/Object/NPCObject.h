#pragma once
#include "SceneObject.h"
class CNPCObject abstract :
    public CSceneObject
{
    friend class CScene;

protected:
    CNPCObject();
    CNPCObject(const CNPCObject& Obj);
    CNPCObject(CNPCObject&& Obj);
    virtual ~CNPCObject();

protected:
    CSharedPtr<class CTileMapObj>           mTileMap;
    CSharedPtr<class CSpriteComponent>      mRoot;
    CSharedPtr<class CColliderSphere2D>        mBody;

    CSharedPtr<class CSpriteComponent>      mQuestMark;
    class CAnimation2D* mQuestMarkAnim = nullptr;

    // NPC 이름과 NPCID
    int                 mNPCID = -1;
    std::string         mNPCName;
    // NPC 현재 대사 스크립트 아이디
    int                 mCurrentScriptID = -1;
    int                 mLastScriptID;
    // 퀘스트 ID 목록
    std::list<int> mQuestIDList;

    class CAnimation2D* mAnimation = nullptr;

public:
    const std::string& GetNPCName() const { return mNPCName; }
    int     GetNPCID() const { return mNPCID; }
    int     GetCurrentScriptID()  { return mCurrentScriptID; }
    int     GetLastScriptID() { return mLastScriptID; }
    const std::list<int>& GetQuestIDList() const { return mQuestIDList; }
    int     GetStartScriptID() const;

public:
    void    SetLastScriptID(int ID) { mLastScriptID = ID; }
    void    SetCurrentScriptID(int ID) 
    { 
        mLastScriptID = mCurrentScriptID; // 이전 값 저장
        mCurrentScriptID = ID;
    }
    void    SetQuestMarkEnable(bool Flag);
    void    SetNPCID(int ID) { mNPCID = ID; }
    void    SetTileMap(CSharedPtr<class CTileMapObj> Map);
    void    AddQuestID(int ID);
    void    RemoveQuestID(int ID) { mQuestIDList.remove(ID); }

public:
    void    ChangQuesteAnimation(const std::string& Name);

public:
    virtual bool    Init();
    virtual void    Update(float DeltaTime);
    virtual void    Render();

private:
    void CollisionNPCBegin(const FVector3D& HitPoint,
        class CColliderBase* Dest);
    void CollisionNPCEnd(class CColliderBase* Dest);
};

