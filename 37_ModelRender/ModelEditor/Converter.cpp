#include "stdafx.h"
#include "Converter.h"
#include "Types.h"
#include "Utilities/BinaryFile.h"

Converter::Converter()
{
	importer = new Assimp::Importer();
}

Converter::~Converter()
{
	SafeDelete(importer);
}

void Converter::ReadFile(wstring file)
{
	this->file = L"../../_Assets/" + file;


	/*
	 * �Ϲ������� DX�� �޼� ��ǥ��, Ⱦ�켱,
	 * ��κ��� 3d Graphic program : ������ ��ǥ��, ���켱
	 *
	 * -> �޼� ��ǥ��� �������ִ� ��ũ�� : aiProcess_ConvertToLeftHanded,
	 * -> Ⱦ�켱���� ������ ���� ��ġ��� ���
	 *
	 * aiProcess_Trialgulate : �ﰢ�� ���� �����ͷ� ��ȯ. �� �ܿ� ���� �� �������� ���̽� ����.
	 *
	 * aiProcess_GenUVCoords : �ﰢ���� �´� UV ��ǥ ��ȯ
	 * aiProcess_GenNormals : Normal�� �ﰢ���� �´� UV ��ǥ ��ȯ
	 *
	 */
	scene = importer->ReadFile
	(
		String::ToString(this->file),
		aiProcess_ConvertToLeftHanded 
		| aiProcess_Triangulate 
		| aiProcess_GenUVCoords 
		| aiProcess_GenNormals 
		| aiProcess_CalcTangentSpace
	);

	assert(scene != NULL);


}

void Converter::ExportMesh(wstring savePath)
{
	savePath = L"../../_Models/" + savePath + L".mesh";

	ReadBoneData(scene->mRootNode, -1, -1);
	WriteMeshData(savePath);
}

void Converter::ReadBoneData(const aiNode* const node, const int index, const int parent)
{
	// TODO : �� ���� �б�
	{	
	asBone* bone = new asBone();
	bone->Index = index;
	bone->Parent = parent;
	bone->Name = node->mName.C_Str();

	/*
	 * Matrix(float*) -> �ش������� �ּҺ��� 16�� ��� �о����.
	 * aiNode->mTransformation�� float 16�� ����.
	 */
	Matrix transform(node->mTransformation[0]);


	/* ���켱�� Ⱦ�켱���� ����.*/
	D3DXMatrixTranspose(&bone->Transform, &transform);

	Matrix matParent;
	/*
	 * Root�ΰ�� �ش� matParent�� SRT�� �Է��ϴ� ��쵵 ����.
	 * �׷��� ���� Root�� ���̸� ���� �߻� ������ ũ�� ������ �츮�� ������� ����.
	 */

	if(parent < 0)
	{
		D3DXMatrixIdentity(&matParent);
	}
	else
	{
		matParent = bones[parent]->Transform;
	}

	/*
	 * �θ��� transform���κ��� �ڽ��� transform ����
	 */
	bone->Transform = bone->Transform * matParent;
	bones.push_back(bone);
	}

	// TODO : �޽� ���� �б�
	ReadMeshData(node, index);

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ReadBoneData(node->mChildren[i], bones.size(), index);
	}
}

void Converter::ReadMeshData(const aiNode* const node, const int bone)
{
	if(node->mNumChildren < 1) return;

	asMesh* const mesh = new asMesh();

	mesh->Name = node->mName.C_Str();
	mesh->BoneIndex = bone;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		const UINT index = node->mMeshes[i];
		const aiMesh* srcMesh = scene->mMeshes[index];

		aiMaterial* material = scene->mMaterials[srcMesh->mMaterialIndex];
		mesh->MaterialName = material->GetName().C_Str();

		// vector�� top ����
		const UINT startVertex = mesh->Vertices.size();

		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			Model::ModelVertex vertex;
			memcpy(&vertex.Position, &srcMesh->mVertices[v], sizeof(Vector3));

			/*
			 * �Ϲ������� �������� �ؽ��� ������ ����(�Ǻ�, �Ǻ� �� ���� ��) �׷��� 3DMAX�� ���� ���α׷����� Layering ���.
			 * Client���� �ش� ��� ����ص� ������, �ַ� Shader �̿��Ͽ� ��������, Mesh�� ���������� ����.
			 *
			 */
			if(srcMesh->HasTextureCoords(0))
			{
				memcpy(&vertex.Uv, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));
			}

			if(srcMesh->HasNormals())
			{
				memcpy(&vertex.Normal, &srcMesh->mNormals[0], sizeof(Vector3));
			}

			mesh->Vertices.push_back(vertex);
		}

		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			const aiFace& face = srcMesh->mFaces[f];

			for (UINT k = 0; k < face.mNumIndices; k++)
			{
				// mesh�� Indices, vertex ��� �迭 ���·� ����.
				/*
				 * vertex  : 0, 1, 2, 3,             4, 5, 6, 7
				 * Indices : 0, 1, 2,    2, 1, 3,    4 ,5, 6,    6, 5, 7
				 * ���� ���� ����.
				 *
				 * vertex�� 3���� �� �������� startVertex : 4.
				 * ���� �簢�� rendering�� �Էµ� Indices�� ���� : 4.
				 * face.mIndeices�� �ش� face�� index ��ȣ.
				 */
				// Txt ���Ϸ� �ش� ������ ���� �� ���� ����(�ſ� ����)
				// Binary File�� ����(�츮�� ���ϴ� ����Ʈ ������ �о� ���� �� ����.)
				mesh->Indices.push_back(face.mIndices[k] + startVertex);
			}
		}
		meshes.push_back(mesh);
	}
}

void Converter::WriteMeshData(wstring savePath)
{
	Path::CreateFolder(Path::GetDirectoryName(savePath));

	BinaryWriter* w = new BinaryWriter();

	w->Open(savePath);
	w->UInt(bones.size());

	for (asBone* bone : bones)
	{
		w->Int(bone->Index);
		w->String(bone->Name);
		w->Int(bone->Parent);
		w->Matrix(bone->Transform);

		SafeDelete(bone);
	}

	w->UInt(meshes.size());
	for (asMesh* meshData : meshes)
	{
		w->String(meshData->Name);
		w->Int(meshData->BoneIndex);

		w->String(meshData->MaterialName);

		w->UInt(meshData->Vertices.size());
		w->Byte(&meshData->Vertices[0], sizeof(Model::ModelVertex) * meshData->Vertices.size());

		w->UInt(meshData->Indices.size());
		w->Byte(&meshData->Indices[0], sizeof(UINT) * meshData->Indices.size());

		SafeDelete(meshData);
	}

	w->Close();
	SafeDelete(w);
}
