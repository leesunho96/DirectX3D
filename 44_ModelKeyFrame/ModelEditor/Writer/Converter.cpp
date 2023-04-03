#include "stdafx.h"
#include "Converter.h"
#include "Types.h"
#include "Utilities/BinaryFile.h"
#include "Utilities/Xml.h"

Converter::Converter()
{
	importer = new Assimp::Importer();
}

Converter::~Converter()
{
	SafeDelete(importer);
}

void Converter::ReadFile(wstring file)
{
	this->file = L"../../_Assets/" + file;

	scene = importer->ReadFile
	(
		String::ToString(this->file),
		aiProcess_ConvertToLeftHanded
		| aiProcess_Triangulate
		| aiProcess_GenUVCoords
		| aiProcess_GenNormals
		| aiProcess_CalcTangentSpace
	);
	assert(scene != NULL);
}

void Converter::ExportMesh(wstring savePath)
{
	savePath = L"../../_Models/" + savePath + L".mesh";

	ReadBoneData(scene->mRootNode, -1, -1);
	ReadSkinningData();

	//WriteFile CSV
	{
		FILE* file;
		fopen_s(&file, "../Vertices.csv", "w");

		for (asBone* bone : bones)
		{	
			string name = bone->Name;
			fprintf(file, "%d,%s\n", bone->Index, bone->Name.c_str());
		}

		fprintf(file, "\n");

		fprintf(file, "%s,%s,%s,", string("position.x").c_str(), string("position.y").c_str(), string("position.z").c_str());
		fprintf(file, "%s,%s,%s,%s,",    string("Indices.x").c_str(), string("Indices.y").c_str(), string("Indices.z").c_str(), string("Indices.w").c_str());
		fprintf(file, "%s,%s,%s,%s\n",	string("weights.x").c_str(), string("weights.y").c_str(), string("weights.z").c_str(), string("weights.w").c_str());

		for (asMesh* mesh : meshes)
		{
			string name = mesh->Name;
			printf("%s\n", name.c_str());

			for (UINT i = 0; i < mesh->Vertices.size(); i++)
			{
				Vector3 p = mesh->Vertices[i].Position;
				Vector4 indices = mesh->Vertices[i].BlendIndices;
				Vector4 weights = mesh->Vertices[i].BlendWeights;

				fprintf(file, "%f,%f,%f,", p.x, p.y, p.z);
				fprintf(file, "%f,%f,%f,%f,", indices.x, indices.y, indices.z, indices.w);
				fprintf(file, "%f,%f,%f,%f\n", weights.x, weights.y, weights.z, weights.w);
			}
		}

		fclose(file);
	}
	WriteMeshData(savePath);
}

void Converter::ReadBoneData(aiNode* node, int index, int parent)
{
	asBone* bone = new asBone();
	bone->Index = index;
	bone->Parent = parent;
	bone->Name = node->mName.C_Str();


	Matrix transform(node->mTransformation[0]);
	D3DXMatrixTranspose(&bone->Transform, &transform);

	Matrix matParent;
	if (parent < 0)
		D3DXMatrixIdentity(&matParent);
	else
		matParent = bones[parent]->Transform;

	bone->Transform = bone->Transform * matParent;
	bones.push_back(bone);


	ReadMeshData(node, index);

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadBoneData(node->mChildren[i], bones.size(), index);
}


void Converter::ReadSkinningData() const
{
for (UINT i = 0; i < scene->mNumMeshes; i++)
{
	aiMesh* aiMesh = scene->mMeshes[i];
	if (aiMesh->HasBones() == false) continue;


	asMesh* mesh = meshes[i];

	vector<asBoneWeights> boneWeights;
	boneWeights.assign(mesh->Vertices.size(), asBoneWeights());

	for (UINT b = 0; b < aiMesh->mNumBones; b++)
	{
		aiBone* aiMeshBone = aiMesh->mBones[b];

		UINT boneIndex = 0;
		for (asBone* bone : bones)
		{
			if (bone->Name == (string)aiMeshBone->mName.C_Str())
			{
				boneIndex = bone->Index;

				break;
			}
		}//for(bone)

		for (UINT w = 0; w < aiMeshBone->mNumWeights; w++)
		{
			UINT index = aiMeshBone->mWeights[w].mVertexId;
			float weight = aiMeshBone->mWeights[w].mWeight;

			boneWeights[index].AddWeights(boneIndex, weight);
		}
	}//for(b)


	for (UINT w = 0; w < boneWeights.size(); w++)
	{
		boneWeights[i].Normalize();

		asBlendWeight blendWeight;
		boneWeights[w].GetBlendWeights(blendWeight);

		mesh->Vertices[w].BlendIndices = blendWeight.Indices;
		mesh->Vertices[w].BlendWeights = blendWeight.Weights;
	}
}
}

void Converter::ReadMeshData(aiNode* node, int bone)
{
	if (node->mNumMeshes < 1) return;

	asMesh* mesh = new asMesh();
	mesh->Name = node->mName.C_Str();
	mesh->BoneIndex = bone;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		UINT index = node->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[index];

		aiMaterial* material = scene->mMaterials[srcMesh->mMaterialIndex];
		mesh->MaterialName = material->GetName().C_Str();


		UINT startVertex = mesh->Vertices.size();
		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			Model::ModelVertex vertex;
			memcpy(&vertex.Position, &srcMesh->mVertices[v], sizeof(Vector3));

			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.Uv, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));

			if (srcMesh->HasNormals())
				memcpy(&vertex.Normal, &srcMesh->mNormals[v], sizeof(Vector3));

			mesh->Vertices.push_back(vertex);
		}

		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			aiFace& face = srcMesh->mFaces[f];

			for (UINT k = 0; k < face.mNumIndices; k++)
				mesh->Indices.push_back(face.mIndices[k] + startVertex);
		}

		meshes.push_back(mesh);
	}
}

void Converter::WriteMeshData(wstring savePath)
{
	Path::CreateFolders(Path::GetDirectoryName(savePath));

	BinaryWriter* w = new BinaryWriter();
	w->Open(savePath);

	w->UInt(bones.size());
	for (asBone* bone : bones)
	{
		w->Int(bone->Index);
		w->String(bone->Name);
		w->Int(bone->Parent);
		w->Matrix(bone->Transform);

		SafeDelete(bone);
	}

	w->UInt(meshes.size());
	for (asMesh* meshData : meshes)
	{
		w->String(meshData->Name);
		w->Int(meshData->BoneIndex);

		w->String(meshData->MaterialName);

		w->UInt(meshData->Vertices.size());
		w->Byte(&meshData->Vertices[0], sizeof(Model::ModelVertex) * meshData->Vertices.size());

		w->UInt(meshData->Indices.size());
		w->Byte(&meshData->Indices[0], sizeof(UINT) * meshData->Indices.size());

		SafeDelete(meshData);
	}

	w->Close();
	SafeDelete(w);
}

void Converter::ClipList(vector<wstring>* list)
{
	for (UINT i = 0; i < scene->mNumAnimations; i++)
	{
		aiAnimation* anim = scene->mAnimations[i];

		list->push_back(String::ToWString(anim->mName.C_Str()));
	}
}

void Converter::ExportAnimClip(UINT index, wstring savePath)
{
	savePath = L"../../_Models/" + savePath +L".clip";

	asClip*  clip = ReadClipData(scene->mAnimations[index]);

	WriteClipData(clip, savePath);
}

asClip* Converter::ReadClipData(aiAnimation* animation)
{
	asClip* clip = new asClip();
	clip->Name = animation->mName.C_Str();
	clip->FrameRate = (float)animation->mTicksPerSecond;
	clip->FrameCount = (UINT)animation->mDuration * 1;

	vector<asClipNode> aniNodeInfos;

	for (UINT i = 0; i < animation->mNumChannels; i++)
	{
		aiNodeAnim* animNode = animation->mChannels[i];

		asClipNode animNodeInfo;
		animNodeInfo.Name = animNode->mNodeName;

		UINT keyCount = max(animNode->mNumPositionKeys, animNode->mNumScalingKeys);
		keyCount = max(keyCount, animNode->mNumRotationKeys);

		
		for (UINT j = 0; j < keyCount; j++)
		{
			asKeyframeData frameData;

			bool bFound = false;
			UINT t = animNodeInfo.Keyframe.size();

			// mTime : keyframe의 key가 갖고 있는 시간.
			// 한 프레임의 시간에서 해당 프레임 number를 뺀 수. 해당 값이 D3DX_16_EPSILON(2byte float에 대한 오차값)
			// 해당 오차보다 작으면 해당 프레임으로 판단.
			// 
			if(fabsf((float)animNode->mPositionKeys[j].mTime - (float)t <= D3DX_16F_EPSILON))
			{
				aiVectorKey key = animNode->mPositionKeys[j];
				frameData.Time = (float)key.mTime;
				memcpy_s(&frameData.Translation, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));
				bFound = true;
			}

			if (fabsf((float)animNode->mRotationKeys[j].mTime - (float)t <= D3DX_16F_EPSILON))
			{
				aiQuatKey key = animNode->mRotationKeys[j];
				frameData.Time = (float)key.mTime;
						

				// Quat의 저장 방식이 우리 데이터 형식과 다름
				// aiQuat : W, X, Y, Z
				// Querternion : x, y, z, w
				// 따라서 memcpy사용 불가능.
				frameData.Rotation.x = key.mValue.x;
				frameData.Rotation.y = key.mValue.y;
				frameData.Rotation.z = key.mValue.z;
				frameData.Rotation.w = key.mValue.w;

				bFound = true;
			}

			if (fabsf((float)animNode->mScalingKeys[j].mTime - (float)t <= D3DX_16F_EPSILON))
			{
				aiVectorKey key = animNode->mScalingKeys[j];
				frameData.Time = (float)key.mTime;
				memcpy_s(&frameData.Scale, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));

				bFound = true;
			}
			if(bFound)
				animNodeInfo.Keyframe.push_back(frameData);
		}


		if(animNodeInfo.Keyframe.size() < clip->FrameCount)
		{
			UINT count = clip->FrameCount - animNodeInfo.Keyframe.size();
			asKeyframeData keyframe = animNodeInfo.Keyframe.back();

			for (UINT n = 0; n < count; n++)
			{
				animNodeInfo.Keyframe.push_back(keyframe);
			}
		}
		clip->Duration = max(clip->Duration, animNodeInfo.Keyframe.back().Time);

		aniNodeInfos.push_back(animNodeInfo);
	}//for(i)




	


	ReadKeyFrameData(clip, scene->mRootNode, aniNodeInfos);


	return clip;
}

void Converter::ReadKeyFrameData(asClip* clip, aiNode* node, vector<asClipNode>& aiNodeInfos)
{
	asKeyframe* keyframe = new asKeyframe();


	/*
	 * Retargeting
	 * Bone의 이름을 매칭시키는 과정.
	 * 출처, 제작자 등에 따라 본의 이름이 각각 다를 수 있기 때문에 그에 따라 변경.
	 */
	keyframe->BoneName = node->mName.C_Str();
	for (UINT i = 0; i < clip->FrameCount; i++)
	{
		asClipNode* asClipNode = nullptr;

		/*
		 * 주어진 aiNodeInfos에서 원하는 Node를 찾는 과정.
		 * 이름을 이용하여 찾는다.
		 */
		for (UINT n  = 0; n < aiNodeInfos.size(); n++)
		{
			if(aiNodeInfos[n].Name == node->mName)
			{
				asClipNode = &aiNodeInfos[n];

				break;
			}

		}// for(n)


		asKeyframeData frameData;


		/*
		 * asClipNode == nullptr -> 주어진 이름을 갖는 본에 대한 트랜스폼이 존재하지 않음
		 * --> 부모의 transform에 따라 움직임.
		 * 별도의 움직임을 취하지 않도록 구현.
		 */
		//if(!!asClipNode)
		if(asClipNode == nullptr)
		{
			Matrix transform(node->mTransformation[0]);

			D3DXMatrixTranspose(&transform, &transform);

			frameData.Time = float(i);
			D3DXMatrixDecompose(&frameData.Scale, &frameData.Rotation, &frameData.Translation, &transform);
		}
		else
		{
			frameData = asClipNode->Keyframe[i];
		}

		keyframe->Transforms.push_back(frameData);
	} // for(i)

	clip->Keyframes.push_back(keyframe);



	/*
	 * 재귀를 통해 모든 본에 대한 keyfrme 저장.
	 */
	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ReadKeyFrameData(clip, node->mChildren[i], aiNodeInfos);
	}
}

void Converter::WriteClipData(asClip* clip, wstring savePath)
{
	Path::CreateFolder(Path::GetDirectoryName(savePath));

	BinaryWriter* w = new BinaryWriter();

	w->Open(savePath);

	w->String(clip->Name);
	w->Float(clip->Duration);
	w->Float(clip->FrameRate);
	w->UInt(clip->FrameCount);

	w->UInt(clip->Keyframes.size());

	for (auto keyFrame : clip->Keyframes)
	{
		w->String(keyFrame->BoneName);

		w->UInt(keyFrame->Transforms.size());
		w->Byte(&keyFrame->Transforms[0], sizeof(asKeyframeData) * keyFrame->Transforms.size());

		SafeDelete(keyFrame);
	}

	w->Close();
	SafeDelete(w);
}

void Converter::ExportMaterial(wstring savePath, bool bOverwrite)
{
	savePath = L"../../_Textures/" + savePath + L".material";

	if(!bOverwrite)
	{
		if(Path::ExistFile(savePath))
		{
			return;
		}
	}

	ReadMaterialData();
	WriteMaterialData(savePath);
}

void Converter::ReadMaterialData()
{
	for (UINT i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* srcMaterial = scene->mMaterials[i];
		asMaterial* material = new asMaterial();

		material->Name = srcMaterial->GetName().C_Str();

		aiColor3D color;


		/*
		 * AI_MATKEY_COLOR_AMBIENT : 매개변수 3개의 매크로.
		 * 필요에 따라 원하는 데이터 추출 가능.
		 * AI_MAT_COLOR_TRANSPERANT : alpha 값. 반투명 등 렌더링
		 *
		 * 빛은 불투명 값. Specular.alpha에 빛의 강도를 저장하는 경우가 많음.
		 */
		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);		
		material->Diffuse = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->Ambient = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->Specular = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		material->Emissive = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_SHININESS, material->Specular.a);


		// Material File의 경우는 file Path 반환.
		aiString file;

		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->DiffuseFile = file.C_Str();

		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->SpecularFile = file.C_Str();

		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->NormalFile = file.C_Str();


		


		materials.push_back(material);
	}
}

void Converter::WriteMaterialData(wstring savepath)
{
	string folder = String::ToString(Path::GetDirectoryName(savepath));
	string file = String::ToString(Path::GetFileName(savepath));

	Path::CreateFolder(folder);


	// XML 관리 코드.
	Xml::XMLDocument* document = new Xml::XMLDocument();

	Xml::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	Xml::XMLElement* root = document->NewElement("Materials");
	root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	document->LinkEndChild(root);

	for(auto material : materials)
	{
		Xml::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		Xml::XMLElement* element = nullptr;

		element = document->NewElement("Name");
		element->SetText(material->Name.c_str());		
		node->LinkEndChild(element);

		element = document->NewElement("DiffuseFile");
		element->SetText(WriteTexture(folder, material->DiffuseFile).c_str());;
		node->LinkEndChild(element);

		element = document->NewElement("SpecularFile");
		element->SetText(WriteTexture(folder, material->SpecularFile).c_str());;
		node->LinkEndChild(element);

		element = document->NewElement("NormalFile");
		element->SetText(WriteTexture(folder, material->NormalFile).c_str());;
		node->LinkEndChild(element);
		element = document->NewElement("Ambient");
		element->SetAttribute("R", material->Ambient.r);
		element->SetAttribute("G", material->Ambient.g);
		element->SetAttribute("B", material->Ambient.b);
		element->SetAttribute("A", material->Ambient.a);
		node->LinkEndChild(element);

		element = document->NewElement("Diffuse");
		element->SetAttribute("R", material->Diffuse.r);
		element->SetAttribute("G", material->Diffuse.g);
		element->SetAttribute("B", material->Diffuse.b);
		element->SetAttribute("A", material->Diffuse.a);
		node->LinkEndChild(element);

		element = document->NewElement("Specular");
		element->SetAttribute("R", material->Specular.r);
		element->SetAttribute("G", material->Specular.g);
		element->SetAttribute("B", material->Specular.b);
		element->SetAttribute("A", material->Specular.a);
		node->LinkEndChild(element);

		element = document->NewElement("Emissive");
		element->SetAttribute("R", material->Emissive.r);
		element->SetAttribute("G", material->Emissive.g);
		element->SetAttribute("B", material->Emissive.b);
		element->SetAttribute("A", material->Emissive.a);
		node->LinkEndChild(element);

		SafeDelete(material);
	}





	document->SaveFile((folder + file).c_str());
	SafeDelete(document);

}

string Converter::WriteTexture(string saveFolder, string file)
{
	if(file.length() < 1) return "";

	string fileName = Path::GetFileName(file);

	/*
	 * fbx 파일의 내장 텍스쳐가 존재하면 해당 텍스쳐 반환. 없으면 null
	 */
	const aiTexture* texture = scene->GetEmbeddedTexture(file.c_str());


	string path = "";
	if(texture != nullptr)
	{
		path = saveFolder + fileName;


		/*
		 * Texture의 height가 없다 -> 데이터가 바이트파일로 순서대로 작성
		 * 텍스쳐 크기만큼 파일로 작성해버리면 완료.
		 */
		if(texture->mHeight < 1)
		{
			BinaryWriter w;
			w.Open(String::ToWString(path));
			w.Byte(texture->pcData, texture->mWidth);
			w.Close();
		}
		else
		{
			/*
			 * 해당 방식 파일 현실적으로 존재하지 않음.
			 * 이론상 존재.
			 */
			D3D11_TEXTURE2D_DESC destDesc;
			ZeroMemory(&destDesc, sizeof(D3D11_TEXTURE2D_DESC));
			destDesc.Width = texture->mWidth;
			destDesc.Height = texture->mHeight;
			destDesc.MipLevels = 1;
			destDesc.ArraySize = 1;
			destDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			destDesc.SampleDesc.Count = 1;
			destDesc.SampleDesc.Quality = 0;
			destDesc.Usage = D3D11_USAGE_IMMUTABLE;

			D3D11_SUBRESOURCE_DATA subResource = { 0 };
			subResource.pSysMem = texture->pcData;


			ID3D11Texture2D* dest;

			HRESULT hr;
			hr = D3D::GetDevice()->CreateTexture2D(&destDesc, &subResource, &dest);
			assert(SUCCEEDED(hr));

			D3DX11SaveTextureToFileA(D3D::GetDC(), dest, D3DX11_IFF_PNG, saveFolder.c_str());
		}
	}
	/*
	 * 외장 텍스쳐.
	 * fbx 파일에 텍스쳐가 존재하는것이 아닌 외부에 별도의 텍스쳐 존재.
	 * 해당 텍스쳐 파일 읽어들여서 사용.
	 */
	else
	{
		string directory = Path::GetDirectoryName(String::ToString(this->file));
		string origin = directory + file;
		String::Replace(&origin, "\\", "/");

		if (Path::ExistFile(origin) == false)
			return "";

		path = saveFolder + fileName;
		CopyFileA(origin.c_str(), path.c_str(), FALSE);

		String::Replace(&path, "../../_Textures/", "");
	}

	return Path::GetFileName(path);
}
