#pragma once

#include "SceneObject.h"
#include "../Component/TileMapComponent.h"
#include "../Component/TileMapRendererComponent.h"

class CTileMapObj :
    public CSceneObject
{
    friend class CScene;

protected:
    CTileMapObj();
    CTileMapObj(const CTileMapObj& Obj);
    CTileMapObj(CTileMapObj&& Obj);
    virtual ~CTileMapObj();

private:
    CSharedPtr<CTileMapRendererComponent>   mTileMapRenderer;
    CSharedPtr<CTileMapComponent>   mTileMap;
    
public:
    CTileMapComponent* GetTileMap() const
    {
        return mTileMap;
    }
    const FVector2D GetPlayerMinBounds() const
    {
        FVector2D MinBounds;
        MinBounds.x = 0.f;
        MinBounds.y = 0.f;
        return MinBounds;
    }

    const FVector2D& GetPlayerMaxBounds() const
    {
        return mTileMap->GetTileMapSize();
    }

    const FVector2D& GetMinBounds() const
    {
        return mTileMap->GetMinBounds();
    }
    const FVector2D& GetMaxBounds() const
    {
        return mTileMap->GetMaxBounds();
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

    void SetBackTexture(const std::string& Name)
    {
        mTileMapRenderer->SetBackTexture(Name);
    }
    void SetBackTexture(const std::string& Name, 
        const TCHAR* FileName)
    {
        mTileMapRenderer->SetBackTexture(Name, FileName);
    }

    void SetTileTexture(const std::string& Name)
    {
        mTileMapRenderer->SetTileTexture(Name);
    }
    void SetTileTexture(const std::string& Name,
        const TCHAR* FileName)
    {
        mTileMapRenderer->SetTileTexture(Name, FileName);
    }

public:
    void Save(const TCHAR* FullPath);
    void Load(const TCHAR* FullPath);
};

