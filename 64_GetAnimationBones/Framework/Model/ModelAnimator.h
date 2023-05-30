#pragma once
class ModelAnimator
{
public:
	ModelAnimator(Shader* shader);
	~ModelAnimator();
private:
	void UpdateTweenMode(UINT index);
	void UpdateBlendMode(UINT index);
public:
	void Update();
	void Render();


public:
	void ReadMesh(wstring file);
	void ReadMaterial(wstring file);
	void ReadClip(wstring file);

	
	Model* GetModel() {return model;};
	Transform* AddTransform();
	Transform* GetTransform(UINT index) {return transforms[index]; };
	void UpdateTransforms();

	void Pass(UINT pass);

	void PlayTweenMode(UINT index, UINT clip, float speed = 1.0f, float takeTime = 1.0f);
	void PlayBlendMode(UINT index, UINT clip1, UINT clip2, UINT clip3);
	void SetBlendAlpha(UINT index, float alpha);
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

	void CreateComputeBuffer();
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

private:
	struct KeyFrameDesc
	{
		// 현재 clip 수
		int Clip = 0;
		// 현재 프레임 번호
		UINT CurrFrame = 0;
		// 다음 프레임 번호. 일반적으로 currFrame + 1
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

	} tweenDesc[MAX_MODEL_INSTANCE];

private:
	struct BlendDesc
	{
		// 0보다 크면 blend Mode, 0이면 TweenMode
		UINT Mode = 0;;
		float Alpha = 0;;
		Vector2 Padding;

		KeyFrameDesc Clip[3];
	} blendDesc[MAX_MODEL_INSTANCE];


	ConstantBuffer* blendBuffer;
	ID3DX11EffectConstantBuffer* sBlendBuffer;
private:

	Shader* shader;
	Model* model;

	vector<Transform * > transforms;
	Matrix worlds[MAX_MODEL_INSTANCE];

	VertexBuffer* instanceBuffer;



private:
	// compute shader로 입력될 구조체
	struct CS_InputDesc
	{
		Matrix Bone;
	};

	struct CS_OutputDesc
	{
		Matrix Result;
	};

	struct AttachDesc
	{
		UINT BoneIndex = 40;
		float Padding[3];
	} attachDesc;

private:
	Shader* computeShader = nullptr;
	StructuredBuffer* computeBuffer = nullptr;

	CS_InputDesc*  csInput = nullptr;
	CS_OutputDesc* csOutput = nullptr;

	ID3DX11EffectShaderResourceVariable* sInputSRV;
	ID3DX11EffectUnorderedAccessViewVariable* sOutputUAV;

	ConstantBuffer* computeAttachBuffer = nullptr;

	

	ID3DX11EffectConstantBuffer* sComputeAttachBuffer;
	ID3DX11EffectConstantBuffer* sComputeTweenBuffer;
	ID3DX11EffectConstantBuffer* sComputeBlendBuffer;


	;




};
