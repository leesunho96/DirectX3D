#include "stdafx.h"
#include "Converter.h"
#include "Types.h"

Converter::Converter()
{
	importer = new Assimp::Importer();
}

Converter::~Converter()
{
	SafeDelete(importer);
}

void Converter::ReadFile()
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
		aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_GenNormals | aiProcess_CalcTangentSpace

	);

	assert(scene != NULL);


}

void Converter::ExportMesh(wstring savePath)
{
	savePath = L"../../_Models/" + savePath + L".mesh";

	ReadBoneData(scene->mRootNode, -1, -1);
}

void Converter::ReadBoneData(const aiNode* const node, const int index, const int parent)
{
	// TODO : �� ���� �б�
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

	// TODO : �޽� ���� �б�


	ReadMeshData(node, index);

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ReadBoneData(node->mChildren[i], bones.size(), index);
	}
}

void Converter::ReadMeshData(const aiNode* const node, int bone)
{

}

void Converter::WriteMeshData(wstring savePath)
{
}
