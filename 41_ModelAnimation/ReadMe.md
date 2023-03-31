# Chap41 SkinningModel

- Skinning
    - 모델의 각 정점은 Bone의 위치에 따라서 보정되어 피부를 덮는다.
    - Animation의 변화에 따라 Bone의 상대 위치가 변경되고, 그에 따라 Mesh의 Vertex또한 변함.
    - vertex
        
        ---
        
        - Position
        - BlendIndices (float4)(x, y, z, w)
        - BlendWeights (float4)(x, y, z, w)
        
        ---
        
        - BlendIndices : 정점이 영향을 받을 Bone의 번호.
        - BlendWeights : 정점이 영향받을 본에 대한 가중치.
    - Bone에 영향을 받아 Bone의 이동에 따라 Mesh 표면을 덮을 정점의 위치를 변경한다.
        - 늘어날 부분은 늘어나고, 줄어들 부분은 줄어든다.
    - 일반적으로  해당 정점 근처의 Bone 1~2개의 영향을 받음.
    - e.g.
        
        ---
        
        - V0
            - 1, 0, 0, 0
            - 0, 1, 0 ,0
            - 0, 0, 1, 0
            - 2, 0, 0 ,1
            
        - V1
            - 1, 0, 0, 0
            - 0, 1, 0 ,0
            - 0, 0, 1, 0
            - 0, 2, 0, 1
        - V.r
            - BlendIndices : 0,     1
            - BlendWeight : 0.5, 0.5
            - → v0 * 0.5 +  v1 * 0.5
        - 반드시 BlendWeights의 합은 1이어야 한다.
    - 만약 더 많은 수의 Bone에 영향을 받게 하고 싶다면(blendIndices의 수가 4 이상이 되게 할 경우. 일반적인 경우는 아님.)
        
        ---
        
        - float4 BlendIndices0;
        - float4 BlendIndices1;
        - float4 BlendWeight0;
        - float4 BlendWeight1;
        
        ---
        
    - 의 형태로 작성