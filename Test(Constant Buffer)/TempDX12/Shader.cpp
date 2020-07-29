#include "framework.h"
#include "Shader.h"

CShader::CShader(ID3D12Device* pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CShader::CShader(const CShader& rhs)
	: CComponent(rhs)
	, m_pFilePath(rhs.m_pFilePath)
	, m_pPixelShader(rhs.m_pPixelShader)
	, m_pVertexShader(rhs.m_pVertexShader)
	, m_pVSEntryPoint(rhs.m_pVSEntryPoint)
	, m_pPSEntryPoint(rhs.m_pPSEntryPoint)
{

}

HRESULT CShader::Ready_Shader(const _tchar* pFilepath, const char* VSEntry, const char* PSEntry, _uint iFlag)
{
	m_pFilePath = pFilepath;
	m_pVSEntryPoint = VSEntry;
	m_pPSEntryPoint = PSEntry;
	m_iCompileFlags = iFlag;
	return S_OK;
}

HRESULT CShader::Complie_Shader()
{
	ID3DBlob* pErrorBlob;
	if (FAILED(D3DCompileFromFile(m_pFilePath, nullptr, nullptr, m_pVSEntryPoint, "vs_5_1", m_iCompileFlags, 0, &m_pVertexShader, &pErrorBlob)))
	{
		char pTemp[1000] = {};
		strcpy_s(pTemp, (char*)pErrorBlob->GetBufferPointer());
		return E_FAIL;
	}

	if (FAILED(D3DCompileFromFile(m_pFilePath, nullptr, nullptr, m_pPSEntryPoint, "ps_5_1", m_iCompileFlags, 0, &m_pPixelShader, nullptr)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CShader::SetUp_OnShader(ID3D12PipelineState* pPipeline, _matrix matWorld, _matrix matView, _matrix matProj)
{
	// Current SwapChainBuffer Idx
	// Constant Buffer View Adress
	// Struct of Constant Buffer view

	CDevice::GetInstance()->GetCommandList()->SetGraphicsRootSignature(CDevice::GetInstance()->GetRootSignature());
	CDevice::GetInstance()->GetCommandList()->SetPipelineState(pPipeline);

	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&matWorld)));
	CDevice::GetInstance()->GetCommandList()->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);

	XMFLOAT4X4 xmf4x4View;
	XMStoreFloat4x4(&xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&matView)));
	CDevice::GetInstance()->GetCommandList()->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4View, 0);

	XMFLOAT4X4 xmf4x4Proj;
	XMStoreFloat4x4(&xmf4x4Proj, XMMatrixTranspose(XMLoadFloat4x4(&matProj)));
	CDevice::GetInstance()->GetCommandList()->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4Proj, 16);

	////XMMATRIX wvpMat = XMLoadFloat4x4(&cube1WorldMat) * viewMat * projMat
	//XMMATRIX wvpMat = XMLoadFloat4x4(&matWorld) * XMLoadFloat4x4(&matView) * XMLoadFloat4x4(&matProj);
	//CDevice::GetInstance()->GetCommandList()->SetGraphicsRoot32BitConstants(0, 16, &wvpMat, 0);
	return S_OK;
}

HRESULT CShader::SetUp_OnShader(ID3D12PipelineState* pPipeline, _matrix matWorld, _matrix matView, _matrix matProj, MAINPASS& tMain)
{
	CDevice::GetInstance()->GetCommandList()->SetGraphicsRootSignature(CDevice::GetInstance()->GetRootSignature());
	CDevice::GetInstance()->GetCommandList()->SetPipelineState(pPipeline);

	XMMATRIX			xmMatWorld = XMLoadFloat4x4(&matWorld);
	XMMATRIX			xmMatView = XMLoadFloat4x4(&matView);
	XMMATRIX			xmMatProj = XMLoadFloat4x4(&matProj);


	XMStoreFloat4x4(&tMain.matWorld, XMMatrixTranspose(xmMatWorld));
	XMStoreFloat4x4(&tMain.matView, XMMatrixTranspose(xmMatView));
	XMStoreFloat4x4(&tMain.matProj, XMMatrixTranspose(xmMatProj));

	return S_OK;
}


CShader* CShader::Create(ID3D12Device* pGraphic_Device, const _tchar* pFilepath, const char* VSEntry, const char* PSEntry, _uint iFlag)
{
	CShader* pInstance = new CShader(pGraphic_Device);

	if (FAILED(pInstance->Ready_Shader(pFilepath, VSEntry, PSEntry, iFlag)))
	{
		MessageBox(0, L"CShader Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CShader::Clone_Component(void* pArg)
{
	return new CShader(*this);
}

void CShader::Free()
{
	CComponent::Free();
}
