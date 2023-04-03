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
	 * 만약 KeyFrame의 각각 데이터를 CBuffer로 전달 할 경우,
	 * 500(MAX_MODEL_KEYFRAME) * 250(MAX_MODEL_TRANSFORM) * 64Byte(4x4MATRIX)
	 * -> 대략 8MB.
	 * CBuffer의 최대 크기 : 4996Byte.
	 * 따라서 CBUffer로 전달은 불가능하다.
	 * 그에 따라 Texture화 한 후, 해당 Texture를 전달하는 방식으로 구현.
	 */
	void CreateTexture();

	/*
	 * bone->Transform() : 부모 bone에 대한 현재 bone의 상대 transform.
	 * ModelEditor->Converter::ReadBoneData()에서 구해짐.	 * 
	 * 현재 transform은 actor의 localTransform에 대한 상대 transform.
	 * invGlobal : presentBone의 inverse : parent Bone의 actor에 대한 상대 Transform.
	 * invGlobal * animation.Transform : actor에 대한 해당 bone의 Animation이 적용된 상대 Transform.
	 */
	void CreateClipTransform(UINT index);
private:

	/*
	 * 해당 ClipTransform은 한 Clip에 대한 BoneTransform 들 저장.
	 * transform의 행은 KeyFrame, 열은 해당 Frame에 대한 Bone Transform,
	 * Transform[i] 는 Clip[i]
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
	 * texture. buffer. 모두 ID3D11RESOURCE로부터 상속
	 * -> 모두 ID3D11ShaderResourceView를 이용하여 RenderingPipeline타지 않고 전달 가능.
	 */
	ID3D11ShaderResourceView* srv = nullptr;

public:
private:

	Shader* shader;
	Model* model;
	Transform* transform;

};