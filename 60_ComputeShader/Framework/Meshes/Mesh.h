#pragma once

class Mesh
{
public:
	typedef VertexTextureNormal MeshVertex;

public:
	Mesh();
	virtual ~Mesh();

	void SetShader(Shader* shader);
	void Pass(UINT val) {pass = val;};


	void Update();
	void Render(UINT drawCount);

protected:
	virtual void Create() = 0;

protected:
	Shader* shader;
	UINT pass;

	PerFrame* perFrame = nullptr;

	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;
	
	MeshVertex* vertices = NULL;
	UINT* indices = NULL;

	UINT vertexCount, indexCount;
private:

};