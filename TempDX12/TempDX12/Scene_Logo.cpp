#include "framework.h"
#include "Scene_Logo.h"
#include "Management.h"
#include "Debug_Camera.h"
#include "Rect.h"
CScene_Logo::CScene_Logo(ID3D12Device* pGraphic_Device)
	: CScene(pGraphic_Device)
{

}

HRESULT CScene_Logo::Ready_Scene()
{

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;
	if(FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;
	if(FAILED(Ready_Layer_Tri(L"Layer_Tri")))
		return E_FAIL;

	CDevice::GetInstance()->GetCommandList()->Close();
	return S_OK;
}

_int CScene_Logo::Update_Scene(const _float& fTimeDelta)
{
	return CScene::Update_Scene(fTimeDelta);
}

_int CScene_Logo::LastUpdate_Scene(const _float& fTimeDelta)
{
	return CScene::LastUpdate_Scene(fTimeDelta);
}

void CScene_Logo::Render_Scene()
{
	int a = 10;
}

HRESULT CScene_Logo::Ready_Prototype_GameObject()
{
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	pManagement->AddRef();

	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Back_Logo", CRect::Create(m_pGraphic_Device))))
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CScene_Logo::Ready_Prototype_Component()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	if (FAILED(pManagement->Add_Prototype_Component(SCENE_LOGO, L"Component_Shader_Default", 
		CShader::Create(m_pGraphic_Device, L"../ShaderFiles/Shader_Default.hlsl", "VSMain", "PSMain", 0))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype_Component(SCENE_LOGO, L"Component_Buffer_TriCol", 
		CBuffer_TriCol::Create(m_pGraphic_Device))))
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CScene_Logo::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	pManagement->AddRef();

	CDebug_Camera* pCameraObject = nullptr;
	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Camera_Debug", SCENE_STAGE, pLayerTag, (CGameObject**)&pCameraObject)))
		return E_FAIL;

	CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CAMERADESC));
	CameraDesc.vEye = _vec3(0.f, 20.f, -10.f);
	CameraDesc.vAt = _vec3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _vec3(0.f, 1.f, 0.f);

	PROJDESC		ProjDesc;
	ZeroMemory(&ProjDesc, sizeof(PROJDESC));
	ProjDesc.fFovY = XMConvertToRadians(60.0f);

	ProjDesc.fAspect = _float(WINCX) / WINCY;
	ProjDesc.fNear = 0.2f;
	ProjDesc.fFar = 500.0f;

	if (FAILED(pCameraObject->SetUp_CameraProjDesc(CameraDesc, ProjDesc)))
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CScene_Logo::Ready_Layer_Tri(const _tchar* pLayerTag)
{
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	pManagement->AddRef();

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Back_Logo", SCENE_LOGO, pLayerTag)))
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}



CScene_Logo* CScene_Logo::Create(ID3D12Device* pGraphic_Device)
{
	CScene_Logo* pInstance = new CScene_Logo(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MessageBox(0, L"CScene_Logo Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CScene_Logo::Free()
{
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return;

	pManagement->AddRef();

	pManagement->Clear_Layers(SCENE_LOGO);

	Safe_Release(pManagement);

	CScene::Free();
}
