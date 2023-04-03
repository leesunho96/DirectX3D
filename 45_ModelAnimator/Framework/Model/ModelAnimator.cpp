#include "Framework.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(Shader* shader) : shader(shader)
{
	model = new Model();
	transform = new Transform(shader);
}

ModelAnimator::~ModelAnimator()
{
	SafeDelete(transform);
	SafeDelete(model);
	SafeDelete(shader);
}

void ModelAnimator::Update()
{
	if(texture == nullptr)
	{
		for (auto mesh : model->Meshes())
			mesh->SetShader(shader);

		CreateTexture();
	}

	for (auto mesh : model->Meshes())
	{
		mesh->Update();	
	}
}

void ModelAnimator::Render()
{
	for (auto mesh : model->Meshes())
	{
		mesh->SetTransform(transform);
		mesh->Render();
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

void ModelAnimator::Pass(UINT pass)
{
	for (auto mesh : model->Meshes())
	{
		mesh->Pass(pass);
	}
}

void ModelAnimator::CreateTexture()
{
	clipTransforms = new ClipTransform[model->ClipCount()];


	/*
	 * 
	 * ���� Clip����ŭ Transform����, �ش� Transform�� ���� KeyFrame, ���� �ش� Frame�� Bone Transform
	 */
	for (UINT i = 0; i < model->ClipCount(); i++)
	{
		CreateClipTransform(i);
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
			 * Animation�� ���� ����� ���� Bone�� �θ� Bone�� ���� ��� Transform ���ϴ� ����.
			 * ���� �ش� �����ӿ� ���� ��� transform�� ���ٸ� �������.
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
			 * bones[b] = animation * parent; // animation : Relative, parent : Global() -> �ش� ���� global transform.
			 * ClipTransforms[index].Transform[f][b] = invGlobal * bones[b]
			 *
			 * ���� global * bones[b] -> global * global
			 * ���� global�� inverse�� invGlobal�� global ��ǥ�� bones[b] ����.
			 */
			bones[b] = animation * parent;
			clipTransforms[index].Transform[f][b] = invGlobal * bones[b];
			
		}// for(b)
	}//for(f)
}
