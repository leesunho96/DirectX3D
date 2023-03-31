#pragma once
#include "stdafx.h"

struct asBone
{
	int Index;
	string Name;

	int Parent;
	Matrix Transform;
};



/*
 * Mesh�� Bone�� �⺻������ 1 : 1 ����
 * ���� �����ϳ� Mesh�� �������� �ʴ� ���� ����. -> ���� ��
 * 
 * string Name : MeshName. �Ϲ������� Bone�� Name�� ��ġ
 */
struct asMesh
{
	string Name;
	int BoneIndex;

	aiMesh* mesh;

	string MaterialName;

	vector<Model::ModelVertex> Vertices;
	vector<UINT> Indices;
};

struct asMaterial
{
	string Name; // asMesh�� MaterialName�� ��ġ

	Color Ambient;
	Color Diffuse;
	Color Specular;
	Color Emissive;


	
	string DiffuseFile; // ������ ������ Texture File
	string SpecularFile;
	string NormalFile;
};