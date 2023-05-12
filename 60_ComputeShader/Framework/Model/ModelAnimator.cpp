#include "Framework.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(Shader* shader)
	: shader(shader)
{
	model = new Model();
	

	frameBuffer = new ConstantBuffer(&tweenDesc, sizeof(TweenDesc) * MAX_MODEL_INSTANCE);
	sFrameBuffer = shader->AsConstantBuffer("CB_TweenFrame");

	blendBuffer = new ConstantBuffer(&blendDesc, sizeof(BlendDesc) * MAX_MODEL_INSTANCE);
	sBlendBuffer = shader->AsConstantBuffer("CB_BlendFrame");

	instanceBuffer = new VertexBuffer(worlds, MAX_MODEL_INSTANCE, sizeof(Matrix), 1, true);
}

ModelAnimator::~ModelAnimator()
{
	SafeDelete(model);
	

	SafeDeleteArray(clipTransforms);
	SafeRelease(texture);
	SafeRelease(srv);

	SafeDelete(frameBuffer);
	SafeDelete(blendBuffer);

	SafeDelete(instanceBuffer);
}

void ModelAnimator::Update()
{
	for (UINT i = 0; i < transforms.size(); i++)
	{
		if (blendDesc[i].Mode == 0)
			UpdateTweenMode(i);
		else
			UpdateBlendMode(i);
	}

	if (texture == NULL)
	{
		for (ModelMesh* mesh : model->Meshes())
			mesh->SetShader(shader);

		CreateTexture();
	}

	for (ModelMesh* mesh : model->Meshes())
		mesh->Update();
}

void ModelAnimator::UpdateTweenMode(UINT index)
{
	TweenDesc& desc = tweenDesc[index];

	//현재 애니메이션
	{
		ModelClip* clip = model->ClipByIndex(desc.Curr.Clip);

		desc.Curr.RunningTime += Time::Delta();

		float time = 1.0f / clip->FrameRate() / desc.Curr.Speed;
		if (desc.Curr.Time >= 1.0f)
		{
			desc.Curr.RunningTime = 0;

			desc.Curr.CurrFrame = (desc.Curr.CurrFrame + 1) % clip->FrameCount();
			desc.Curr.NextFrame = (desc.Curr.CurrFrame + 1) % clip->FrameCount();
		}
		desc.Curr.Time = desc.Curr.RunningTime / time;
	}

	if (desc.Next.Clip > -1)
	{
		desc.ChangeTime += Time::Delta();
		desc.TweenTime = desc.ChangeTime / desc.TakeTime;

		if (desc.TweenTime >= 1.0f)
		{
			desc.Curr = desc.Next;

			desc.Next.Clip = -1;
			desc.Next.CurrFrame = 0;
			desc.Next.NextFrame = 0;
			desc.Next.Time = 0;
			desc.Next.RunningTime = 0.0f;

			desc.ChangeTime = 0.0f;
			desc.TweenTime = 0.0f;
		}
		else
		{
			ModelClip* clip = model->ClipByIndex(desc.Next.Clip);

			desc.Next.RunningTime += Time::Delta();

			float time = 1.0f / clip->FrameRate() / desc.Next.Speed;
			if (desc.Next.Time >= 1.0f)
			{
				desc.Next.RunningTime = 0;

				desc.Next.CurrFrame = (desc.Next.CurrFrame + 1) % clip->FrameCount();
				desc.Next.NextFrame = (desc.Next.CurrFrame + 1) % clip->FrameCount();
			}
			desc.Next.Time = desc.Next.RunningTime / time;
		}
	}
}

void ModelAnimator::UpdateBlendMode(UINT index)
{
	BlendDesc& desc = blendDesc[index];

	for (UINT i = 0; i < 3; i++)
	{
		ModelClip* clip = model->ClipByIndex(desc.Clip[i].Clip);

		desc.Clip[i].RunningTime += Time::Delta();

		float time = 1.0f / clip->FrameRate() / desc.Clip[i].Speed;
		if (desc.Clip[i].Time >= 1.0f)
		{
			desc.Clip[i].RunningTime = 0;

			desc.Clip[i].CurrFrame = (desc.Clip[i].CurrFrame + 1) % clip->FrameCount();
			desc.Clip[i].NextFrame = (desc.Clip[i].CurrFrame + 1) % clip->FrameCount();
		}
		desc.Clip[i].Time = desc.Clip[i].RunningTime / time;
	}
}

void ModelAnimator::Render()
{
	frameBuffer->Render();
	sFrameBuffer->SetConstantBuffer(frameBuffer->Buffer());

	blendBuffer->Render();
	sBlendBuffer->SetConstantBuffer(blendBuffer->Buffer());

	instanceBuffer->Render();

	for (ModelMesh* mesh : model->Meshes())
	{		
		mesh->Render(transforms.size());
	}
}

void ModelAnimator::ReadMesh(wstring file)
{
	model->ReadMesh(file);
}

void ModelAnimator::ReadMaterial(wstring file)
{
	model->ReadMaterial(file);
}

void ModelAnimator::ReadClip(wstring file)
{
	model->ReadClip(file);
}

Transform* ModelAnimator::AddTransform()
{
	Transform* transform = new Transform();
	transforms.push_back(transform);

	return transform;
}

void ModelAnimator::UpdateTransforms()
{
	for (UINT i = 0; i < transforms.size(); i++)
	{
		memcpy(worlds[i], transforms[i]->World(), sizeof(Matrix));
	}
	D3D11_MAPPED_SUBRESOURCE subResource;

	D3D::GetDC()->Map(instanceBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, worlds, sizeof(Matrix) * MAX_MESH_INSTANCING);
	}
	D3D::GetDC()->Unmap(instanceBuffer->Buffer(), 0);
}


void ModelAnimator::Pass(UINT pass)
{
	for (ModelMesh* mesh : model->Meshes())
		mesh->Pass(pass);
}

void ModelAnimator::PlayTweenMode(UINT index, UINT clip, float speed, float takeTime)
{
	blendDesc[index].Mode = 0;
	tweenDesc[index].TakeTime = takeTime;
	tweenDesc[index].Next.Clip = clip;
	tweenDesc[index].Next.Speed = speed;
}

void ModelAnimator::PlayBlendMode(UINT index, UINT clip1, UINT clip2, UINT clip3)
{
	blendDesc[index].Mode = 1;	
	blendDesc[index].Clip[0].Clip = clip1;
	blendDesc[index].Clip[1].Clip = clip2;
	blendDesc[index].Clip[2].Clip = clip3;
}

void ModelAnimator::SetBlendAlpha(UINT index, float alpha)
{
	alpha = Math::Clamp(alpha, 0.0f, 2.0f);

	blendDesc[index].Alpha = alpha;
}


void ModelAnimator::CreateTexture()
{
	clipTransforms = new ClipTransform[model->ClipCount()];
	/*
	 * 
	 * 모델의 Clip수만큼 Transform생성, 해당 Transform의 행은 KeyFrame, 열은 해당 Frame의 Bone Transform
	 */
	for (UINT i = 0; i < model->ClipCount(); i++)
	{
		CreateClipTransform(i);
	}

	//Create Texture
	{
		D3D11_TEXTURE2D_DESC desc;

		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		/*
		 * 하나의 픽셀 최대값 : R32G32B32A32 -> 16byte.
		 * 우리가 사용할 transform : matrix : 4x4 float -> 64byte.
		 * 따라서 하나의 매트릭스 표현을 위해 4개의 픽셀 데이터 필요.
		 * -> Width = MAX_MODEL_KEYFRAMES * 4가 될 것. 
		 */
		desc.Width = MAX_MODEL_KEYFRAMES * 4;
		desc.Height = MAX_MODEL_TRANSFORMS;
		desc.ArraySize = model->ClipCount();

		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 16byte * 4 -> 64byte. Matrix;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // SRV를 통해서 전달.
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1; // MSAA


		
		UINT pageSize = desc.Width * desc.Height * 16;

		/*
		 * malloc의 복사 가능 량은 stack frame의 영향을 받음. 대략 2MB.
		 */

		//void * p = malloc(pageSize * model->ClipCount());

		// MEMORY_BASIC_INFROMATION, VirtualQuery

		// MEM_RESERVE : 메모리 예약
		void * p = VirtualAlloc(NULL, pageSize * model->ClipCount(), MEM_RESERVE, PAGE_READWRITE);

		for (UINT c = 0; c < model->ClipCount(); c++)
		{
			UINT start = pageSize * c;

			for (UINT k = 0; k < MAX_MODEL_KEYFRAMES; k++)
			{
				void* temp = (BYTE*)p + MAX_MODEL_TRANSFORMS * k * sizeof(Matrix) + start;

				// MEM_COMMIT : 메모리 실제 할당.
				VirtualAlloc(temp, MAX_MODEL_TRANSFORMS * sizeof(Matrix), MEM_COMMIT, PAGE_READWRITE);

				memcpy(temp, clipTransforms[c].Transform[k], MAX_MODEL_TRANSFORMS * sizeof(Matrix));
			}
		}// for(c)


		D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[model->ClipCount()];

		for (UINT c = 0; c < model->ClipCount(); c++)
		{
			void* temp = (BYTE*)p + c * pageSize;

			subResource[c].pSysMem = temp;
			subResource[c].SysMemPitch = MAX_MODEL_TRANSFORMS * sizeof(Matrix);
			subResource[c].SysMemSlicePitch = pageSize;
		}
		Check(D3D::GetDevice()->CreateTexture2D(&desc, subResource, &texture));

		SafeDeleteArray(subResource);
		VirtualFree(p, 0, MEM_RELEASE);
	}

	// Create SRV

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		// 위의 Texture의 데이터 형식과 일치.
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = model->ClipCount();

		Check(D3D::GetDevice()->CreateShaderResourceView(texture, &desc, &srv));
	}

	for (ModelMesh* mesh : model->Meshes())
	{
		mesh->TransformsSRV(srv);
	}
}

void ModelAnimator::CreateClipTransform(UINT index)
{
	Matrix* bones = new Matrix[MAX_MODEL_TRANSFORMS];

	ModelClip* clip = model->ClipByIndex(index);


	// f : frameCount
	for (UINT f = 0; f < clip->FrameCount(); f++)
	{
		// b = boneCount
		for (UINT b = 0;  b < model->BoneCount(); b++)
		{
			ModelBone* bone = model->BoneByIndex(b);

			Matrix parent;
			Matrix invGlobal = bone->Transform();
			D3DXMatrixInverse(&invGlobal, NULL, &invGlobal);

			int parentIndex = bone->ParentIndex();

			if(parentIndex < 0)
				D3DXMatrixIdentity(&parent);
			else
				parent = bones[parentIndex];

			Matrix animation;
			ModelKeyframe* frame = clip->Keyframe(bone->Name());


			/*
			 * Animation에 의해 적용될 현재 Bone의 부모 Bone에 대한 상대 Transform 구하는 과정.
			 * 만약 해당 프레임에 대한 상대 transform이 없다면 정방행렬.
			 */
			if(frame != nullptr)
			{
				ModelKeyframeData& data = frame->Transforms[f];

				Matrix S, R, T;
				D3DXMatrixScaling(&S, data.Scale.x, data.Scale.y, data.Scale.z);
				D3DXMatrixRotationQuaternion(&R, &data.Rotation);
				D3DXMatrixTranslation(&T,data.Translation.x, data.Translation.y, data.Translation.z);

				animation = S * R * T;


			}
			else
			{
				D3DXMatrixIdentity(&animation);
			}

			/*
			 * bones[b] = animation * parent; // animation : Relative, parent : Global() -> 해당 값은 global transform.
			 * ClipTransforms[index].Transform[f][b] = invGlobal * bones[b]
			 *
			 * 만약 global * bones[b] -> global * global
			 * 따라서 global의 inverse인 invGlobal에 global 좌표인 bones[b] 곱함.
			 */
			bones[b] = animation * parent;
			clipTransforms[index].Transform[f][b] = invGlobal * bones[b];
			
		}// for(b)
	}//for(f)
}
