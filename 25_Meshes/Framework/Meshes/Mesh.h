#pragma once



class Mesh
{
public:
	typedef VertexTextureNormal MeshVertex;


public:
	Mesh(Shader* shader);

	virtual ~Mesh();

	void Update();
	void Render();


public:
	void Pass(UINT val) {pass = val;};

	void Position(float x, float y, float z);
	void Position(Vector3& vec);
	void Position(Vector3* vec);

	void Rotation(float x, float y, float z);
	void Rotation(Vector3& vec);
	void Rotation(Vector3* vec);

	void RotationDegree(float x, float y, float z);
	void RotationDegree(Vector3& vec);
	void RotationDegree(Vector3* vec);

	void Scale(float x, float y, float z);
	void Scale(Vector3& vec);
	void Scale(Vector3* vec);


	Matrix World() { return world; }

	Vector3 Forward();
	Vector3 Up();
	Vector3 Right();

	void DiffuseMap(wstring file);	

protected:

	// Method That Create Itself Vertex
	virtual void Create() = 0;
	void CreateBuffer();

private:

	void UpdateWorld();





// 해당 Protected 영역 변수들은 자식 클래스에서 초기화하여 할당.
protected:
	MeshVertex* vertices = NULL;
	UINT* indices = NULL;

	UINT vertexCount;
	UINT indexCount;

private:
	Shader* shader;
	UINT pass;

	Vector3 position = Vector3(0, 0, 0);
	Vector3 rotation = Vector3(0, 0, 0);
	Vector3 scale = Vector3(1, 1, 1);

	Matrix world;

	ID3D11Buffer* vertexBuffer = NULL;
	ID3D11Buffer* indexBuffer = NULL;

	ID3DX11EffectMatrixVariable *sWorld, *sView, *sProjection;
	ID3DX11EffectShaderResourceVariable* sSrv;

	Texture* texture = NULL;
};