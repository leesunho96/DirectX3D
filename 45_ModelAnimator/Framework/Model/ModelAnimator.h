#pragma once

class ModelAnimator
{
public:
	ModelAnimator(Shader* shader);
	~ModelAnimator();

	void Update();
	void Render();


public:
	void ReadMesh(wstring file);
	void ReadMaterial(wstring file);
	void ReadClip(wstring file);

	Transform* GetTransform() {return transform;};
	Model* GetModel() {return model;};

	void Pass(UINT pass);

private:
	/*
	 * ���� KeyFrame�� ���� �����͸� CBuffer�� ���� �� ���,
	 * 500(MAX_MODEL_KEYFRAME) * 250(MAX_MODEL_TRANSFORM) * 64Byte(4x4MATRIX)
	 * -> �뷫 8MB.
	 * CBuffer�� �ִ� ũ�� : 4996Byte.
	 * ���� CBUffer�� ������ �Ұ����ϴ�.
	 * �׿� ���� Textureȭ �� ��, �ش� Texture�� �����ϴ� ������� ����.
	 */
	void CreateTexture();

	/*
	 * bone->Transform() : �θ� bone�� ���� ���� bone�� ��� transform.
	 * ModelEditor->Converter::ReadBoneData()���� ������.	 * 
	 * ���� transform�� actor�� localTransform�� ���� ��� transform.
	 * invGlobal : presentBone�� inverse : parent Bone�� actor�� ���� ��� Transform.
	 * invGlobal * animation.Transform : actor�� ���� �ش� bone�� Animation�� ����� ��� Transform.
	 */
	void CreateClipTransform(UINT index);
private:

	/*
	 * �ش� ClipTransform�� �� Clip�� ���� BoneTransform �� ����.
	 * transform�� ���� KeyFrame, ���� �ش� Frame�� ���� Bone Transform,
	 * Transform[i] �� Clip[i]
	 */
	struct ClipTransform
	{
	public:
		Matrix** Transform;

	public:
		ClipTransform()
		{
			Transform = new Matrix*[MAX_MODEL_KEYFRAMES];

			for (UINT i = 0; i < MAX_MODEL_KEYFRAMES; i++)
			{
				Transform[i] = new Matrix[MAX_MODEL_TRANSFORMS];
			}
		};
		~ClipTransform()
		{
			for (UINT i = 0; i < MAX_MODEL_TRANSFORMS; i++)
			{
				SafeDelete(Transform[i]);
			}

			SafeDeleteArray(Transform);
		};
	};

	ClipTransform* clipTransforms = nullptr;
	ID3D11Texture2D* texture = nullptr;


	/*
	 * texture. buffer. ��� ID3D11RESOURCE�κ��� ���
	 * -> ��� ID3D11ShaderResourceView�� �̿��Ͽ� RenderingPipelineŸ�� �ʰ� ���� ����.
	 */
	ID3D11ShaderResourceView* srv = nullptr;

public:
private:

	Shader* shader;
	Model* model;
	Transform* transform;

};