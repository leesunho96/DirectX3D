#include "stdafx.h"
#include "Vertex_Line.h"

void Vertex_Line::Initialize()
{
	shader = new Shader(L"01_Vertex.fx");
	vertices[0].Position = Vector3(0.0f, 0.0f, 0.0f);
	vertices[1].Position = Vector3(0.0f, 1.0f, 0.0f);

	//
	D3D11_BUFFER_DESC desc;

	// ZeroMemory : 내부에 포인터 변수가 존재하는 경우 사용.
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = sizeof(D3D11_BUFFER_DESC) * 2;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = vertices;

	// assert의 일종.
	Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
}

void Vertex_Line::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void Vertex_Line::Update()
{

}

void Vertex_Line::Render()
{
	UINT stride = sizeof(Vertex);

	// 시작 지점. 해당 index 정점부터 Render.
	UINT offset = 0;
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Draw(0, 0, 2);


}


