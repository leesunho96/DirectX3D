#include "stdafx.h"
#include "UserInterface.h"

void UserInterface::Initialize()
{
	shader = new Shader(L"07_Rect.fx");
	{
		vertices[0].Position = Vector3(-0.5f, +0.5f, +0.0f);
		vertices[1].Position = Vector3(+0.5f, +0.5f, +0.0f);
		vertices[2].Position = Vector3(+0.5f, +0.0f, +0.0f);

		vertices[3].Position = Vector3(-0.5f, +0.5f, +0.0f);
		vertices[4].Position = Vector3(+0.5f, +0.0f, +0.0f);
		vertices[5].Position = Vector3(-0.5f, +0.0f, +0.0f);
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

	
}

void UserInterface::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void UserInterface::Update()
{
	if (Keyboard::Get()->Press(VK_LEFT))
	{
		vertices[0].Position.x -= 2.0f * Time::Delta();
	}
	else if (Keyboard::Get()->Press(VK_RIGHT))
	{
		vertices[0].Position.x += 2.0f * Time::Delta();
	}

	static float y = 0.5f;

	ImGui::SliderFloat("Y", &y, -1, 1);

	vertices[1].Position.y = y;

	D3D::GetDC()->UpdateSubresource(vertexBuffer, 0, NULL, vertices, sizeof(Vertex) * iVertexNum, 0);
}

void UserInterface::Render()
{
	UINT stride = sizeof(Vertex);
	static bool bOpen = true;
	ImGui::Checkbox("DemoBox", &bOpen);
	if (bOpen)
	{
		ImGui::ShowDemoWindow(&bOpen);
	}


	// ���� ����. �ش� index �������� Render.
	string text = "";

	if (Keyboard::Get()->Press(VK_LEFT))
	{
		text += "LEFT";
	}
	else if (Keyboard::Get()->Press(VK_RIGHT))
	{
		text += "RIGHT";
	}

	Gui::Get()->RenderText(10, 60, 1, 0, 0 ,text);

	{
		UINT offset = 0;
		D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//shader->Draw(0, 1, iVertexNum, 0);
		shader->Draw(0, 0, iVertexNum, 0);
	}

}


