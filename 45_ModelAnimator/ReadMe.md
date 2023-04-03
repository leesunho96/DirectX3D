# Chap45 Animation

- Amimation :
    - cBuffer 사용 불가능
        - 현재 프로젝트 기준 MAX_MODEL_KEYFRAME : 250, MAX_MODEL_TRANSFORM : 500
        - 250 * 500 * 64(matrix 4x4) → 대략 하나의 애니메이션에 8MB.
        - cBUffer의 최대 크기 : 4996byte.
        - 해당 문제를 해결하기 위해서 데이터를 Texture화. (texture는 사실상 용량 제한이 없다. GB급이 되지 않는 이상.)
    - Texture화 한 값에는 애니메이션이 계산 완료된 행렬을 저장.
        - clipTransform* clipTransforms;의 형태로 저장.
        - clipTransform : Matrix** transform.
        - clipTransform::Matrix* transform : keyframe.
        - clipTransform::Matrix** transform : 해당 keyFrame에 대한 Global Transform.
    - clipTransform : textureArray → Clip
    - textureArray : 행(keyFrame), 열(transforms).
    
    - ModelAnimator::CreateClipTransform()
        - bone→Transform() : Global Transform
        - invGlobal : inverse(bone→Transform) → 현재 bone의 부모 bone에 대한 상대 transform.
        - animation→translation() : 부모 본에 대한 상대 transform.
            - 만약 해당 프레임에 적용될 transform이 없다면 정방행렬 적용.
        - parent = bone→transform(parentIndex); // 부모 bone의 global transform.
        - bones[b] = animation * parent // animation(relative) * parent(global) → Global Transform.
        - ClipTransforms[index].Transform[f][b] = invGlobal * bones[b]; // bones[b] : Global Transform, 만약 parent * bones[b] : Global Transform * global transform : 연산하는 좌표계가 다르기 때문에 예상외의 결과가 나올 것. → invGlobal * bones[b] 하여 Global 좌표계 연산으로 만들어줌.
    
- ID3DShaderResourceView
    - ID3D11Resource 싱속한 모든 객체에 대하여 Rendering Pipeline를 거치지 않은 관리 가능.
        - ID3D11Textire
        - ID3D11Buffer 등