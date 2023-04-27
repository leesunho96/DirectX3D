# Chap50 Instancing

- 여러번의 Draw Call(Draw Primitive Call. DP)을 호출하는 방식이 아닌, 한번의 Draw Call에 여러개의 Mesh를 Draw하여 한번에 처리하는 기법.
- Draw Call : IA→VS→RS→PS→OM을 겪음.
- 만약 1000개의 동일한 Mesh를 Render 시도시, 1000번의 DP를 호출시 1000번의 Rendering Pipeline을 겪음. → 매우 비효율적.
    - Vertex Buffer Setting
    - Index Buffer Setting
    - Primitive Topology Setting
    - …. 그외의 필요한 데이터 세팅
    - GPU로 값 Mapping
    - Rendering → OM → SwapChain
- 현대의 모든 그래픽 프로그래밍은 Instancing 기반 실행.
- VertexBuffer : 정점 정보.
- Instance Buffer(Vertex Buffer) : 각각 Mesh에 대한 World Info,
- 정점에 대한 Vertex Info와 Instance Buffer를 이용하여 다수의 매시를 한번에 렌더링.

- Without Instancing Frame : 123

- With Instancing Frame : 304

![D3D Game 2023-04-27 오후 6_00_37.png](ReadMe/D3D_Game_2023-04-27_%25EC%2598%25A4%25ED%259B%2584_6_00_37.png)

![D3D Game 2023-04-27 오후 6_42_15.png](ReadMe/D3D_Game_2023-04-27_%25EC%2598%25A4%25ED%259B%2584_6_42_15.png)

- 실제로는 더 큰 차이가 존재해야 하지만, 테스트 환경상 두배 정도의 차이 존재.