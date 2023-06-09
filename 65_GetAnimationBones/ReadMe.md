# Chap 63 GetBoneTransform

- StructuredBuffer
    - uint inputStride : 입력 구조체의 크기
    - uint inputCount : 입력 구조체의 개수.
    - uint outputStride : 출력 구조체의 크기.
    - uint outputCount : 출력 구조체의 크기.

- outputStride or outputCount == 0
    - inputStride/Count와 동일하게 설정.
- inputDesc.MiscFlag = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED
- inputDesc.Usage = D3D11_USAGE_DYNAMIC

- Thread Group
    - Dispatch메소드에 의해 개수 정해짐.
        - shader→dispatch(x, y, z); x*y*z개의 threadGroup 생성.
        - 1개의 ThreadGroup당 Thread개수 : shader의 [numthread(x1, y1, z1)]에 의해 결정.
            - 1개의 threadgroup당 x1*y1*z1개의 thread 생성
    - SV_GroupID : Dispatch된 Thread Group의 ID.
    - SV_GroupThreadID: Thread Group내에서의 ID. numthread 번호.
        - eg. numthread(10, 8, 1)에서의 번호.
            - (5, 3, 1)의 형태로 사용.
    - SV_GroupIndex : 해당 ThreadGroup에서 몇번째 Thread인지.
        - SV_GroupID = (2, 1, 0)
        - SV_GroupThreadID= (7, 5, 0)
        - numthread(10, 8, 3)
            
            ```glsl
            SV_GroupIndex = numthread.x(10) * numthread.y(8) * GroupThreadID.z + 
            								numthread.x(10) * GroupThreadID.y +
            								GroupThreadID.x;
            ```
            
    - SV_DispatchedThreadID =  ThreadGroup 전체에서 몇번째 GroupThreadID인지.
        - SV_DIspatchedThreadID = SV_GroupID * numthread(x, y, z) + SV_GroupThreadID;
        
        ![threadgroupids.png](ReadMe/threadgroupids.png)