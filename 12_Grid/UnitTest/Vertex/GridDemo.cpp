#include "stdafx.h"
#include "GridDemo.h"

void GridDemo::Initialize()
{
	shader = new Shader(L"10_World.fx");

	vertexCount = (width + 1) * (height + 1);
	vertices = new Vertex[vertexCount];

	for (UINT y = 0; y < height ; y++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT i = (width + 1) * y + x;

			vertices[i].Position.x = (float)x;
			vertices[i].Position.y = (float)y;
			vertices[i].Position.z = 0.0f;
		}
	}

	//
	// CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		// ZeroMemory : 내부에 포인터 변수가 존재하는 경우 사용.
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(D3D11_BUFFER_DESC) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		// ssert의 일종.
		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));


		// 정방행렬로 변경 해주는 메소드.	
	}


	indexCount = (width * height) * 6;
	Indices = new UINT[indexCount];

	UINT Index = 0;
	for (UINT y = 0; y < height ; y++)
	{
		for (UINT x = 0; x < width; x++)
		{
			Indices[Index + 0] = (width + 1) * y + x;
			Indices[Index + 1] = (width + 1) * (y + 1) + x;
			Indices[Index + 2] = (width + 1) * y + x + 1;
			Indices[Index + 3] = (width + 1) * y + x + 1;
			Indices[Index + 4] = (width + 1) * (y + 1) + x;
			Indices[Index + 5] = (width + 1) * (y + 1) + x + 1;

			Index += 6;
		}
	}

	// CreateIndexBuffer
	{
		D3D11_BUFFER_DESC desc;
		// ZeroMemory : 내부에 포인터 변수가 존재하는 경우 사용.
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = Indices;

		// ssert의 일종.
		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &IndexBuffer));


		// 정방행렬로 변경 해주는 메소드.

	}


	SafeDelete(vertices);
	SafeDelete(Indices);

	D3DXMatrixIdentity(&(world));

}

void GridDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);	
	SafeRelease(IndexBuffer);
}

void GridDemo::Update()
{
}

void GridDemo::Render()
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

	shader->DrawIndexed(0, 1, indexCount);
}
     