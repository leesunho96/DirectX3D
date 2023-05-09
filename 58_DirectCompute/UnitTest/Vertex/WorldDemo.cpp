#include "stdafx.h"
#include "WorldDemo.h"

void WorldDemo::Initialize()
{
	shader = new Shader(L"09_World.fx");
	{
		vertices[0].Position = Vector3(-0.5f, +0.5f, +0.0f);
		vertices[1].Position = Vector3(+0.5f, +0.5f, +0.0f);
		vertices[2].Position = Vector3(+0.5f, +0.0f, +0.0f);

		vertices[3].Position = Vector3(-0.5f, +0.5f, +0.0f);
		vertices[4].Position = Vector3(+0.5f, +0.0f, +0.0f);
		vertices[5].Position = Vector3(-0.5f, +0.0f, +0.0f);
		//
		D3D11_BUFFER_DESC desc;

		// ZeroMemory : 내부에 포인터 변수가 존재하는 경우 사용.
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(D3D11_BUFFER_DESC) * iVertexNum;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		// ssert의 일종.
		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}
	{
		// 정방행렬로 변경 해주는 메소드.
		D3DXMatrixIdentity(&world);
	}
	
}

void WorldDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void WorldDemo::Update()
{
	if (Keyboard::Get()->Press(VK_LEFT))
	{
		world._11 -= 2.0f * Time::Delta();
		world._22 -= 2.0f * Time::Delta();
	}
	else if (Keyboard::Get()->Press(VK_RIGHT))
	{
		world._11 += 2.0f * Time::Delta();
		world._22 += 2.0f * Time::Delta();
	}

	static float y = 0.5f;
		
}

void WorldDemo::Render()
{
	UINT stride = sizeof(Vertex);
	
	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());

	{
		UINT offset = 0;
		D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//shader->Draw(0, 1, iVertexNum, 0);
		shader->Draw(0, 0, iVertexNum, 0);
	}

}


