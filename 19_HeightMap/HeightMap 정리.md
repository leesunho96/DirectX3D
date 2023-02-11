# HeightMap

- Texture(png 등)를 이용하여 Terrain 생성 가능.
- Texture의 각 픽셀은 r.g.b.a 4개의 수로 이루어져 있으며, 각 값은 0~1로 정규화 되어 있는 상태로 구성된다. 일반적인 경우 r 값 하나로만 구성되어 있는 경우와 흑백(0,0,0)~(1,1,1)로 구성되어 있는 경우가 존재하며, 해당 값을 평면의 y좌표로 사용하여 높낮이를 설정한다.
- DirectX는 xz평면과 상단의 y축으로 이루어져 있기 때문에 y값이 높이, z값이 깊이를 이루게 된다. Texture의 UV좌표는 좌측상단이 0의 값을 갖게 되며, 하단으로 내려 올수록 값이 커지게 된다. 반대로 우리가 보게 될 화면은 좌측 하단의 값이 0이며, 상단으로 올라갈 수록 값이 커지기 때문에 실제 HeightMap의 y값을 반대로 뒤집어 주어야 우리가 보는 대로의 Terrain을 갖게 된다.

TextureSplatting (Texture Layering)

- 일반적으로 Terrain을 생성 할 때 하나의 Texture만을 사용하는 경우는 흔치 않으며, 여려개의 Texture를 조합하여 사용하는 경우가 많다. 이러한 경우에 여러개의 Texture를 어떠한 비율로 섞어서 사용 하는지 정의하는 방법이다.
- TextureSplatting은 크게 두가지 방식으로 사용 될 수 있는데, 첫번째로 HeightMap의 a값 만을 높이값으로 사용하고, r.g.b 세개의 값을 다른 3장의 texture의 가중치로 사용하는 방식이 있다. 두번쨰로는 각각의 Texture의 a값을 가중치로 사용하는 방식이 존재 할 수 있다.
- heightmap 하나의 채널을 이용하는 방식은 3개의 texture만 사용 가능하다는 단점이 있지만, 일반적으로 여러개의 texture를 사용하는 방식보다는 약간 더 효율적이다.
- 반대로 각각의 texture의 a값을 가중치로 사용하는 경우 많은 개수의 texture를 사용 할 수 있지만, 하나의 채널에 a값을 모아두는 방식보다 약간이나마 비용이 더 들수도 있다.
- 물론 위 두가지의 비교는 일반적인 상황에서의 결과이며, 프로그래밍 작성 방식, 하드웨어 환경 등에 의해 달라 질 수 있다.

HeightMap 영상

[https://www.youtube.com/watch?v=sjFVMrBn5is&list=PLbo7gRk05HhjqJzGmj0rJsOUkkMhkIaJE&index=3](https://www.youtube.com/watch?v=sjFVMrBn5is&list=PLbo7gRk05HhjqJzGmj0rJsOUkkMhkIaJE&index=3)