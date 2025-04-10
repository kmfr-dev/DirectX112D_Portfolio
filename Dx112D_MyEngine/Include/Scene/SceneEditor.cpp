#include "SceneEditor.h"
#include "../Object/TileMapObj.h"
#include "../Object/EditorPlayer.h"
#include "Input.h"
#include "../GameManager.h"

CSceneEditor::CSceneEditor()
{
}

CSceneEditor::~CSceneEditor()
{
}

void CSceneEditor::Update(float DeltaTime)
{
    CScene::Update(DeltaTime);
    // mTileMapObj
    const FVector2D& MousePos =
        mInput->GetMouseWorldPos2D();

    if (mEditorMode == EEditorMode::TileType)
    {
        if (mInput->GetMouseHold(EMouseButtonType::LButton))
        {
            mOnMousePrevTileType = mEditTileType;
            mTileMapObj->GetTileMap()->ChangeTileType(mEditTileType, MousePos);
        }

        else if (!mInput->GetMouseDown(EMouseButtonType::LButton))
        {
            int Index = mTileMapObj->GetTileMap()->GetTileIndex(MousePos);

            if (Index != mOnMousePrevIndex)
            {
                ETileType PrevType =
                    mTileMapObj->GetTileMap()->ChangeTileType(ETileType::MouseOver, MousePos);

                if (PrevType != ETileType::None)
                {
                    if (mOnMousePrevIndex != -1)
                    {
                        mTileMapObj->GetTileMap()->ChangeTileType(mOnMousePrevTileType,
                            mOnMousePrevIndex);
                    }

                    mOnMousePrevIndex = mTileMapObj->GetTileMap()->GetTileIndex(MousePos);
                    mOnMousePrevTileType = PrevType;
                }

                else
                {
                    mTileMapObj->GetTileMap()->ChangeTileType(mOnMousePrevTileType,
                        mOnMousePrevIndex);
                    mOnMousePrevTileType = ETileType::None;
                    mOnMousePrevIndex = -1;
                }
            }
        }
    }

    else if (mEditorMode == EEditorMode::TileImage)
    {
        if (mInput->GetMouseHold(EMouseButtonType::LButton))
            mTileMapObj->GetTileMap()->ChangeTileFrame(mEditTileFrame, MousePos);
    }

    else if (mEditorMode == EEditorMode::Object)
    {
        if (mInput->GetMouseHold(EMouseButtonType::LButton))
        {
            int Index = mTileMapObj->GetTileMap()->GetTileIndex(MousePos);
            mTileMapObj->GetTileMap()->ChangeTileType(ETileType::UnableToMove, Index);

            FPlacedObject Obj;
            Obj.ObjectType = "Tree";
            Obj.TileIndex = Index;

            mPlacedObjectList.push_back(Obj);
        }
    }
}

bool CSceneEditor::InitAsset()
{
    return true;
}

bool CSceneEditor::InitObject()
{
    CEditorPlayer* EditorPlayer = CreateObj<CEditorPlayer>("EditorPlayer");

    mTileMapObj = CreateObj<CTileMapObj>("Map1");

    mTileMapObj->SetBackTexture("Map3Back",
        TEXT("Texture\\Map\\Map3BackGround.png"));
    mTileMapObj->SetTileTexture("Tile",
        TEXT("Texture\\Floors.png"));

    mInput->AddBindKey("EditorMode", '1');

    mInput->AddBindFunction<CSceneEditor>("EditorMode",
        EInputType::Down, this, &CSceneEditor::EditorMode);

    mInput->AddBindKey("TileType", '2');

    mInput->AddBindFunction<CSceneEditor>("TileType",
        EInputType::Down, this, &CSceneEditor::TileTypeKey);

    mInput->AddBindKey("TileFrame", '3');

    mInput->AddBindFunction<CSceneEditor>("TileFrame",
        EInputType::Down, this, &CSceneEditor::TileFrameKey);

    mInput->AddBindKey("Save", 'S');
    mInput->ChangeKeyCtrl("Save", true);

    mInput->AddBindFunction<CSceneEditor>("Save",
        EInputType::Down, this, &CSceneEditor::SaveKey);

    mInput->AddBindKey("Load", 'O');
    mInput->ChangeKeyCtrl("Load", true);

    mInput->AddBindFunction<CSceneEditor>("Load",
        EInputType::Down, this, &CSceneEditor::LoadKey);

    return true;
}

bool CSceneEditor::InitWidget()
{
    return true;
}

void CSceneEditor::EditorMode(float DeltaTime)
{
    mEditorMode = (EEditorMode)((int)mEditorMode + 1);

    if (mEditorMode == EEditorMode::End)
        mEditorMode = EEditorMode::TileType;
}

void CSceneEditor::TileTypeKey(float DeltaTime)
{
    mEditTileType = (ETileType)((int)mEditTileType + 1);

    if (mEditTileType == ETileType::End)
        mEditTileType = ETileType::Normal;
}

void CSceneEditor::TileFrameKey(float DeltaTime)
{
    mEditTileFrame = (mEditTileFrame + 1) %
        mTileMapObj->GetTileMap()->GetTileFrameCount();
}

void CSceneEditor::SaveKey(float DeltaTime)
{
    OPENFILENAME    ofn = {};

    // 선택한 경로를 저장하기 위한 배열
    TCHAR   FullPath[MAX_PATH] = {};
    TCHAR   Filter[] = TEXT("모든파일\0*.*\0Map 파일\0*.tlm\0");
    
    TCHAR	DefaultPath[MAX_PATH] = {};

    lstrcpy(DefaultPath, gRootPath);
    lstrcat(DefaultPath, TEXT("Asset\\Data\\"));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = CGameManager::GetInst()->GetWindowHandle();
    ofn.lpstrFilter = Filter;
    ofn.lpstrDefExt = TEXT("tlm");
    ofn.lpstrInitialDir = DefaultPath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFile = FullPath;

    ShowCursor(TRUE);

    if (GetSaveFileName(&ofn) != 0)
    {
        mTileMapObj->Save(FullPath);
        SaveObjList("MapObj.dat");
    }

    ShowCursor(FALSE);
}

void CSceneEditor::LoadKey(float DeltaTime)
{
    OPENFILENAME    ofn = {};

    // 선택한 경로를 저장하기 위한 배열
    TCHAR   FullPath[MAX_PATH] = {};
    TCHAR   Filter[] = TEXT("모든파일\0*.*\0Map 파일\0*.tlm\0");

    TCHAR	DefaultPath[MAX_PATH] = {};

    lstrcpy(DefaultPath, gRootPath);
    lstrcat(DefaultPath, TEXT("Asset\\Data\\"));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = CGameManager::GetInst()->GetWindowHandle();
    ofn.lpstrFilter = Filter;
    ofn.lpstrDefExt = TEXT("tlm");
    ofn.lpstrInitialDir = DefaultPath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFile = FullPath;

    ShowCursor(TRUE);

    if (GetOpenFileName(&ofn) != 0)
    {
        mTileMapObj->Load(FullPath);
        LoadObjList("MapObj.dat");
    }

    ShowCursor(FALSE);
}

void CSceneEditor::SaveObjList(const char* FileName)
{
    char FullPath[MAX_PATH] = {};
    strcpy_s(FullPath, gRootPathMultibyte);
    strcat_s(FullPath, "Asset\\Data\\MapObjectData\\");
    strcat_s(FullPath, FileName);

    FILE* File = nullptr;
    fopen_s(&File, FullPath, "wb");

    if (!File)
        return;

    int ObjectCount = static_cast<int>(mPlacedObjectList.size());
    fwrite(&ObjectCount, sizeof(int), 1, File);

    for (const FPlacedObject& Obj : mPlacedObjectList)
    {
        size_t NameLen = Obj.ObjectType.size();
        fwrite(&NameLen, sizeof(size_t), 1, File);
        fwrite(Obj.ObjectType.c_str(), sizeof(char), NameLen, File);
        fwrite(&Obj.TileIndex, sizeof(int), 1, File);
    }

    fclose(File);
}

void CSceneEditor::LoadObjList(const char* FileName)
{
    char FullPath[MAX_PATH] = {};
    strcpy_s(FullPath, gRootPathMultibyte);
    strcat_s(FullPath, "Asset\\Data\\MapObjectData\\");
    strcat_s(FullPath, FileName);

    FILE* File = nullptr;
    fopen_s(&File, FullPath, "rb");

    if (!File)
        return;

    int ObjectCount = 0;
    fread(&ObjectCount, sizeof(int), 1, File);

    mPlacedObjectList.clear();

    for (int i = 0; i < ObjectCount; ++i)
    {
        size_t NameLen = 0;
        fread(&NameLen, sizeof(size_t), 1, File);

        char NameBuf[256] = {};
        fread(NameBuf, sizeof(char), NameLen, File);

        int TileIndex = 0;
        fread(&TileIndex, sizeof(int), 1, File);

        FPlacedObject Obj;
        Obj.ObjectType = std::string(NameBuf, NameLen);
        Obj.TileIndex = TileIndex;

        mPlacedObjectList.push_back(Obj);
    }

    fclose(File);
}

