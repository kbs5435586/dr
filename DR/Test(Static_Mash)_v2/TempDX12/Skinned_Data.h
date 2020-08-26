#pragma once
class CSkinned_Data
{
public:
	CSkinned_Data();
	~CSkinned_Data();
public:
	UINT BoneCount()const;
public:

	_float							GetClipStartTime(const  string& clipName)const;
	_float							GetClipEndTime(const  string& clipName)const;
	string							GetAnimationName(int num) const;
	vector<_int>					GetBoneHierarchy() const;
	vector<_matrix>					GetBoneOffsets() const;
	AnimationClip					GetAnimation(string clipName) const;
	vector<_int>					GetSubmeshOffset() const;
	XMFLOAT4X4						getBoneOffsets(int num) const;
	vector< string>					GetBoneName() const;
public:
	void Set(vector<int>& boneHierarchy, vector<DirectX::XMFLOAT4X4>& boneOffsets,
		map< string, AnimationClip>* animations = nullptr);
	void SetAnimation(AnimationClip inAnimation, string inClipName);
	void SetAnimationName(const  string& clipName);
	void SetBoneName(string boneName);
	void SetSubmeshOffset(int num);
public:
	void clear();
	void GetFinalTransforms(const  string& clipName, float timePos,
		vector<DirectX::XMFLOAT4X4>& finalTransforms)const;
private:
	vector<string>				m_vecBoneName;
	vector<_int>				m_vecBoneHierarchy;
	vector<_matrix>				m_vecBoneOffsets;
	vector<string>				m_vecAnimationName;
	vector<_int>				m_vecSubmeshOffset;
	map<string, AnimationClip>	m_vecAnimations;
};

