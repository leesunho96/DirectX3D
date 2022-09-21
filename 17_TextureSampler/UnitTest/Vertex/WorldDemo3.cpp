#include "stdafx.h"
#include "WorldDemo3.h"

void WorldDemo3::Initialize()
{
	shader = new Shader(L"10_World.fx");
	{
		CreateVertex();
		CreateVertex2();
	}
	
}

void WorldDemo3::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
	SafeRelease(vertexBuffer2);
}

void WorldDemo3::Update()
{
	if (Keyboard::Get()->Press(VK_CONTROL))
	{
		if (Keyboard::Get()->Press(VK_RIGHT))
		{
			Position.x += 2.0f * Time::Delta();
		}
		else if (Keyboard::Get()->Press(VK_LEFT))
		{
			Position.x -= 2.0f * Time::Delta();
		}
	}
	else
	{
		if (Keyboard::Get()->Press(VK_RIGHT))
		{
			Scale.x += 2.0f * Time::Delta();
		}
		else if (Keyboard::Get()->Press(VK_LEFT))
		{
			Scale.x -= 2.0f * Time::Delta();
		}
	}
	
	Matrix S, T;
	D3DXMatrixScaling(&S, Scale.x, Scale.y, Scale.z);
	D3DXMatrixTranslation(&T, Position.x, Position.y, Position.z);

	Matrix W = S * T;
	world = W;


	//if (Keyboard::Get()->Press(VK_CONTROL))
	//{
	//	if (Keyboard::Get()->Press(VK_LEFT))
	//	{
	//		world._41 -= 2.0f * Time::Delta();
	//		world._42 -= 2.0f * Time::Delta();
	//	}
	//	else if (Keyboard::Get()->Press(VK_RIGHT))
	//	{
	//		world._41 += 2.0f * Time::Delta();
	//		world._42 += 2.0f * Time::Delta();
	//	}
	//}
	//else
	//{
	//	if (Keyboard::Get()->Press(VK_LEFT))
	//	{
	//		world2._41 -= 2.0f * Time::Delta();
	//		world2._42 -= 2.0f * Time::Delta();
	//	}
	//	else if (Keyboard::Get()->Press(VK_RIGHT))
	//	{
	//		world2._41 += 2.0f * Time::Delta();
	//		world2._42 += 2.0f * Time::Delta();
	//	}
	//}
}

void WorldDemo3::Render()
{
	UINT stride = sizeof(Vertex);
		
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());

	{
		UINT offset = 0;
		D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		shader->AsMatrix("World")->SetMatrix(world);
		shader->AsScalar("Index")->SetInt(0);
		shader->Draw(0, 0, iVertexNum, 0);

		shader->AsMatrix("World")->SetMatrix(world2);
		shader->AsScalar("Index")->SetInt(1);
		shader->Draw(0, 1, iVertexNum, 0);		
	}

}

void WorldDemo3::CreateVertex()
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


	// 정방행렬로 변경 해주는 메소드.

	D3DXMatrixIdentity(&(world));

}

void WorldDemo3::CreateVertex2()
{

	vertices2[0].Position = Vector3(-0.5f, +0.5f, +0.0f);
	vertices2[1].Position = Vector3(+0.5f, +0.5f, +0.0f);
	vertices2[2].Position = Vector3(+0.5f, +0.0f, +0.0f);
			
	vertices2[3].Position = Vector3(-0.5f, +0.5f, +0.0f);
	vertices2[4].Position = Vector3(+0.5f, +0.0f, +0.0f);
	vertices2[5].Position = Vector3(-0.5f, +0.0f, +0.0f);
	//
	D3D11_BUFFER_DESC desc;

	// ZeroMemory : 내부에 포인터 변수가 존재하는 경우 사용.
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = sizeof(D3D11_BUFFER_DESC) * iVertexNum;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = vertices2;

	// ssert의 일종.
	Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer2));


	// 정방행렬로 변경 해주는 메소드.

	D3DXMatrixIdentity(&(world2));

}


