#pragma once
#include "VIBuffer.h"
class CBuffer_RectCol :
	public CVIBuffer
{
private:
	CBuffer_RectCol(ID3D12Device* pGraphic_Device);
	CBuffer_RectCol(const CBuffer_RectCol& rhs);
	~CBuffer_RectCol() = default;
public:
	HRESULT					Ready_VIBuffer();
	void					Render_VIBuffer();
public:
	static					CBuffer_RectCol* Create(ID3D12Device* pGraphic_Device);
	virtual CComponent*		Clone_Component(void* pArg = nullptr);
protected:
	virtual void			Free();
};

