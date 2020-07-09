#pragma once
#include "Scene.h"
class CScene_Logo :
    public CScene
{
private:
	explicit CScene_Logo(ID3D12Device* pGraphic_Device);
	virtual ~CScene_Logo() = default;
public:
	virtual HRESULT			Ready_Scene() override;
	virtual _int			Update_Scene(const _float & fTimeDelta) override;
	virtual _int			LastUpdate_Scene(const _float & fTimeDelta) override;
	virtual void			Render_Scene() override;
private:
	HRESULT					Ready_Prototype_GameObject();
	HRESULT					Ready_Prototype_Component();
private:
	HRESULT					Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT					Ready_Layer_Tri(const _tchar* pLayerTag);

public:
	static CScene_Logo*		Create(ID3D12Device* pGraphic_Device);
protected:
	virtual void			Free();
};

