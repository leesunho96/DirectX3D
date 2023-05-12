#pragma once

class ModelRender
{
public:
	ModelRender(Shader* shader);
	~ModelRender();

	void Update();
	void Render();

public:
	void ReadMesh(wstring file);
	void ReadMaterial(wstring file);
	
	Model* GetModel() { return model; }

	void Pass(UINT pass);

	// Ư�� Bone�� Matrix ����.
	void UpdateTransform(UINT instsanceId, UINT boneIndex, Transform& transform);
public:
	Transform* AddTransform();
	Transform* GetTransform(UINT index) { return transforms[index]; }

	// ��ü ��ġ�� GPU�� ������ �Լ�
	void UpdateTransforms();
private:

	void CreateTexture();
private:
	Shader* shader;
	Model* model;

	vector<Transform *> transforms;
	Matrix worlds[MAX_MODEL_INSTANCE];

	VertexBuffer* instanceBuffer;

	Matrix boneTransforms[MAX_MODEL_INSTANCE][MAX_MODEL_TRANSFORMS];

	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* srv;
	
};