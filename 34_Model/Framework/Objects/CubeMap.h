#pragma once


class CubeMap
{
public:
	CubeMap(Shader* shader);
	~CubeMap();

	void Texture(wstring file);

	void Update();
	void Render();


	void Pass(UINT val) { pass = val; };

	Transform* GetTransform() {return mesh->GetTransform(); };
private:

	//MeshCube* mesh;
	MeshSphere* mesh;
	Shader* shader;

	UINT pass = 0;

	ID3D11ShaderResourceView* srv = nullptr;
	ID3DX11EffectShaderResourceVariable* sSrv = nullptr;

};