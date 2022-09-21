#include "stdafx.h"
#include "WorldDemo2.h"

void WorldDemo2::Initialize()
{
	shader = new Shader(L"10_World.fx");
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
		for (int i = 0; i < 3; i++)
		{
			D3DXMatrixIdentity(&(world[i]));
		}
	}
	
}

void WorldDemo2::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void WorldDemo2::Update()
{
	ImGui::InputInt("Select", (int*)(&iWorldIndex));
	iWorldIndex %= 3;


	if (Keyboard::Get()->Press(VK_CONTROL))
	{
		if (Keyboard::Get()->Press(VK_LEFT))
		{
			world[iWorldIndex]._11 -= 2.0f * Time::Delta();
			world[iWorldIndex]._22 -= 2.0f * Time::Delta();
		}
		else if (Keyboard::Get()->Press(VK_RIGHT))
		{
			world[iWorldIndex]._11 += 2.0f * Time::Delta();
			world[iWorldIndex]._22 += 2.0f * Time::Delta();
		}
	}
	else
	{
		if (Keyboard::Get()->Press(VK_LEFT))
		{
			world[iWorldIndex]._41 -= 2.0f * Time::Delta();
		}
		else if (Keyboard::Get()->Press(VK_RIGHT))
		{
			world[iWorldIndex]._41 += 2.0f * Time::Delta();
		}
	}
}

void WorldDemo2::Render()
{
	UINT stride = sizeof(Vertex);
		
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());

	{
		UINT offset = 0;
		D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		shader->AsMatrix("World")->SetMatrix(world[0]);
		shader->AsScalar("Index")->SetInt(0);
		shader->Draw(0, 0, iVertexNum, 0);

		shader->AsMatrix("World")->SetMatrix(world[1]);
		shader->AsScalar("Index")->SetInt(1);
		shader->Draw(0, 0, iVertexNum, 0);

		shader->AsMatrix("World")->SetMatrix(world[2]);
		shader->AsScalar("Index")->SetInt(2);
		shader->Draw(0, 0, iVertexNum, 0);
	}

}


