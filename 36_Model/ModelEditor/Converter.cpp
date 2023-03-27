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
	// TODO : 본 정보 읽기
	{	
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
	}

	// TODO : 메시 정보 읽기
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

		// vector의 top 역할
		const UINT startVertex = mesh->Vertices.size();

		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			Model::ModelVertex vertex;
			memcpy(&vertex.Position, &srcMesh->mVertices[v], sizeof(Vector3));

			/*
			 * 일반적으로 여러장의 텍스쳐 렌더링 가능(피부, 피부 위 문신 등) 그러나 3DMAX와 같은 프로그램에서 Layering 사용.
			 * Client에서 해당 기능 사용해도 되지만, 주로 Shader 이용하여 구현하지, Mesh에 구현하지는 않음.
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
				// mesh의 Indices, vertex 모두 배열 형태로 구성.
				/*
				 * vertex  : 0, 1, 2, 3,             4, 5, 6, 7
				 * Indices : 0, 1, 2,    2, 1, 3,    4 ,5, 6,    6, 5, 7
				 * 위와 같이 구성.
				 *
				 * vertex가 3까지 들어간 시점에서 startVertex : 4.
				 * 다음 사각형 rendering시 입력될 Indices의 시작 : 4.
				 * face.mIndeices는 해당 face의 index 번호.
				 */
				// Txt 파일로 해당 정보를 저장 할 수는 없음(매우 느림)
				// Binary File로 저장(우리가 원하는 바이트 단위로 읽어 들일 수 있음.)
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
