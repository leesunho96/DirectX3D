# Chap37 ModelRender

- Model
    - ModelMesh
        - ModelBone : Bone Data
        - ModelMesh : Drawing. ModelBone 참조
    - ModelRender : MeshRender
        - shader→CBuffer(Bone배열 전체, 해당 mesh 참조하는 bone Index) // Animation 호환성 유지 위해 배열 전체 전달.