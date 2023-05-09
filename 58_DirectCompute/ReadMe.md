# Chap 58 Direct Compute

- IA→VS→RS→PS→OM
    - 이중 VS/PS는 화면 조작
        - VS : 정점 하나당 Thread 하나.
        - PS : 픽셀 하나당 Thread 하나.
- Rendering Pipeline과는 별개로 호출 가능한 쉐이더 하나 존재
    - Compute Shader(CS)
    - float만 처리 가능.