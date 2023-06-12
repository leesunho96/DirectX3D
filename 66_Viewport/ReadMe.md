# Chap 66 Viewport

- 3D공간 → 2D 공간
    - In Rendering Pipeline
        - IA→VS→RS→PS→OM
    - VS : 정점 처리. W, V, P 처리.
    - RS : ViewPort 변환.
    - 3D 위치 :  W(World) → V(View) → P(Projection)
        - W : 직육면체 공간.
        - V → P
            - 원근 좌표계(Perspective) : 절두체 공간
                - World 공간의 앞면 : 축소.
                - World 공간의 뒷면 : 확대
            - 직교 좌표계(Orthogrphy) : 직육면체 공간
                - 크기 변화 X
            - 시야에 따라 공간이 설정됨.
            - Projection 완료 후에는 깊이가 남아있는 2D공간으로 이해.
        
    - Viewport공간(Rasterizer) : 직육면체 공간.
        - 원근 좌표계
            - 절두체 공간의 앞면 : 확대
            - 절두체 공간의 뒷면 : 축소.
        - 직교 좌표계 : 크기 변화 X
        - 정점들의 위치가 NDC 공간으로 변환.(-1, 1)
        - RS 종료시 완전한 2D공간으로 변환, Rendering Pipeline에 의해 결정됨.
        - 절두체 공간을 어떠한 크기로, 어떠한 깊이로 보여 줄 지 결정하는 것이 Viewport
        - Viewport의 깊이는 0~1의 범위로 구성
            - 0은 사용하지 않는다. 내부에 / 연산이 존재.
        - FOV : Field Of View.
            - 시야각.
                - 커질수록 화면 축소
                - 작아질 수록 화면 확대.