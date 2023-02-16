# NormalVector

NormalVector : 한 점에서의 수직 벡터.

한 정점의 수직 벡터를 구하는 방법은 일반적으로 외적을 이용하여 산출한다.

벡터(v1-v0), 벡터(v2-v0)을 구하면 한 삼각형의 정점을 구할 수 있으며, 두개의 벡터를 외적하면 두 벡터와 수직인 제 3의 벡터가 산출된다. 그러나 하나의 정점은 여러 삼각형의 구성 요소가 되며, 한 삼각형에서만의  Normal Vector를 사용 하는것이 아닌, 모든 삼각형에서의 각각의 정점의 노멀 벡터를 더한 후, 해당 벡터의 평균을 구하면 해당 정점의 실질적인 NormalVector를 구할 수 있다.

평균을 구하는 방식은 실제로는 Cost가 매우 높을 수 있으며, 추가적인 메모리 또한 필요한 등 많은 요소가 필요하다. 따라서 실제로는 해당 정점의 크기로 나누어 정규화 하는 방식으로 한 정점의 노멀 벡터를 만들어 낸다.

노멀 벡터는 각 정점에서의 노멀 벡터를 구한 후, 그 사이의 점들을 Rasterizer State에서 보간하여 구하는 방식이 있고, 노멀 맵이라는 별도의 파일을 이용하여 각 픽셀들의 노멀 벡터를 구하는 방식 또한 존재한다.


# ID3D11DeviceContext

ID3D11DeviceContext는 GPU에 Rendering Command를 제공하는 객체이며, GPU의 상태(Vertex, IndexBuffer, Shader, PipelineResources…)의 관리 책임을 갖고 있다.

CPU와 GPU가 Device-Context State를 유지하도록 해주는 객체이다.

DeviceContext는 목적에 맞는 개수의 생성이 필요하다.

예를 들어 GameEngine의 Rendering을 위한 DeviceContext와 Animation 계산을 위한 DirectCompute용 DeviceContext를 소유하는 방식이 있다.

DeviceContext의 수가 늘어남에 따른 장점

- 병렬화를 확대할 수 있으며, 잠재적인 프로그램의 성능 향상을 이룰 수 있다
- 각각의 DeviceContext에 자원을 할당 함으로써 자원 관리가 용이 해 질 수 있다.
- 다양한 입력-출력을 위한 ComputeShader 사용과 같은 작업에 유리 할 수 있다.

DeviceContext의 수가 줄어듬에 따른 장점.

- 작업을 단순화 할 수 있으며, 관리가 편리해진다. 또한 다수의 DeviceContext간 할당 문제를 고려하지 않아도 된다.
- 각각의 DeviceContext의 복제에 따른 메모리 낭비 문제를 줄일 수 있다.
- 최적화와 디버깅이 편리하다.


# Vector3 vs Vector4

- 일반적으로 벡터를 사용시 해당 벡터는 방향 혹은 위치를 나타내게 된다. 사람이 해당 좌표를 보게 되면 해당 좌표의 의미를 직관적으로 이해 가능하지만, 컴퓨터는 해당 이해가 불가능 하게 되며, 이를 구분해 줄 필요가 있다.

- Vector3(Shader에서는 float 3)는 단순한 방향 벡터를 의미한다.
    
    vector3는 3x3 matrix와 연산이 가능하다. 그러나 4x4 matrix에서 _ 41, _ 42. _ 43은 x, y, z 좌표를 의미한다. 따라서 vector3으로 연산시 해당 x,y,z 좌표가 사라지며 방향 좌표만 남게 된다.
    

- Vector4(Shader에서는 float 4)는 위치 혹은 방향 모두 의미 할 수 있으며, 이는 w에 따라 달라진다.
    
    vector3은 4x4 matrix와 연산이 가능하다. 그러므로 4x4 matrix의  _ 41, _ 42. _ 43 좌표와 연산이 가능하며, 유의미한 값을 내놓게 된다. 그러나 w가 0이 되면 해당 값은 남지 않게 된다.
    
    - 따라서 w값이 0이면 방향, 1이면 위치를 의미하게 된다.
    

 

Vector4의 W.

- 일반적으로 W는 Rendering Pipeline의 단계에 따라 다른 의미를 갖고 있다.
- Vertex Shader에서 W는 해당 값이 위치인지, 방향인지 구분하기 위한 목적을 갖고 있다.
- Pixel Shader에서는 W가 동차의 의미를 갖고 있다.

동차 : 동차는 화면의 해상도에 따른 연산 목적을 위해 해당 픽셀의 좌표의 비율을 정규화하여(0 ~ 1 혹은 -1 ~ 1) 표기. 동차는 Pixel Shader에서 처리.

Vector4의 W 는 해당 값이 사용되는 위치에 따라 달라진다. VertexShader의 경우 위치/방향 구분, PixelShader의 경우 동차의 의미로 사용된다.

[https://www.youtube.com/watch?v=FKqtNrkBjoc&list=PLbo7gRk05HhjqJzGmj0rJsOUkkMhkIaJE&index=4](https://www.youtube.com/watch?v=FKqtNrkBjoc&list=PLbo7gRk05HhjqJzGmj0rJsOUkkMhkIaJE&index=4)

[https://www.youtube.com/watch?v=YpfbPSQ8lAw&list=PLbo7gRk05HhjqJzGmj0rJsOUkkMhkIaJE&index=5](https://www.youtube.com/watch?v=YpfbPSQ8lAw&list=PLbo7gRk05HhjqJzGmj0rJsOUkkMhkIaJE&index=5)