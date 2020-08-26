#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CBuffer_Terrain;
class CShader;
class CTexture;
class CTexture_Terrain :
    public CGameObject
{
private:
	CTexture_Terrain(ID3D12Device* pGraphic_Device);
	CTexture_Terrain(const CTexture_Terrain& rhs);
	virtual ~CTexture_Terrain() = default;
public:
	virtual HRESULT							Ready_Prototype();
	virtual HRESULT							Ready_GameObject(void* pArg = nullptr);
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
	static CTexture_Terrain*				Create(ID3D12Device* pGraphic_Device);
	virtual CGameObject*					Clone_GameObject(void* pArg = nullptr) override;
private:
	virtual void							Free();
	HRESULT									Ready_Component(void* pArg);
private:
	CTransform*								m_pTransformCom = nullptr;
	CRenderer*								m_pRendererCom = nullptr;
	CBuffer_Terrain*						m_pBufferCom = nullptr;
	CShader*								m_pShaderCom = nullptr;
	CTexture*								m_pTextureCom = nullptr;

};

