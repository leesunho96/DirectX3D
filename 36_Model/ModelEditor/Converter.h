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
	void ReadBoneData(const aiNode* const node, const int index, const int parent);
	void ReadMeshData(const aiNode* const node, const int bone);

	void WriteMeshData(wstring savePath);

private:
	wstring file;

	Assimp::Importer* importer;
	const aiScene* scene;

	vector<struct asBone*> bones;
	vector<struct asMesh*> meshes;
public:
};