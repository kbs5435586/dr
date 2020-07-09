#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CBuffer_TriCol;
class CRect :
    public CGameObject
{
private:
    CRect(ID3D12Device* pGraphic_Device);
    CRect(const CRect& rhs);
    virtual ~CRect() = default;
public:
	virtual HRESULT			Ready_Prototype();
	virtual HRESULT			Ready_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual _int			LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void			Render_GameObject();
public:
	static CRect*			Create(ID3D12Device* pGraphic_Device);
	virtual CGameObject*	Clone_GameObject() override;
private:
	virtual void			Free();
	HRESULT					Ready_Component();
private:
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CBuffer_TriCol*			m_pBufferCom = nullptr;
};

