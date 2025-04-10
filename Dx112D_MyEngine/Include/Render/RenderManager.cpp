#include "RenderManager.h"
#include "RenderStateManager.h"
#include "RenderState.h"
#include "../Component/SceneComponent.h"
#include "../Device.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneUIManager.h"
#include "../UI/UserWidget/MouseWidget.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()
{
}

CRenderManager::~CRenderManager()
{
	auto	iter = mLayerList.begin();
	auto	iterEnd = mLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	SAFE_RELEASE(mSampler);
	SAFE_DELETE(mStateManager);
}

void CRenderManager::AddRenderList(CSceneComponent* Component)
{
	FRenderLayer* Layer = FindLayer(Component->GetRenderLayerName());

	if (!Layer)
		return;

	Layer->RenderList.emplace_back(Component);
}

void CRenderManager::ChangeLayer(CSceneComponent* Component, 
	const std::string& Name)
{
	// 컴포넌트에 설정한 레이어 이름을 가지고
	// 레이어를 찾아온다.
	FRenderLayer* Layer = FindLayer(Component->GetRenderLayerName());
	FRenderLayer* InsertLayer = FindLayer(Name);

	// 없으면 리턴
	if (!Layer)
		return;

	if (!InsertLayer)
		return;


	// 해당 레이어의 컴포넌트 목록을 순회한다.
	auto iter = Layer->RenderList.begin();
	auto iterEnd = Layer->RenderList.end();

	for (; iter != iterEnd;)
	{
		if ((*iter) == Component)
		{
			Layer->RenderList.erase(iter);
			Component->SetRenderLayerName(Name);
			InsertLayer->RenderList.emplace_back(Component);
			break;
		}
		++iter;
	}
}
 
void CRenderManager::ClearRenderList()
{
	auto	iter = mLayerList.begin();
	auto	iterEnd = mLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->RenderList.clear();
	}
}

void CRenderManager::SetMouseWidget(CWidget* Widget)
{
	mMouseWidget = Widget;

	if (mMouseWidget)
		ShowCursor(FALSE);

	else
		ShowCursor(TRUE);
}

bool CRenderManager::Init()
{
	CreateRenderLayer("BackGround", INT_MIN);
	CreateRenderLayer("Object", 0);

	mStateManager = new CRenderStateManager;

	mStateManager->Init();

	D3D11_SAMPLER_DESC	Desc = {};

	Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	Desc.MipLODBias = 0.f;
	Desc.MaxAnisotropy = 1;
	Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	Desc.MinLOD = -FLT_MAX;
	Desc.MaxLOD = FLT_MAX;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateSamplerState(
		&Desc, &mSampler)))
		return false;

	mAlphaBlend = mStateManager->FindState("AlphaBlend");

	mDepthDisable = mStateManager->FindState("DepthDisable");

	mMouseWidget = CSceneUIManager::CreateWidgetStatic<CMouseWidget>("Mouse");

	ShowCursor(FALSE);

	return true;
}

void CRenderManager::Update(float DeltaTime)
{
	if (mMouseWidget)
		mMouseWidget->Update(DeltaTime);
}

void CRenderManager::Render()
{
	CDevice::GetInst()->GetContext()->PSSetSamplers(0,
		1, &mSampler);

	mAlphaBlend->SetState();

	mDepthDisable->SetState();

	auto	iter = mLayerList.begin();
	auto	iterEnd = mLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		FRenderLayer* Layer = iter->second;

		switch (mRenderSortType)
		{
		case ERenderSortType::None:
			break;
		case ERenderSortType::Y:
			if (Layer->RenderList.size() > 1)
				Layer->RenderList.sort(CRenderManager::SortY);
			break;
		case ERenderSortType::Alpha:
			if (Layer->RenderList.size() > 1)
				Layer->RenderList.sort(CRenderManager::SortAlpha);
			break;
		}

		auto	iter1 = Layer->RenderList.begin();
		auto	iter1End = Layer->RenderList.end();

		for (; iter1 != iter1End;)
		{
			if (!(*iter1)->IsActive())
			{
				iter1 = Layer->RenderList.erase(iter1);
				iter1End = Layer->RenderList.end();
				continue;
			}

			else if (!(*iter1)->IsEnable())
			{
				++iter1;
				continue;
			}

			(*iter1)->PreRender();

			(*iter1)->Render();

			(*iter1)->PostRender();

			++iter1;
		}
	}

	// UI 출력
	CSceneManager::GetInst()->RenderUI();


	// 마우스 출력
	if (mMouseWidget)
		mMouseWidget->Render();

	mDepthDisable->ResetState();

	mAlphaBlend->ResetState();
}

bool CRenderManager::CreateRenderLayer(const std::string& Name, 
	int ZOrder)
{
	FRenderLayer* Layer = FindLayer(Name);

	if (Layer)
		return false;

	Layer = new FRenderLayer;

	Layer->ZOrder = ZOrder;

	mLayerList.insert(std::make_pair(ZOrder, Layer));
	mLayerNameList[Name] = ZOrder;

	return true;
}

FRenderLayer* CRenderManager::FindLayer(const std::string& Name)
{
	auto	iter1 = mLayerNameList.find(Name);

	if (iter1 == mLayerNameList.end())
		return nullptr;

	int	ZOrder = iter1->second;

	auto	iter = mLayerList.find(ZOrder);

	return iter->second;
}

bool CRenderManager::SortY(
	const CSharedPtr<class CSceneComponent>& Src, 
	const CSharedPtr<class CSceneComponent>& Dest)
{
	float SrcY = Src->GetWorldPosition().y -
		Src->GetWorldScale().y * Src->GetPivot().y;

	float DestY = Dest->GetWorldPosition().y -
		Dest->GetWorldScale().y * Dest->GetPivot().y;

	return SrcY > DestY;
}

bool CRenderManager::SortAlpha(
	const CSharedPtr<class CSceneComponent>& Src, 
	const CSharedPtr<class CSceneComponent>& Dest)
{
	return false;
}
