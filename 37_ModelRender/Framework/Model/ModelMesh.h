#pragma once

class ModelBone
{
public:
	friend class Model;

public:
	int Index() const {return index;};

	int ParentIndex() const {return parentIndex;};
	ModelBone* Parent() const {return parent;};

	wstring Name() const {return name;};

	Matrix& Transform() {return transform;};
	void Transform(Matrix& matrix) {transform = matrix;};

	vector<ModelBone*>& Childs() {return child;};

private:
	/*
	 * Model에서만 접근 가능하게 하도록 private 설정.
	 */
	ModelBone();
	~ModelBone();



private:
	int index;
	wstring name;

	int parentIndex;
	ModelBone* parent;

	Matrix transform;
	vector<ModelBone*> child;
};

///////////////////////////////////////////////////////////////////////


class ModelMesh
{
public:
	friend class Model;
private:
	ModelMesh();
	~ModelMesh();

	// Material 때문에 받음.
	void Binding(Model* model);

public:
	void Pass(UINT val) {pass = val;};
	void SetShader(Shader* shader);

	void Update();
	void Render();

	wstring Name() const {return name;};

	int BoneIndex() const {return boneIndex;};
	class ModelBone* Bone() const {return bone;};

	void Transforms(Matrix* transform);
	void SetTransform(Transform* trasnform);



private:
	struct BoneDesc
	{
		Matrix Transform[MAX_MODEL_TRANSFORMS];

		UINT Index;
		float Padding[3];
	}boneDesc;

private:
	wstring name;

	Shader* shader;
	UINT pass = 0;

	Transform* transform = nullptr;
	PerFrame* perFrame = nullptr;

	wstring materialName = L"";

	int boneIndex;
	class ModelBone* bone;

	VertexBuffer* vertexBuffer;
	UINT vertexCount;
	Model::ModelVertex* vertices;

	IndexBuffer* indexBuffer;
	UINT indexCount;
	UINT* indices;


	ConstantBuffer* boneBuffer;
	ID3DX11EffectConstantBuffer* sBoneBuffer;
};
