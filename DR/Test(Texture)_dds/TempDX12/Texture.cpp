#include "framework.h"
#include "Texture.h"
#include "DDSTextureLoader.h"

CTexture::CTexture(ID3D12Device* pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
	, m_pTexture(rhs.m_pTexture)
	, m_pTextureUpLoadHeap(rhs.m_pTextureUpLoadHeap)
{

}

HRESULT CTexture::Ready_Texture(const _tchar* pFilepath)
{
	CDevice::GetInstance()->Open();
	if (FAILED(CreateDDSTextureFromFile12(CDevice::GetInstance()->GetDevice(),
		CDevice::GetInstance()->GetCommandList(), pFilepath, m_pTexture, m_pTextureUpLoadHeap)))
	{
		return E_FAIL;
	}
	CDevice::GetInstance()->Close();
	CDevice::GetInstance()->WaitForGpuComplete();
	return S_OK;
}

HRESULT CTexture::SetUp_OnShader(ID3D12PipelineState* pPipeline)
{
	return S_OK;
}

CTexture* CTexture::Create(ID3D12Device* pGraphic_Device, const _tchar* pFilepath)
{
	CTexture* pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->Ready_Texture(pFilepath)))
	{
		MessageBox(0, L"CTexture Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CTexture::Clone_Component(void* pArg)
{
	return new CTexture(*this);
}

void CTexture::Free()
{
	if (m_pTexture)
		m_pTexture->Release();
	if (m_pTextureUpLoadHeap)
		m_pTextureUpLoadHeap->Release();
	CComponent::Free();
}
