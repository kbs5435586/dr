#include "framework.h"
#include "Skinned_Data.h"

CSkinned_Data::CSkinned_Data()
{
}

CSkinned_Data::~CSkinned_Data()
{
}

UINT CSkinned_Data::BoneCount() const
{
	return (_uint)m_vecBoneHierarchy.size();
}

float CSkinned_Data::GetClipStartTime(const string& clipName) const
{
	return 0.0f;
}

float CSkinned_Data::GetClipEndTime(const string& clipName) const
{
	return 0.0f;
}

string CSkinned_Data::GetAnimationName(int num) const
{
	return m_vecAnimationName[num];
}

vector<_int> CSkinned_Data::GetBoneHierarchy() const
{
	return m_vecBoneHierarchy;
}

vector<_matrix> CSkinned_Data::GetBoneOffsets() const
{
	return m_vecBoneOffsets;
}

AnimationClip CSkinned_Data::GetAnimation(string clipName) const
{
	return m_vecAnimations.find(clipName)->second;
}

vector<int> CSkinned_Data::GetSubmeshOffset() const
{
	return m_vecSubmeshOffset;
}

DirectX::XMFLOAT4X4 CSkinned_Data::getBoneOffsets(int num) const
{
	return m_vecBoneOffsets.at(num);
}

vector<string> CSkinned_Data::GetBoneName() const
{
	return m_vecBoneName;
}

void CSkinned_Data::Set(vector<int>& boneHierarchy, vector<DirectX::XMFLOAT4X4>& boneOffsets, map<string, AnimationClip>* animations)
{
	m_vecBoneHierarchy = boneHierarchy;
	m_vecBoneOffsets = boneOffsets;
	if (animations != nullptr)
	{
		m_vecAnimations = (*animations);
	}
}

void CSkinned_Data::SetAnimation(AnimationClip inAnimation, string inClipName)
{
	m_vecAnimations[inClipName] = inAnimation;
}

void CSkinned_Data::SetAnimationName(const string& clipName)
{
	m_vecAnimationName.push_back(clipName);
}

void CSkinned_Data::SetBoneName(string boneName)
{
	m_vecBoneName.push_back(boneName);
}

void CSkinned_Data::SetSubmeshOffset(int num)
{
	m_vecSubmeshOffset.push_back(num);
}

void CSkinned_Data::clear()
{
	m_vecBoneName.clear();
	m_vecBoneHierarchy.clear();
	m_vecBoneOffsets.clear();
	m_vecAnimationName.clear();
	m_vecSubmeshOffset.clear();
	m_vecAnimations.clear();
}

void CSkinned_Data::GetFinalTransforms(const string& clipName, float timePos, vector<DirectX::XMFLOAT4X4>& finalTransforms) const
{
	UINT numBones = (UINT)m_vecBoneOffsets.size();

	std::vector<XMFLOAT4X4> toParentTransforms(numBones);

	auto clip = m_vecAnimations.find(clipName);
	clip->second.Interpolate(timePos, toParentTransforms);
	std::vector<XMFLOAT4X4> toRootTransforms(numBones);

	toRootTransforms[0] = toParentTransforms[0];
	for (UINT i = 1; i < numBones; ++i)
	{
		XMMATRIX toParent = XMLoadFloat4x4(&toParentTransforms[i]);

		int parentIndex = m_vecBoneHierarchy[i];
		XMMATRIX parentToRoot = XMLoadFloat4x4(&toRootTransforms[parentIndex]);

		XMMATRIX toRoot = XMMatrixMultiply(toParent, parentToRoot);

		XMStoreFloat4x4(&toRootTransforms[i], toRoot);
	}

	for (UINT i = 0; i < numBones; ++i)
	{
		XMMATRIX offset = XMLoadFloat4x4(&m_vecBoneOffsets[i]);
		XMMATRIX toRoot = XMLoadFloat4x4(&toParentTransforms[i]);
		XMMATRIX finalTransform = XMMatrixMultiply(offset, toRoot);
		finalTransform *= XMMatrixScaling(0.01f, 0.01f, 0.01f);

		XMStoreFloat4x4(&finalTransforms[i], XMMatrixTranspose(finalTransform));
	}
}
