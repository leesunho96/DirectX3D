#pragma once
#define MAX_MESH_INSTANCING 500

class MeshRender
{
public:
	MeshRender(Shader* shader, Mesh* mesh);
	~MeshRender();

	Mesh* GetMesh() { return mesh; }

	void Update();
	void Render();

	void Pass(UINT val) { mesh->Pass(val); }

	Transform* AddTransform();
	Transform* GetTransform(UINT index) { return transforms[index]; }
	void UpdateTransforms();

private:
	Mesh* mesh;


	// transforms.size() : instancing size
	vector<Transform *> transforms;
	Matrix worlds[MAX_MESH_INSTANCING];

	VertexBuffer* instanceBuffer;


};