#include "Framework.h"
#include "DebugLine.h"

void DebugLine::Create()
{
	assert(instance == NULL);
	
	debugLine = new DebugLine();
}

void DebugLine::Delete()
{
	SafeDelete(debugLine);
}

DebugLine* DebugLine::Get()
{
	assert(debugLine != NULL);

	return debugLine;
}

void DebugLine::RenderLine(Vector3& start, Vector3& end)
{
	RenderLine(start, end, Color(0, 1, 0, 1));
}

void DebugLine::RenderLine(Vector3& start, Vector3& end, float r, float g, float b)
{
	RenderLine(start, end, Color(r, g, b, 1));
}

void DebugLine::RenderLine(Vector3& start, Vector3& end, Color& color)
{
	vertices[drawCount].Color = color;
	vertices[drawCount++].Position = start;

	vertices[drawCount].Color = color;
	vertices[drawCount++].Position = end;


}

void DebugLine::RenderLine(float x, float y, float z, float x2, float y2, float z2)
{
	RenderLine(Vector3(x, y, z), Vector3(x2, y2, z2));
}

void DebugLine::RenderLine(float x, float y, float z, float x2, float y2, float z2, Color& color)
{
	RenderLine(Vector3(x, y, z), Vector3(x2, y2, z2), color);
}

void DebugLine::RenderLine(float x, float y, float z, float x2, float y2, float z2, float r, float g, float b)
{
	RenderLine(Vector3(x, y, z), Vector3(x2, y2, z2), Color(r, g, b, 1));

}

DebugLine::DebugLine()
	: drawCount(0)
{
	shader = new Shader(L"20_DebugLine");

	vertices = new VertexColor[MAX_DEBUG_LINE];

	ZeroMemory(vertices, sizeof(VertexColor) * MAX_DEBUG_LINE);

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * MAX_DEBUG_LINE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	D3DXMatrixIdentity(&World);
}

DebugLine::~DebugLine()
{
	SafeDelete(shader);
	SafeDeleteArray(vertices);
	SafeRelease(vertexBuffer);
}

void DebugLine::Render()
{
	
}
