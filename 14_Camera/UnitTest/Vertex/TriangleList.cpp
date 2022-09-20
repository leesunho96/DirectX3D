#include "stdafx.h"
#include "TriangleList.h"

void TriangleList::Initialize()
{
	shader = new Shader(L"06_Triangle.fx");
	{
		vertices[0].Position = Vector3(+0.0f, +0.5f, +0.0f);
		vertices[1].Position = Vector3(+0.5f, +0.0f, +0.0f);
		vertices[2].Position = Vector3(-0.5f, +0.0f, +0.0f);
		//
		D3D11_BUFFER_DESC desc;

		// ZeroMemory : ���ο� ������ ������ �����ϴ� ��� ���.
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(D3D11_BUFFER_DESC) * iVertexNum;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		// ssert�� ����.
		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	{
		vertices2[0].Position = Vector3(+0.0f, +0.0f, +0.0f);
		vertices2[1].Position = Vector3(+0.5f, -0.5f, +0.0f);
		vertices2[2].Position = Vector3(-0.5f, -0.5f, +0.0f);

		D3D11_BUFFER_DESC desc;

		// ZeroMemory : ���ο� ������ ������ �����ϴ� ��� ���.
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(D3D11_BUFFER_DESC) * iVertexNum;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices2;

		// ssert�� ����.
		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer2));
	}
}

void TriangleList::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void TriangleList::Update()
{

}

void TriangleList::Render()
{
	UINT stride = sizeof(Vertex);

	// ���� ����. �ش� index �������� Render.

	{
		UINT offset = 0;
		D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		shader->Draw(0, 0, iVertexNum, 0);
	}
	{
		UINT offset = 0;
		D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer2, &stride, &offset);
		D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		shader->Draw(0, 1, iVertexNum, 0);
	}
}


