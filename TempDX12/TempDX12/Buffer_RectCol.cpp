#include "framework.h"
#include "Buffer_RectCol.h"

CBuffer_RectCol::CBuffer_RectCol(ID3D12Device* pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CBuffer_RectCol::CBuffer_RectCol(const CBuffer_RectCol& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CBuffer_RectCol::Ready_VIBuffer()
{
	m_iVertices = 8;
	m_iStride = sizeof(VTXCOL);
	m_PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	VTXCOL pVertices[8];
	pVertices[0] = VTXCOL(XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.f));
	pVertices[1] = VTXCOL(XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.f));
	pVertices[2] = VTXCOL(XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.f));
	pVertices[3] = VTXCOL(XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.f));
	pVertices[4] = VTXCOL(XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.f));
	pVertices[5] = VTXCOL(XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.f));
	pVertices[6] = VTXCOL(XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.f));
	pVertices[7] = VTXCOL(XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.f));


	m_iIndices = 36;

	_uint	indicies[36]{};
	indicies[0] = 1; indicies[1] = 5; indicies[2] = 6;
	indicies[3] = 1; indicies[4] = 6; indicies[5] = 2;

	indicies[6] = 4; indicies[7] = 0; indicies[8] = 3;
	indicies[9] = 4; indicies[10] = 3; indicies[11] = 7;

	indicies[12] = 4; indicies[13] = 5; indicies[14] = 1;
	indicies[15] = 4; indicies[16] = 1; indicies[17] = 0;

	indicies[18] = 3; indicies[19] = 2; indicies[20] = 6;
	indicies[21] = 3; indicies[22] = 6; indicies[23] = 7;

	indicies[24] = 7; indicies[25] = 6; indicies[26] = 5;
	indicies[27] = 7; indicies[28] = 5; indicies[29] = 4;

	indicies[30] = 0; indicies[31] = 1; indicies[32] = 2;
	indicies[33] = 0; indicies[34] = 2; indicies[35] = 3;

	{
		UINT8* pVertexDataBegin;
		if (FAILED(m_pGraphic_Device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,&CD3DX12_RESOURCE_DESC::Buffer(sizeof(pVertices)),D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,IID_PPV_ARGS(&m_pVertexBuffer))))
			return E_FAIL;

		CD3DX12_RANGE readRange(0, 0);
		if (FAILED(m_pVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin))))
			return E_FAIL;
		memcpy(pVertexDataBegin, pVertices, sizeof(pVertices));
		m_pVertexBuffer->Unmap(0, nullptr);
	}


	{
		UINT8* pIndexDataBegin;
		if (FAILED(m_pGraphic_Device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indicies)),D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,IID_PPV_ARGS(&m_pIndexBuffer))))
			return E_FAIL;

		CD3DX12_RANGE readRange(0, 0);
		if (FAILED(m_pIndexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin))))
			return E_FAIL;
		memcpy(pIndexDataBegin, indicies, sizeof(indicies));
		m_pIndexBuffer->Unmap(0, nullptr);
	}


	m_VertexBufferView.BufferLocation = m_pVertexBuffer->GetGPUVirtualAddress();
	m_VertexBufferView.StrideInBytes = m_iStride;
	m_VertexBufferView.SizeInBytes = m_iStride * m_iVertices;

	m_IndexBufferView.BufferLocation = m_pIndexBuffer->GetGPUVirtualAddress();
	m_IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_IndexBufferView.SizeInBytes = sizeof(_uint) * m_iIndices;

	CDevice::GetInstance()->WaitForGpuComplete();
	return S_OK;
}

void CBuffer_RectCol::Render_VIBuffer()
{
	CDevice::GetInstance()->GetCommandList()->IASetPrimitiveTopology(m_PrimitiveTopology);
	CDevice::GetInstance()->GetCommandList()->IASetVertexBuffers(m_iSlot, 1, &m_VertexBufferView);
	CDevice::GetInstance()->GetCommandList()->IASetIndexBuffer(&m_IndexBufferView);
	CDevice::GetInstance()->GetCommandList()->DrawIndexedInstanced(m_iIndices, 1, 0, 0, 0);
}

CBuffer_RectCol* CBuffer_RectCol::Create(ID3D12Device* pGraphic_Device)
{
	CBuffer_RectCol* pInstance = new CBuffer_RectCol(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer()))
	{
		MessageBox(0, L"CBuffer_RectCol Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CBuffer_RectCol::Clone_Component(void* pArg)
{
	return new CBuffer_RectCol(*this);
}

void CBuffer_RectCol::Free()
{
	CVIBuffer::Free();
}
