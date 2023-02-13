#pragma once

constexpr int MAX_DEBUG_LINE = 150000;


class DebugLine
{
public:

	// 
	friend class Window;

	static void Create();
	static void Delete();

	static DebugLine* Get();
private:

	void RenderLine(Vector3& start, Vector3& end);
	void RenderLine(Vector3& start, Vector3& end, float r, float g, float b);
	void RenderLine(Vector3& start, Vector3& end, Color& color);

	void RenderLine(float x, float y, float z, float x2, float y2, float z2);
	void RenderLine(float x, float y, float z, float x2, float y2, float z2, Color& color);
	void RenderLine(float x, float y, float z, float x2, float y2, float z2, float r, float g, float b);


private:

	DebugLine();
	~DebugLine();

	void Render();

private:

	static DebugLine* debugLine;
private:
	Shader* shader;

	Matrix World;
	ID3D11Buffer* vertexBuffer;
	VertexColor* vertices;


	UINT drawCount = 0;
};