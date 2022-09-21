#pragma once
#include "Systems/IExecute.h"

class WorldDemo : public IExecute
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
	Shader* shader;
	const static int iVertexNum = 6;
	Vertex vertices[iVertexNum];
	// I�� �����ϴ� Ŭ���� : DirectX Interface. 
	ID3D11Buffer* vertexBuffer;

	Vertex vertices2[iVertexNum];
	// I�� �����ϴ� Ŭ���� : DirectX Interface. 
	ID3D11Buffer* vertexBuffer2;


	Matrix world;

};