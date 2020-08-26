#pragma once

typedef struct tagVertexCol
{
	XMFLOAT3 vPosition;
	XMFLOAT4 vColor;
	tagVertexCol()
	{

	}
	tagVertexCol(XMFLOAT3 vPos, XMFLOAT4 vColor_)
	{
		vPosition = vPos;
		vColor = vColor_;
	}
}VTXCOL;

typedef struct tagVertexTex
{
	XMFLOAT3 vPosition;
	XMFLOAT2 vTex;
	tagVertexTex()
	{

	}
	tagVertexTex(XMFLOAT3 vPos, XMFLOAT2 vTex_)
	{
		vPosition = vPos;
		vTex = vTex_;
	}
}VTXTEX;

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


typedef struct tagMainPass
{
	XMMATRIX	matWorld;
	XMMATRIX	matView;
	XMMATRIX	matProj;

}MAINPASS;

struct Keyframe
{
	Keyframe()
	{
		TimePos = 0.f;
		Translation = XMFLOAT3(0.0f, 0.0f, 0.0f);
		Scale= XMFLOAT3(1.0f, 1.0f, 1.0f);
		RotationQuat= XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	~Keyframe()
	{

	}

	float TimePos;
	DirectX::XMFLOAT3 Translation;
	DirectX::XMFLOAT3 Scale;
	DirectX::XMFLOAT4 RotationQuat;

	bool operator == (const Keyframe& key)
	{
		if (Translation.x != key.Translation.x || Translation.y != key.Translation.y || Translation.z != key.Translation.z)
			return false;

		if (Scale.x != key.Scale.x || Scale.y != key.Scale.y || Scale.z != key.Scale.z)
			return false;

		if (RotationQuat.x != key.RotationQuat.x || RotationQuat.y != key.RotationQuat.y || RotationQuat.z != key.RotationQuat.z || RotationQuat.w != key.RotationQuat.w)
			return false;

		return true;
	}
};

struct BoneAnimation
{
	std::vector<Keyframe> Keyframes;

	float GetStartTime()const
	{
		return Keyframes.front().TimePos;
	}
	float GetEndTime()const
	{
		float f = Keyframes.back().TimePos;
		return f;
	}

	void Interpolate(float t, DirectX::XMFLOAT4X4& M)const
	{
		if (t <= Keyframes.front().TimePos)
		{
			XMVECTOR S = XMLoadFloat3(&Keyframes.front().Scale);
			XMVECTOR P = XMLoadFloat3(&Keyframes.front().Translation);
			XMVECTOR Q = XMLoadFloat4(&Keyframes.front().RotationQuat);

			XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
			XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
		}
		else if (t >= Keyframes.back().TimePos)
		{
			XMVECTOR S = XMLoadFloat3(&Keyframes.back().Scale);
			XMVECTOR P = XMLoadFloat3(&Keyframes.back().Translation);
			XMVECTOR Q = XMLoadFloat4(&Keyframes.back().RotationQuat);

			XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
			XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
		}
		else
		{
			for (UINT i = 0; i < Keyframes.size() - 1; ++i)
			{
				if (t >= Keyframes[i].TimePos && t <= Keyframes[i + 1].TimePos)
				{
					float lerpPercent = (t - Keyframes[i].TimePos) / (Keyframes[i + 1].TimePos - Keyframes[i].TimePos);

					XMVECTOR s0 = XMLoadFloat3(&Keyframes[i].Scale);
					XMVECTOR s1 = XMLoadFloat3(&Keyframes[i + 1].Scale);

					XMVECTOR p0 = XMLoadFloat3(&Keyframes[i].Translation);
					XMVECTOR p1 = XMLoadFloat3(&Keyframes[i + 1].Translation);

					XMVECTOR q0 = XMLoadFloat4(&Keyframes[i].RotationQuat);
					XMVECTOR q1 = XMLoadFloat4(&Keyframes[i + 1].RotationQuat);

					XMVECTOR S = XMVectorLerp(s0, s1, lerpPercent);
					XMVECTOR P = XMVectorLerp(p0, p1, lerpPercent);
					XMVECTOR Q = XMQuaternionSlerp(q0, q1, lerpPercent);

					XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
					XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));

					break;
				}
			}
		}
	}


};
struct AnimationClip
{
	float GetClipStartTime()const
	{
		float t = MathHelper::Infinity;
		for (UINT i = 0; i < BoneAnimations.size(); ++i)
		{
			t = MathHelper::Min(t, BoneAnimations[i].GetStartTime());
		}

		return t;
	}
	float GetClipEndTime()const
	{
		float t = 0.0f;
		for (UINT i = 0; i < BoneAnimations.size(); ++i)
		{
			t = MathHelper::Max(t, BoneAnimations[i].GetEndTime());
		}

		return t;
	}

	void Interpolate(float t, std::vector<DirectX::XMFLOAT4X4>& boneTransforms) const
	{
		for (UINT i = 0; i < BoneAnimations.size(); ++i)
		{
			BoneAnimations[i].Interpolate(t, boneTransforms[i]);
		}
	}

	std::vector<BoneAnimation> BoneAnimations;
};
struct Material
{
	std::string Name;
	int MatCBIndex = -1;

	int DiffuseSrvHeapIndex = -1;

	int NormalSrvHeapIndex = -1;
	int NumFramesDirty = 2;

	XMFLOAT3 Ambient = { 0.0f, 0.0f, 0.0f };
	XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
	XMFLOAT3 Specular = { 0.01f, 0.01f, 0.01f };
	XMFLOAT3 Emissive  = { 0.01f, 0.01f, 0.01f };

	float Roughness = .25f;
	XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();
};
struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexC;

	bool operator==(const Vertex& other) const
	{
		if (Pos.x != other.Pos.x || Pos.y != other.Pos.y || Pos.z != other.Pos.z)
			return false;

		if (Normal.x != other.Normal.x || Normal.y != other.Normal.y || Normal.z != other.Normal.z)
			return false;

		if (TexC.x != other.TexC.x || TexC.y != other.TexC.y)
			return false;

		return true;
	}
};
struct CharacterVertex : Vertex
{
	DirectX::XMFLOAT3 BoneWeights;
	BYTE BoneIndices[4];

	uint16_t MaterialIndex;
};