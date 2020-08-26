#pragma once
class CSkinned_Data;
class CFBX_Loader
{
private:
    CFBX_Loader();
    ~CFBX_Loader() = default;
public:
	//Animation
	HRESULT LoadFBX(
		std::vector<CharacterVertex>& outVertexVector,
		std::vector<uint32_t>& outIndexVector,
		CSkinned_Data& outSkinnedData,
		const std::string& ClipName,
		std::vector<Material>& outMaterial,
		std::string fileName);
	// Animation 제외
	HRESULT LoadFBX(
		std::vector<Vertex>& outVertexVector,
		std::vector<uint32_t>& outIndexVector,
		std::vector<Material>& outMaterial,
		std::string fileName);
	// Animation 만
	HRESULT LoadFBX(
		CSkinned_Data& outSkinnedData,
		const std::string& clipName,
		std::string fileName);
public:
	bool LoadSkeleton(CSkinned_Data& outSkinnedData, const std::string& clipName, std::string fileName);
	// 위의 Load FBX 에 따라 호출
	bool LoadMesh(
		std::string fileName,
		std::vector<Vertex>& outVertexVector,
		std::vector<uint32_t>& outIndexVector,
		std::vector<Material>* outMaterial = nullptr);
	bool LoadMesh(
		std::string fileName,
		std::vector<CharacterVertex>& outVertexVector,
		std::vector<uint32_t>& outIndexVector,
		std::vector<Material>* outMaterial = nullptr);

	bool LoadAnimation(CSkinned_Data& outSkinnedData, const std::string& clipName, std::string fileName);


	void GetSkeletonHierarchy(
		fbxsdk::FbxNode* pNode,
		SkinnedData& outSkinnedData,
		int curIndex, int parentIndex);

	void GetControlPoints(fbxsdk::FbxNode* pFbxRootNode);

	void GetAnimation(
		fbxsdk::FbxScene* pFbxScene,
		fbxsdk::FbxNode* pFbxChildNode,
		SkinnedData& outSkinnedData,
		const std::string& ClipName, bool isGetOnlyAnim);

	void GetVerticesAndIndice(
		fbxsdk::FbxMesh* pMesh,
		std::vector<CharacterVertex>& outVertexVector,
		std::vector<uint32_t>& outIndexVector,
		SkinnedData* outSkinnedData);
	void GetVerticesAndIndice(
		fbxsdk::FbxMesh* pMesh,
		std::vector<Vertex>& outVertexVector,
		std::vector<uint32_t>& outIndexVector);


	void GetMaterials(fbxsdk::FbxNode* pNode, std::vector<Material>& outMaterial);

	void GetMaterialAttribute(fbxsdk::FbxSurfaceMaterial* pMaterial, Material& outMaterial);

	void GetMaterialTexture(fbxsdk::FbxSurfaceMaterial* pMaterial, Material& Mat);

	FbxAMatrix GetGeometryTransformation(fbxsdk::FbxNode* pNode);


	void ExportSkeleton(
		SkinnedData& outSkinnedData,
		const std::string& clipName,
		std::string fileName);

	void ExportAnimation(
		const AnimationClip& animation,
		std::string fileName,
		const std::string& clipName);
	void ExportMesh(std::vector<Vertex>& outVertexVector, std::vector<uint32_t>& outIndexVector, std::vector<Material>& outMaterial, std::string fileName);
	void ExportMesh(std::vector<CharacterVertex>& outVertexVector, std::vector<uint32_t>& outIndexVector, std::vector<Material>& outMaterial, std::string fileName);

	void clear();

private:
	std::unordered_map<unsigned int, CtrlPoint*> mControlPoints;
	std::vector<std::string> mBoneName;

	// skinnedData Output
	std::vector<int> mBoneHierarchy;
	std::vector<DirectX::XMFLOAT4X4> mBoneOffsets;
	std::unordered_map<std::string, AnimationClip> mAnimations;
};

