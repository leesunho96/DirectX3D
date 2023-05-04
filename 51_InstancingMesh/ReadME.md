# 51_Instancing Mesh

- Render 기능을 MeshRender 클래스에 몰아서 구현.
- MeshRender::AddTransform() 이용하여 Transform 추가.// MAX 500
- MeshRender::UpdateTransform() :: MeshRender 내부 변수 vector<Transform* > transforms를 Worlds[] 로 전달, GPU로 mapping.

- #hlsl
    - ‘SV_… ‘ : System Value.
        - pre-defined Value : pipeline에 미리 정의되어 있는 변수를 의미.
        - 추후에 자동으로 사라지므로, 해당 변수와 SV가 붙지 않은 변수들 사이의 이름을 달라야 함.
        
- Instancing In UE
    - UE에서 Render : material 별로 진행.
    - 동일한 material, 동일한 Mesh를 소유시 instancing 사용.
    - 만약 동일한 mesh이더라도, 특정 개체에 별도의 기능 추가시 Material/Dynamic Material Instance를 이용하여 구분,
    
    - case 1: 액터의 피격에 의한 색 변경 시도.
        - case 1-1. 동일한 색상으로 변경 시도 : Instancing 이용하여 하나의 Material Instance / Dynamic Material Instance 생성하여 처리 가능.
        - case 1-2. 만약 피격 시간 경과에 따른 색상 변경 시도 : Instancing 이용 할 시, 각 객체들의 Index와 경과 시간의 Index를 matching 시켜서 inxtancing 가능. 하지만 기본적으로  instancing은 대량의 매시를 동일한 데이터로 render 시 효과가 있음. 추천하지는 않음. 그냥 별도의 Material 생성하여 처리.
        
    - case 2: Light
        - 조명 처리시 기본적으로 동일한 값을 액터들 사이에 공유 → Instancing 사용.
        
    - case 3: Mesh가 다른 경우
        - 만약 Mesh간 동일한 vertex layout(vertex number/vertex attribute)시 Instancing 가능.
        - 해당 경우는 극히 희박 → 별도의 rendering.