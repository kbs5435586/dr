#pragma once
#include "Camera.h"
class CDebug_Camera :
    public CCamera
{
private:
	explicit CDebug_Camera(ID3D12Device* pGraphic_Device);
	explicit CDebug_Camera(const CDebug_Camera& rhs);
	virtual ~CDebug_Camera() = default;
public:
	virtual HRESULT				Ready_Prototype();
	virtual HRESULT				Ready_GameObject(void* pArg = nullptr);
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void				Render_GameObject();
public:
	static CDebug_Camera*		Create(ID3D12Device* pGraphic_Device);
	virtual CGameObject*		Clone_GameObject(void* pArg = nullptr);
protected:
	virtual void				Free();
private:
	POINT					m_ptMouse;
};

