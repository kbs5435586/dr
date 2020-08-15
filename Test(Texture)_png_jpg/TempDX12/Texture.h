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
    HRESULT                         Ready_Texture(const _tchar* pFilepath, _uint iNum, TEXTURE_TYPE eType);
public:
    static CTexture*                Create(ID3D12Device* pGraphic_Device, const _tchar* pFilepath, _uint iNum=1, 
                                            TEXTURE_TYPE eType = TEXTURE_TYPE_ELSE);
    virtual CComponent*             Clone_Component(void* pArg = nullptr);
public:
    HRESULT                         Create_ShaderResourceView(_uint iNum=0);
    HRESULT                         SetUp_OnShader(ID3D12GraphicsCommandList* pCommandLst);
protected:
    virtual void					Free();
private:
    TEXTURE_TYPE                    m_eType = TEXTURE_TYPE_END;
    _uint                           m_iTexuterIdx = 0;
private:
    vector< ID3D12Resource*>        m_vecTexture;
    vector< ID3D12Resource*>        m_vecTextureUpload;
};

