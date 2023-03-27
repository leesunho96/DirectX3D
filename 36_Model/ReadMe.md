# Chap 36 Model

일반적인 3D Model은 fbx, 3dx, obj, md5 등 이용.

이 파일들을 생성하는 프로그램은 3dMAX, Maya, Blender 등 존재하며, 이중 3dmax, maya는 autodesk 소유 프로그램, blender는 opensource.

위의 FBX, 3DX, obj, md5 파일들은 모두 Assimp라는 라이브러리 이용하여 동일한 Format으로 반환 가능.

FBX가 사실상 현재 표준이며, UE/Unity 모두 사용.

Assimp는 Material/Mesh/SceneRoot/Light/Camera 모두 소유.

이 데이터 전체를 사용하면 느려기며, 우리는 Light/Camera를 직접 구현 하기 때문에 해당 Assimp의 값을 이용하여 우리 자체 구조체로 사용할 것.

FrameWork(Model을 읽어들이고, 해당 Model Rendering 할 프로젝트)

← UnitTest(Level 개념)

← ModelEditor(Model File(.mesh) 생성. FrameWork에서 사용할 Model임)

← Assimp(Assimp→Variable→File)

추후, MapEditor 등 추가적으로 Editor 생성.

- 3DAssimp : OpenSource로, obj, fbx 등 3d model 파일로부터 데이터를 읽어들여 동일한 포맷으로 반환.
- Assimp::Importer : Scene를 반환. 해당 Scene는 3d Model의 모든 정보를 갖는 객체.
- Scene
    
    ---
    
    우리가 사용하는 데이터들
    
    - Materials : Array형태로 존재. 해당 모델이 사용하는 모든 Material 저장.
        - 색상
        - 텍스쳐
    - Meshes : Array 형태로 존재. 해당 모델이 사용하는 모든 Mesh 저장.
        - 그려질 정보
    - SceneRoot
        - Body
            - Material (Scene→Material의 Index)
            - Meshes (Scene→Meshes의 Index)
                - Vertices
                - indices
            - Transformation : 부모/자식 간의 관계 유지 목적. 상대 Transform 정의
        - e.g. TANK
            - body
                - wheel1
                - wheel2
                - Tower
                    - …
                    - …
    
    ---
    
    우리가 사용하지 않는 데이터들
    
    - Camera
    - Light
    
    ---
    
- Assimp 파일은 aiName의 형태로 구성.
- Assimp파일은 오른손 좌표계, 열우선 형태로 저장한다. 그러나 우리가 만드는 DX 파일은 왼손 좌표계, 횡우선 Matrix 형태를 사용한다.
    - 따라서 우리는 importer을 이용하여 File을 읽어들일 떄 매개변수로 매크로 변수 넘겨줘야 함
    - const aiImporter::ReadFile(const string& filePath, unsigned int flag)
    - → scene = importer::ReadFile(String::ToString(filePath), aiProcess_ConvertToLeftHanded | aiProcess_Trialgulate | aiProcess_GenUVCoords | aiProcess_GenNormals | aiProcess_CalcTangentSpace
        - aiProcess_GenUVCoords : 삼각형에 맞는 UV 좌표 반환
        - aiProcess_GenNormals : Normal도 삼각형에 맞는 UV 좌표 반환
        - aiProcess_Trialgulate : 삼각형 기준 데이터로 반환. 그 외에 복셀 등 여러가지 케이스 존재.
    - 횡우선→열우선 변경은 전치행렬 사용
        - (D3DXMATRIXTranspose(D3DMATRIX & target, D3DMATRIX &from);