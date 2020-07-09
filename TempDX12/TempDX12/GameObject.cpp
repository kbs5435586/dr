#include "framework.h"
#include "GameObject.h"
#include "Component.h"

CGameObject::CGameObject(ID3D12Device* pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pGraphic_Device(rhs.m_pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

HRESULT CGameObject::Ready_GameObject()
{

	return S_OK;
}

_int CGameObject::Update_GameObject(const _float& fTimeDelta)
{
	return _int();
}

_int CGameObject::LastUpdate_GameObject(const _float& fTimeDelta)
{
	return _int();
}

void CGameObject::Render_GameObject()
{
}

HRESULT CGameObject::Add_Component(const _tchar* pComponentTag, CComponent* pComponent)
{
	if (nullptr == pComponent)
		return E_FAIL;

	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	m_mapComponent.insert(MAPCOMPONENT::value_type(pComponentTag, pComponent));

	pComponent->AddRef();

	return S_OK;
}

CComponent* CGameObject::Find_Component(const _tchar* pComponentTag)
{
	auto iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CFinder_Tag(pComponentTag));

	if (iter == m_mapComponent.end())
		return nullptr;

	return iter->second;
}

CGameObject* CGameObject::Clone_GameObject()
{
	return nullptr;
}

void CGameObject::Free()
{
	for (auto& Pair : m_mapComponent)
		Safe_Release(Pair.second);
	m_mapComponent.clear();

	Safe_Release(m_pGraphic_Device);
}
