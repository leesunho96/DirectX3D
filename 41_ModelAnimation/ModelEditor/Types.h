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

struct asMaterial
{
	string Name; // asMesh의 MaterialName와 일치

	Color Ambient;
	Color Diffuse;
	Color Specular;
	Color Emissive;


	
	string DiffuseFile; // 실제로 보여질 Texture File
	string SpecularFile;
	string NormalFile;
};


struct asBlendWeight
{
	Vector4 Indices = Vector4(0, 0, 0, 0);
	Vector4 Weights = Vector4(0, 0, 0, 0);

	void Set(UINT index, UINT boneIndex, float weight)
	{
		float i = (float)boneIndex;
		float w = weight;

		switch (index)
		{
		case 0: Indices.x = i; Weights.x = w; break;
		case 1: Indices.y = i; Weights.y = w; break;
		case 2: Indices.z = i; Weights.z = w; break;
		case 3: Indices.w = i; Weights.w = w; break;
		}
	}
};


struct asBoneWeights
{
private:
	typedef pair<int, float> Pair;
	vector<Pair> BoneWeights;

public:
	void AddWeights(UINT boneIndex, float boneWeights)
	{
		if (boneWeights <= 0.0f) return;

		bool bInsert = false;
		vector<Pair>::iterator it = BoneWeights.begin();
		while (it != BoneWeights.end())
		{
			if (boneWeights > it->second)
			{
				BoneWeights.insert(it, Pair(boneIndex, boneWeights));
				bInsert = true;

				break;
			}

			it++;
		} // while(it)

		if (bInsert == false)
			BoneWeights.push_back(Pair(boneIndex, boneWeights));
	}

	void GetBlendWeights(asBlendWeight& blendWeights)
	{
		for (UINT i = 0; i < BoneWeights.size(); i++)
		{
			if (i >= 4) return;

			blendWeights.Set(i, BoneWeights[i].first, BoneWeights[i].second);
		}
	}

	void Normalize()
	{
		float totalWeight = 0.0f;

		int i = 0;
		vector<Pair>::iterator it = BoneWeights.begin();
		while (it != BoneWeights.end())
		{
			if (i < 4)
			{
				totalWeight += it->second;
				i++; it++;
			}
			else
				it = BoneWeights.erase(it);
		}

		float scale = 1.0f / totalWeight;

		it = BoneWeights.begin();
		while (it != BoneWeights.end())
		{
			it->second *= scale;
			it++;
		}
	}
};