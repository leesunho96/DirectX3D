#pragma once
#include "Systems/IExecute.h"

class WorldDemo3 : public IExecute
{
public:

	// IExecute��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Ready() override {};
	virtual void Destroy() override;
	virtual void Update() override ;
	virtual void PreRender() override {};
	virtual void Render() override;
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};

private:
	void CreateVertex();
	void CreateVertex2();

private:
	Shader* shader;
	const static int iVertexNum = 6;
	Vertex vertices[iVertexNum];
	// I�� �����ϴ� Ŭ���� : DirectX Interface. 
	ID3D11Buffer* vertexBuffer;

	Vertex vertices2[iVertexNum];
	// I�� �����ϴ� Ŭ���� : DirectX Interface. 
	ID3D11Buffer* vertexBuffer2;

	UINT iWorldIndex;
	Matrix world;
	Matrix world2;
	Vector3 Position = Vector3(0, 0, 0);
	Vector3 Scale = Vector3(1, 1, 1);;

	Vector3 Position2 = Vector3(0, 0, 0);;
	Vector3 Scale2 = Vector3(1, 1, 1);;
};