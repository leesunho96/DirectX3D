# Chap38 ModelRenderer

CBuffer : c++로 작성한 구조체에서 Padding을 선언 하더라도, 전달되는 버퍼가 배열이 아닌 일반 객체라면 Shader에서 해당 Padding을 선언 할 필요는 없음.

e.g.

- ModelMesh::BoneDesc
    - Matrix Transform[250];
    - UINT Index;
    - float Padding[3];
- Model.fx : cBuffer CB_Bone
    - matrix BoneTransform[250]
    - Uint BoneIndex;

만약 BoneDesc의 배열이 전달 된다면 해당 Padding도 셰이더에서 선언 필요.

Model-Transform(Model의 S, R, T)→ 모델이 그려질 위치

ModelMesh

- BoneIndex→ BoneIndex를 참조하여 Bone의 Transform 사용. 해당 Bone의 부모 본으로부터의 상대 위치.

World = mul(BoneTransform[boneIndex], World);

mul(input.position, World) → World Position;