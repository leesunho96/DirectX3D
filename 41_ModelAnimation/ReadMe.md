# Chap40 ModelMaterial

aiTexture* aiScene::GetEmbededTexture(wstring Path);

- 경로상에 존재하는 fbx 파일 안에 내장 텍스쳐 파일이 존재하면 해당 텍스쳐 파일 반환. 없는 경우 nullptr 반환.

fbx파일은 해당 파일 내부에 여러가지 파일(texture, sound …) 저장 가능. 해당 fbx파일 내부의 파일 반환하는 함수.

aiTexture→mHeight == 0

- texture의 height가 없다 → 데이터가 byte 순서대로 입력되어 있다.
- 해당 케이스의 경우 texture 크기만큼 그대로 파일로 저장.

aiTexture→mHeight > 1

- texture의 height 가 존재. 사실상 이론의 영역. 해당 케이스 거의 존재하지 않음.

[https://www.youtube.com/watch?v=aXwVkW6J_VY&list=PLbo7gRk05HhjqJzGmj0rJsOUkkMhkIaJE&index=13](https://www.youtube.com/watch?v=aXwVkW6J_VY&list=PLbo7gRk05HhjqJzGmj0rJsOUkkMhkIaJE&index=13)