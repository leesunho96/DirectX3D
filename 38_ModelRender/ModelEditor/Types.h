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
 * Mesh와 Bone은 기본적으로 1 : 1 관계
 * 본은 존재하나 Mesh가 존재하지 않는 경우는 존재. -> 가상 본
 * 
 * string Name : MeshName. 일반적으로 Bone의 Name와 일치
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