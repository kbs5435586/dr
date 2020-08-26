#pragma once
#include "Component.h"
class CFBX_Loader :
    public CComponent
{
private:
    CFBX_Loader(ID3D12Device* pGraphic_Device);
    CFBX_Loader(const CFBX_Loader& rhs);
    ~CFBX_Loader() = default;
public:
    HRESULT                 Ready_FBX_Loader();
};

