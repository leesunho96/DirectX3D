#pragma once



class Mesh : public Renderer
{
public:
	typedef VertexTextureNormal MeshVertex;


public:
	Mesh(Shader* shader);

	virtual ~Mesh();

	void Update();
	void Render();


public:
	void DiffuseMap(wstring file);	

protected:
	virtual void Create() = 0;
private:

// 해당 Protected 영역 변수들은 자식 클래스에서 초기화하여 할당.
protected:
	MeshVertex* vertices = NULL;
	UINT* indices = NULL;	
private:

	Texture* diffuseMap = NULL;
	ID3DX11EffectShaderResourceVariable* sDiffuseMap;
};