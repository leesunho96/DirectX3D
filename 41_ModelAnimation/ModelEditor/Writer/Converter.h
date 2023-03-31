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
	 * �ش� ������ ���� ���� ���� �Ǵ�.
	 * �Ϲ������� fbx���Ͽ� ��� �ʿ� �ؽ��ĵ��� �����ϸ� ������ ���� ������,
	 * �ܺο� ������ �ؽ���(.tga)�� �����Ѵٸ� �ش� �ؽ��ĸ� �����Ͽ� _Texture ������ ����
	 * �׶��� xml������ ���������� ���� �� �ʿ䰡 ����.
	 * �̷��� ���̽��� ��쿡 ���� ������ �ٽ� ����ٸ� ����� ������ ������� �������.
	 * �̷��� ���̽� �����ϱ� ���Ͽ� overwrite ���� �Ҵ��Ͽ� ����� ���� �Ǵ�.
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


