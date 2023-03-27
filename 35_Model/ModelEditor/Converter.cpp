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
	 * 일반적으로 DX는 왼손 좌표계, 횡우선,
	 * 대부분의 3d Graphic program : 오른손 좌표계, 열우선
	 *
	 * -> 왼손 좌표계로 변경해주는 매크로 : aiProcess_ConvertToLeftHanded,
	 * -> 횡우선으로 변경은 추후 전치행렬 사용
	 *
	 * aiProcess_Trialgulate : 삼각형 기준 데이터로 반환. 그 외에 복셀 등 여러가지 케이스 존재.
	 *
	 * aiProcess_GenUVCoords : 삼각형에 맞는 UV 좌표 반환
	 * aiProcess_GenNormals : Normal도 삼각형에 맞는 UV 좌표 반환
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
	// TODO : 본 정보 읽기
	asBone* bone = new asBone();
	bone->Index = index;
	bone->Parent = parent;
	bone->Name = node->mName.C_Str();

	/*
	 * Matrix(float*) -> 해당포인터 주소부터 16개 끊어서 읽어들임.
	 * aiNode->mTransformation은 float 16개 구성.
	 */
	Matrix transform(node->mTransformation[0]);


	/* 열우선을 횡우선으로 변경.*/
	D3DXMatrixTranspose(&bone->Transform, &transform);

	Matrix matParent;
	/*
	 * Root인경우 해당 matParent에 SRT를 입력하는 경우도 존재.
	 * 그러나 만약 Root가 꼬이면 문제 발생 소지가 크기 때문에 우리는 정방행렬 삽입.
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
	 * 부모의 transform으로부터 자식의 transform 생성
	 */
	bone->Transform = bone->Transform * matParent;
	bones.push_back(bone);

	// TODO : 메시 정보 읽기


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
