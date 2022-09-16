#pragma once
#include "Systems/IExecute.h"

class GridDemo : public IExecute
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

	UINT width = 5, height = 5;

	Vertex* vertices;
	UINT vertexCount;
	ID3D11Buffer* vertexBuffer;
		
	// I�� �����ϴ� Ŭ���� : DirectX Interface. 

	UINT indexCount;
	UINT* Indices;	   
	ID3D11Buffer* IndexBuffer;

	Matrix world;

};