#pragma once
#include "Component.h"
class CTexture :
    public CComponent
{
private:
    CTexture(ID3D12Device* pGraphic_Device);
    CTexture(const CTexture& rhs);
    ~CTexture() = default;
public:
    HRESULT                         Ready_Texture(const _tchar* pFilepath);
    HRESULT			                SetUp_OnShader(ID3D12PipelineState* pPipeline);

public:
    static CTexture*                Create(ID3D12Device* pGraphic_Device, const _tchar* pFilepath);
    virtual CComponent*             Clone_Component(void* pArg = nullptr);
protected:
    virtual void					Free();
private:
    ID3D12Resource*                 m_pTexture = nullptr;
    ID3D12Resource*                 m_pTextureUpLoadHeap = nullptr;
public:
    ID3D12Resource*                 GetTexture() { return m_pTexture; }

};

