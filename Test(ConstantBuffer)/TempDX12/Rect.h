#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CBuffer_RectCol;
class CShader;
class CRect :
    public CGameObject
{
private:
    CRect(ID3D12Device* pGraphic_Device);
    CRect(const CRect& rhs);
    virtual ~CRect() = default;
public:
	virtual HRESULT							Ready_Prototype();
	virtual HRESULT							Ready_GameObject();
	virtual _int							Update_GameObject(const _float& fTimeDelta);
	virtual _int							LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void							Render_GameObject();
public:
	virtual HRESULT							CreatePipeLine(CShader* pShader);
	virtual D3D12_RASTERIZER_DESC			CreateRaterizerState();
	virtual D3D12_DEPTH_STENCIL_DESC		CreateDepthStencilState();
	virtual D3D12_BLEND_DESC				CreateBlendState();
	virtual D3D12_INPUT_LAYOUT_DESC			CreateInputLayout();
private:
	HRESULT									CreateConstantBuffer();
public:
	static CRect*							Create(ID3D12Device* pGraphic_Device);
	virtual CGameObject*					Clone_GameObject() override;
private:
	virtual void							Free();
	HRESULT									Ready_Component();
private:
	CTransform*								m_pTransformCom = nullptr;
	CRenderer*								m_pRendererCom = nullptr;
	CBuffer_RectCol*						m_pBufferCom = nullptr;
	CShader*								m_pShaderCom = nullptr;
private:
	ID3D12Resource*							m_pConstBuffer = nullptr;
	void*									m_pTemp = nullptr;
	_int									m_iPassSize = 0;
};

