# Chap39 ModelMaterial

Material 

- Color
- Texture
- 위 두가지 데이터는 aiScene::Material에 저장되어 있음.
- ModelMesh::Render() 호출시 미리 저장한 Material Array에서 이름을 기반으로 탐색하여 저장.

aiMaterial

- aiMaterial→Get(IDX, RetVal);
    - IDX : Macro로 사전 정의
        - AI_MATKEY_COLOR_DIFFUSE/EMISSIVE/SPECULAR/AMBIENT/TRANSPARENT/SHINESS ……
        - 필요에 따라 사전 정의된 매크로 이용하여 데이터 추출.
        - 빛은 불투명 : Specular.a에 빛의 세기(강도)를 주로 저장함.
    - RetVal : 반환값의 형태에 따라 달라짐. COLOR : aiColor3D, float : float 등.
- aiMaterial→GetTexture(aiTextureType, idx, retVal);
    - aiTextureType :
        - aiTextureType_DIFFUSE
        - aiTextureType_SPECULAR
        - aiTextureType_NORMALS ……
    - idx
    - retVal : MaterialFile의 경우 aiString 형태로 file Path 반환.
        - 이 file Path에 따라 추가 처리 필요.
            - 반환값이 비어있으면
                - 내장 Texture
                - 비 내장 Texture
                - 등에 따라 추가적 처리 필요.
            - 
            
            [https://www.youtube.com/watch?v=T7N9N9J1O4U&list=PLbo7gRk05HhjqJzGmj0rJsOUkkMhkIaJE&index=12](https://www.youtube.com/watch?v=T7N9N9J1O4U&list=PLbo7gRk05HhjqJzGmj0rJsOUkkMhkIaJE&index=12)