# Chap 52 Instancing Skeletal Mesh

- Shader
    - Header 중복 Include시 변수 중복 발생 error.
    - Shader에서의 Include는 단순한 코드 치환.
    - 동일한 Rendering Pipeline일 시, structure 가 include되어 있지 않더라도, 참조 가능.
        - 하나의 렌더링 파이프라인을 공유 할 시, 명시적으로 include 하지 않더라도, 두개의 셰이더는 영역을 공유함.
    
- Instancing
    - Texture화 하여 데이터를 전달.
    - Instance Buffer → World.
        - 각 칸 : Bone Index
        - 행 : Instance 번호.
        - 열 : Bone의 정보.
        - 250 X 500 X 64 byte → Texture화 필요.
        
        ```cpp
        void ModelRender::CreateTexture()
        {
        	D3D11_TEXTURE2D_DESC desc;
        	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
        	desc.Width = MAX_MODEL_TRANSFORMS * 4;
        	desc.Height = MAX_MODEL_INSTANCE;
        	desc.ArraySize = 1;
        	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 한 픽셀당 16byte. 하나의 
        	desc.Usage = D3D11_USAGE_DYNAMIC;
        	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        	desc.MipLevels = 1;
        	desc.SampleDesc.Count = 1;
        
        	Matrix bones[MAX_MODEL_TRANSFORMS];
        
        	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
        	{
        		for (UINT b = 0; b < model->BoneCount(); b++)
        		{
        			ModelBone* bone = model->BoneByIndex(b);
        
        			Matrix parent;
        			int parentIndex = bone->ParentIndex();
        
        			if(parentIndex < 0)
        				D3DXMatrixIdentity(&parent);
        			else
        				parent = bones[parentIndex];
        
        			Matrix matrix = bone->Transform();
        			bones[b] = parent;
        
        			boneTransforms[i][b] = matrix * bones[b];
        		} // for(b)
        	} // for(i)
        
        	D3D11_SUBRESOURCE_DATA subResource;
        	subResource.pSysMem = boneTransforms;
        	subResource.SysMemPitch = MAX_MODEL_TRANSFORMS * sizeof(Matrix);
        	subResource.SysMemSlicePitch = MAX_MODEL_TRANSFORMS * sizeof(Matrix) * MAX_MODEL_INSTANCE;
        
        	Check(D3D::GetDevice()->CreateTexture2D(&desc, &subResource, &texture));
        
        	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        	srvDesc.Texture2D.MipLevels = 1;
        	srvDesc.Format = desc.Format;
        
        	Check(D3D::GetDevice()->CreateShaderResourceView(texture, &srvDesc, &srv));
        
        	for (auto mesh : model->Meshes())
        	{
        		mesh->TransformsSRV(srv);
        	}
        }
        ```