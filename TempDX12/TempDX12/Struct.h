#pragma once

typedef struct tagVertexCol
{
	XMFLOAT3 vPosition;
	XMFLOAT4 vColor;
	tagVertexCol()
	{

	}
	tagVertexCol(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse)
	{
		vPosition = xmf3Position;
		vColor = xmf4Diffuse;
	}
}VTXCOL;

typedef struct tagCamera_Desc
{
	XMFLOAT3		vEye;
	XMFLOAT3		vAt;
	XMFLOAT3		vAxisY;
}CAMERADESC;

typedef struct tagProjection_Desc
{
	float		fFovY;
	float		fAspect;
	float		fNear;
	float		fFar;
}PROJDESC;
