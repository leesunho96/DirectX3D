#pragma once
#include "Systems/IExecute.h"

class Index_Demo : public IExecute
{
public:

	// IExecute��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Ready() override {};
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override;
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};

private:

private:
	Shader* shader;
	const static int iVertexNum = 4;
	Vertex vertices[iVertexNum];
	// I�� �����ϴ� Ŭ���� : DirectX Interface. 
	ID3D11Buffer* vertexBuffer;

	UINT Indices[6];
	ID3D11Buffer* IndexBuffer;

	Matrix world;

};