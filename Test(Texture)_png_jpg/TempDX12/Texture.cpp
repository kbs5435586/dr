#include "framework.h"
#include "Texture.h"
#include "DDSTextureLoader.h"

CTexture::CTexture(ID3D12Device* pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
	, m_vecTextureUpload(rhs.m_vecTextureUpload)
	, m_vecTexture(rhs.m_vecTexture)
{

}

HRESULT CTexture::Ready_Texture(const _tchar* pFilepath, _uint iNum, TEXTURE_TYPE eType)
{
	m_vecTexture.reserve(iNum);
		
	CDevice::GetInstance()->Open();
	if (eType == TEXTURE_TYPE_DDS)
	{
		if (iNum == 0)
		{
			_tchar	szFilePath[MAX_PATH] = L"";
			ID3D12Resource* pTexture;
			ID3D12Resource* pTextureUpload;
			wsprintf(szFilePath, pFilepath, 0);

			if (FAILED(CreateDDSTextureFromFile12(CDevice::GetInstance()->GetDevice(),
				CDevice::GetInstance()->GetCommandList(), szFilePath, pTexture, pTextureUpload)))
			{
				return E_FAIL;
			}
			pTexture->SetName(L"Texture");
			if (pTexture != nullptr)
			{
				m_vecTexture.push_back(pTexture);
				m_vecTextureUpload.push_back(pTextureUpload);
			}

		}
		else
		{
			for (_uint i = 0; i < iNum; ++i)
			{
				_tchar	szFilePath[MAX_PATH] = L"";
				ID3D12Resource* pTexture;
				ID3D12Resource* pTextureUpload;
				wsprintf(szFilePath, pFilepath, i);

				if (FAILED(CreateDDSTextureFromFile12(CDevice::GetInstance()->GetDevice(),
					CDevice::GetInstance()->GetCommandList(), szFilePath, pTexture, pTextureUpload)))
				{
					return E_FAIL;
				}
				pTexture->SetName(L"Texture");
				if (pTexture != nullptr)
				{
					m_vecTexture.push_back(pTexture);
					m_vecTextureUpload.push_back(pTextureUpload);

				}
			}
		}

	}

	CDevice::GetInstance()->Close();
	CDevice::GetInstance()->WaitForGpuComplete();
	return S_OK;
}


CTexture* CTexture::Create(ID3D12Device* pGraphic_Device, const _tchar* pFilepath, _uint iNum, TEXTURE_TYPE eType)
{
	CTexture* pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->Ready_Texture(pFilepath, iNum, eType)))
	{
		MessageBox(0, L"CTexture Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}
HRESULT CTexture::Create_ShaderResourceView(_uint iNum)
{
	m_iTexuterIdx = iNum;
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(
		CDevice::GetInstance()->GetShaderResourceDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = m_vecTexture[iNum]->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = m_vecTexture[iNum]->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	CDevice::GetInstance()->GetDevice()->CreateShaderResourceView(m_vecTexture[iNum],&srvDesc, hDescriptor);
	
	return S_OK;
}
HRESULT CTexture::SetUp_OnShader(ID3D12GraphicsCommandList* pCommandLst)
{
	auto heap = CDevice::GetInstance()->GetShaderResourceDescriptorHeap();
	CD3DX12_GPU_DESCRIPTOR_HANDLE hTexture
	(		
		heap->GetGPUDescriptorHandleForHeapStart());
		hTexture.Offset(0, CDevice::GetInstance()->GetSrvDescriptorSize()
	);
	pCommandLst->SetDescriptorHeaps(1, &heap);
	pCommandLst->SetGraphicsRootDescriptorTable(0, hTexture);
	//pCommandLst->SetGraphicsRootShaderResourceView(0, m_vecTexture[m_iTexuterIdx]->GetGPUVirtualAddress());
	
	return S_OK;
}
CComponent* CTexture::Clone_Component(void* pArg)
{
	return new CTexture(*this);
}

void CTexture::Free()
{
	for (auto& iter : m_vecTexture)
	{
		if (iter)
		{
			iter->Release();
		}
	}
	for (auto& iter : m_vecTextureUpload)
	{
		if (iter)
		{
			iter->Release();
		}
	}
	CComponent::Free();
}
