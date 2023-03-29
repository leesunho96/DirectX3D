#include "stdafx.h"
#include "Converter.h"
#include "Types.h"
#include "Utilities/BinaryFile.h"
#include "Utilities/Xml.h"

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

void Converter::ReadBoneData(aiNode* node, int index, int parent)
{
	asBone* bone = new asBone();
	bone->Index = index;
	bone->Parent = parent;
	bone->Name = node->mName.C_Str();


	Matrix transform(node->mTransformation[0]);
	D3DXMatrixTranspose(&bone->Transform, &transform);

	Matrix matParent;
	if (parent < 0)
		D3DXMatrixIdentity(&matParent);
	else
		matParent = bones[parent]->Transform;

	bone->Transform = bone->Transform * matParent;
	bones.push_back(bone);


	ReadMeshData(node, index);

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadBoneData(node->mChildren[i], bones.size(), index);
}

void Converter::ReadMeshData(aiNode* node, int bone)
{
	if (node->mNumMeshes < 1) return;

	asMesh* mesh = new asMesh();
	mesh->Name = node->mName.C_Str();
	mesh->BoneIndex = bone;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		UINT index = node->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[index];

		aiMaterial* material = scene->mMaterials[srcMesh->mMaterialIndex];
		mesh->MaterialName = material->GetName().C_Str();


		UINT startVertex = mesh->Vertices.size();
		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			Model::ModelVertex vertex;
			memcpy(&vertex.Position, &srcMesh->mVertices[v], sizeof(Vector3));

			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.Uv, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));

			if (srcMesh->HasNormals())
				memcpy(&vertex.Normal, &srcMesh->mNormals[v], sizeof(Vector3));

			mesh->Vertices.push_back(vertex);
		}

		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			aiFace& face = srcMesh->mFaces[f];

			for (UINT k = 0; k < face.mNumIndices; k++)
				mesh->Indices.push_back(face.mIndices[k] + startVertex);
		}

		meshes.push_back(mesh);
	}
}

void Converter::WriteMeshData(wstring savePath)
{
	Path::CreateFolders(Path::GetDirectoryName(savePath));

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

void Converter::ExportMaterial(wstring savePath, bool bOverwrite)
{
	savePath = L"../../_Textures/" + savePath + L".material";

	if(!bOverwrite)
	{
		if(Path::ExistFile(savePath))
		{
			return;
		}
	}

	ReadMaterialData();
	WriteMaterialData(savePath);
}

void Converter::ReadMaterialData()
{
	for (UINT i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* srcMaterial = scene->mMaterials[i];
		asMaterial* material = new asMaterial();

		material->Name = srcMaterial->GetName().C_Str();

		aiColor3D color;


		/*
		 * AI_MATKEY_COLOR_AMBIENT : 매개변수 3개의 매크로.
		 * 필요에 따라 원하는 데이터 추출 가능.
		 * AI_MAT_COLOR_TRANSPERANT : alpha 값. 반투명 등 렌더링
		 *
		 * 빛은 불투명 값. Specular.alpha에 빛의 강도를 저장하는 경우가 많음.
		 */
		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);		
		material->Diffuse = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->Ambient = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->Specular = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		material->Emissive = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_SHININESS, material->Specular.a);


		// Material File의 경우는 file Path 반환.
		aiString file;

		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->DiffuseFile = file.C_Str();

		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->SpecularFile = file.C_Str();

		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->NormalFile = file.C_Str();


		materials.push_back(material);
	}
}

void Converter::WriteMaterialData(wstring savepath)
{
	string folder = String::ToString(Path::GetDirectoryName(savepath));
	string file = String::ToString(Path::GetFileName(savepath));

	Path::CreateFolder(folder);


	// XML 관리 코드.
	Xml::XMLDocument* document = new Xml::XMLDocument();

	Xml::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	Xml::XMLElement* root = document->NewElement("Materials");
	root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	document->LinkEndChild(root);

	for(auto material : materials)
	{
		Xml::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		Xml::XMLElement* element = nullptr;

		element = document->NewElement("Name");
		element->SetText(material->Name.c_str());		
		
		node->LinkEndChild(element);

		/*element = document->NewElement("Ambient");
		element->SetAttribute()*/
	}





	document->SaveFile((folder + file).c_str());
	SafeDelete(document);

}
