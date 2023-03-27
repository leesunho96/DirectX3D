# Chap30 Readme

1. SkySphere
    
    in  Direct X
    
- 카메라 주위에 Rendering 하는 방식으로 구현
- Camera의 위치를 Sphere의 중심으로 구현.→ 구가 카메라를 따라다님.
- Shader→RSState에서 FrontCounterClockWise = true;(렌더링 방향 변경)
- 
    
    ```glsl
     DepthStencilState DepthStencilState_FALSE    
        {    
    	    DepthEnable = false;    
        }    
        …
        technique11    
        {    
        pass0    
        {    
    	    …    
    	    SetDepthStencilState(DepthStencilstaet, 0)    
    	    …    
        }    
        }
    
    ```
    
- DepthStencil 제거한 상태로 SkySphere Render → 일반 Actor Render → DepthStencil 제거한 상태로 UI Render
- → DepthStencil 제거한 상태로 Render시 깊이 판정 하지 않고, Render된 순서대로 Rendering. → UI가 묻히는 상황 방지.
- 
    
    [https://youtu.be/cJisdnWKmuI](https://youtu.be/cJisdnWKmuI)
    
    ---
    
    ---
    
- Texture
1. Texture1D
    - x좌표만 존재.
2. Texture2D
    - X, Y 좌표 존재.
3. Texture3D
    - X, Y, Z좌표만 존재
4. Teture2DARRAY
    - Texture2D의 배열.
    - Texture3D와는 Mip-Map의 차이만 존재.
5. TextureCube
    - 큐브를 펼친 형태로 texture 저장
    - 
    
    ![Untitled](Chap30%20Readme%201e12d33c9a694302b0fbfe80ac87c20c/Untitled.png)
    
    - front : [z](https://www.notion.so/z-12499abe76f9464e923c668f720c122f)
    - back : -z
    - left : -x
    - right : [x](https://www.notion.so/x-403f7a47e91e414eae88baf2cdcabb42)
    - bottom : -y
    - top ; +y
    
    이 큐브맵 렌더링시 원점으로부터 해당 pixel의 normal vector와의 교점을 sampling.
    
    [https://youtu.be/IpPx3ThmxuI](https://youtu.be/IpPx3ThmxuI)
    
    [https://youtu.be/NxCEJ4KhF8c](https://youtu.be/NxCEJ4KhF8c)