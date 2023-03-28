#include "Framework.h"
#include "Model/Model.h"
#include "Utilities/BinaryFile.h"

Model::Model()
{
}

Model::~Model()
{
	for (auto bone : bones)
	{
		SafeDelete(bone);
	}

	for (auto mesh : meshes)
	{
		SafeDelete(mesh);
	}

}

ModelBone* Model::BoneByName(wstring name)
{
	for (auto bone : bones)
	{
		if(bone->Name() == name)
			return bone;
	}
	return nullptr;
}

ModelMesh* Model::MeshByName(wstring name)
{
	for (auto mesh : meshes)
	{
		if (mesh->Name() == name)
			return mesh;
	}
	return nullptr;
}

void Model::ReadMesh(wstring file)
{
	file = L"../../_Models/" + file + L".mesh";


	BinaryReader* r = new BinaryReader();
	r->Open(file);

	UINT count = 0;
	count = r->UInt();
	for (UINT i = 0; i < count; i++)
	{
		ModelBone* bone = new ModelBone();
		bone->index = r->Int();
		bone->name = String::ToWString(r->String());
		bone->parentIndex = r->Int();
		bone->transform = r->Matrix();

		bones.push_back(bone);
	}

	count = r->UInt();
	for (UINT i = 0; i < count; i++)
	{
		ModelMesh* mesh = new ModelMesh();

		mesh->name = String::ToWString(r->String());
		mesh->boneIndex = r->Int();

		mesh->materialName = String::ToWString(r->String());

		//VertexData
		{
			UINT count = r->UInt();

			vector<Model::ModelVertex> vertices;
			vertices.assign(count, Model::ModelVertex());

			void* ptr = (void*)&(vertices[0]);
			r->Byte(&ptr, sizeof(Model::ModelVertex) * count);


			mesh->vertices = new Model::ModelVertex[count];
			mesh->vertexCount = count;
			copy
			(
				vertices.begin(), vertices.end(),
				stdext::checked_array_iterator<Model::ModelVertex*>(mesh->vertices, count)
				//mesh->vertices
			);
		}

		//IndexData
		{
			UINT count = r->UInt();

			vector<UINT> indices;
			indices.assign(count, UINT());

			void* ptr = (void*)&(indices[0]);
			r->Byte(&ptr, sizeof(UINT) * count);


			mesh->indices = new UINT[count];
			mesh->indexCount = count;
			copy
			(
				indices.begin(), indices.end(),
				stdext::checked_array_iterator<UINT*>(mesh->indices, count)
			);
		}

		meshes.push_back(mesh);
	}

	r->Close();
	SafeDelete(r);

	BindBone();
	BindMesh();
}

void Model::BindBone()
{
	root = bones[0];

	for (auto bone : bones)
	{
		if(bone->parentIndex > -1)
		{
			bone->parent = bones[bone->parentIndex];
			bone->parent->child.push_back(bone);
		}
		else
		{
			bone->parent = nullptr;
		}
	}
}

void Model::BindMesh()
{
	for (ModelMesh* mesh : meshes)
	{
		mesh->bone = bones[mesh->boneIndex];

		mesh->Binding(this);
	}
}
