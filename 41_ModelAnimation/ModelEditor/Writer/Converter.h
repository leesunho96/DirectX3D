#pragma once

class Converter
{
public:
	Converter();
	~Converter();

	void ReadFile(wstring file);

public:
	void ExportMesh(wstring savePath);

private:
	void ReadBoneData(aiNode* node, int index, int parent);
	void ReadMeshData(aiNode* node, int bone);
	void ReadSkinningData() const;
	void WriteMeshData(wstring savePath);


public:

	/*
	 * bOverwrite
	 * 해당 파일을 덮어 쓸지 여부 판단.
	 * 일반적으로 fbx파일에 모든 필요 텍스쳐들이 존재하면 문제가 되지 않지만,
	 * 외부에 별도의 텍스쳐(.tga)로 존재한다면 해당 텍스쳐를 복사하여 _Texture 폴더에 저장
	 * 그때는 xml파일을 개별적으로 수정 할 필요가 있음.
	 * 이러한 케이스의 경우에 만약 파일을 다시 덮어쓴다면 수기로 수정한 내용또한 덮어씌여짐.
	 * 이러한 케이스 방지하기 위하여 overwrite 변수 할당하여 덮어쓸지 말지 판단.
	 *
	 *
	 */
	void ExportMaterial(wstring savePath, bool bOverwrite = true);


private:
	void ReadMaterialData();
	void WriteMaterialData(wstring savepath);
	string WriteTexture(string saveFolder, string file);
private:
	wstring file;

	Assimp::Importer* importer;
	const aiScene* scene;

	vector<struct asBone*> bones;
	vector<struct asMesh*> meshes;
	vector<struct asMaterial*> materials;
};


