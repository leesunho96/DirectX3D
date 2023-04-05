#include "stdafx.h"
#include "Index_Demo.h"

void Index_Demo::Initialize()
{
	shader = new Shader(L"10_World.fx");

	vertices[0].Position = Vector3(-0.5f, -0.5f, +0.0f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, +0.0f);
	vertices[2].Position = Vector3(+0.5f, -0.5f, +0.0f);
	vertices[3].Position = Vector3(+0.5f, +0.5f, +0.0f);
	//
	// CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		// ZeroMemory : 내부에 포인터 변수가 존재하는 경우 사용.
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(D3D11_BUFFER_DESC) * iVertexNum;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		// ssert의 일종.
		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));


		// 정방행렬로 변경 해주는 메소드.	
	}

	Indices[0] = 0;
	Indices[1] = 1;
	Indices[2] = 2;
	Indices[3] = 2;
	Indices[4] = 1;
	Indices[5] = 3;

	// CreateIndexBuffer
	{
		D3D11_BUFFER_DESC desc;
		// ZeroMemory : 내부에 포인터 변수가 존재하는 경우 사용.
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * 6;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = Indices;

		// ssert의 일종.
		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &IndexBuffer));


		// 정방행렬로 변경 해주는 메소드.

	}


	D3DXMatrixIdentity(&(world));

}

void Index_Demo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);	
	SafeRelease(IndexBuffer);
}

void Index_Demo::Update()
{
}

void Index_Demo::Render()
{
	shader->AsScalar("Index")->SetInt(0);
	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());

	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		D3D::GetDC()->IASetIndexBuffer(IndexBuffer,DXGI_FORMAT_R32_UINT, 0);
			   	
	}

	shader->DrawIndexed(0, 1, 6);
}
     