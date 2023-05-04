#pragma once

class ModelAnimator
{
public:
	ModelAnimator(Shader* shader);
	~ModelAnimator();
private:
	void UpdateTweenMode();
	void UpdateBlendMode();
public:
	void Update();
	void Render();


public:
	void ReadMesh(wstring file);
	void ReadMaterial(wstring file);
	void ReadClip(wstring file);

	Transform* GetTransform() {return transform;};
	Model* GetModel() {return model;};

	void Pass(UINT pass);

	void PlayTweenMode(UINT clip, float speed = 1.0f, float takeTime = 1.0f);
	void PlayBlendMode(UINT clip1, UINT clip2, UINT clip3);
	void SetBlendAlpha(float alpha);
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

private:
	struct KeyFrameDesc
	{
		// ���� clip ��
		int Clip = 0;
		// ���� ������ ��ȣ
		UINT CurrFrame = 0;
		// ���� ������ ��ȣ. �Ϲ������� currFrame + 1
		UINT NextFrame = 0;

		float Time = 0.0f;
		float RunningTime = 0.0f;

		float Speed = 1.0f;

		Vector2 Padding;
	}; //keyFrameDesc;


	ConstantBuffer* frameBuffer;
	ID3DX11EffectConstantBuffer* sFrameBuffer;

	struct TweenDesc
	{
		float TakeTime = 1.0f;
		float TweenTime = 0.0f;
		float ChangeTime = 0.0f;
		float Padding;

		KeyFrameDesc Curr;
		KeyFrameDesc Next;

		TweenDesc()
		{
			Curr.Clip = 0;
			Next.Clip = -1;
		}

	} tweenDesc;


private:
	struct BlendDesc
	{
		// 0���� ũ�� blend Mode, 0�̸� TweenMode
		UINT Mode = 0;;
		float Alpha = 0;;
		Vector2 Padding;

		KeyFrameDesc Clip[3];
	} blendDesc;


	ConstantBuffer* blendBuffer;
	ID3DX11EffectConstantBuffer* sBlendBuffer;
private:

	Shader* shader;
	Model* model;
	Transform* transform;

};
