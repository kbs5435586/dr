#include "framework.h"
#include "Rect.h"
#include "Management.h"

CRect::CRect(ID3D12Device* pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CRect::CRect(const CRect& rhs)
	: CGameObject(rhs)
{
}

HRESULT CRect::Ready_Prototype()
{

	return S_OK;
}

HRESULT CRect::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	return S_OK;
}

_int CRect::Update_GameObject(const _float& fTimeDelta)
{
	return _int();
}

_int CRect::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

void CRect::Render_GameObject()
{
	if (nullptr == m_pBufferCom)
		m_pBufferCom->Render_VIBuffer();
}
HRESULT CRect::Ready_Component()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	// For.Com_Transform
	m_pTransformCom = (CTransform*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == m_pTransformCom)
		return E_FAIL;
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	// For.Com_Renderer
	m_pRendererCom = (CRenderer*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (nullptr == m_pRendererCom)
		return E_FAIL;
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	// For.Com_Buffer
	m_pBufferCom = (CBuffer_TriCol*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Buffer_TriCol");
	if (nullptr == m_pBufferCom)
		return E_FAIL;
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

CRect* CRect::Create(ID3D12Device* pGraphic_Device)
{
	CRect* pInstance = new CRect(pGraphic_Device);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		MessageBox(0, L"CBack_Logo Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CRect::Clone_GameObject()
{
	CRect* pInstance = new CRect(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MessageBox(0, L"CBack_Logo Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRect::Free()
{
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);


	CGameObject::Free();
}

