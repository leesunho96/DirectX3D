# Chap 60 DirectCompute

- ThreadGroup : X, Y, Z
    - X * Y * Z 개의 Thread

```cpp
Shader->Dispatch(teq_Pass, ThreadGroup_X, ThreadGroup_Y, ThreadGroup_Z);
	// X, Y, Z 개수만큼의 Thread 생성.
```

- in C++
    - RawBuffer::input                          →
    - RawBuffer::Output                       →
    - Array                                           →

- in Shader
    - ByteAddressBuffer
    - RWByteAddressBuffer
    - uintN/floatN ……

- SV : SystemValue → Shader에서 제공하는 변수.
- [numthreads(groupX, groupY, groupZ)]
    - 해당 개수만큼의 thread를 생성.
- asuint(val) : Compute Shader에서의 안전선 보장된 형변환
    - (uint)val : 사용은 가능하지만, 안전성 보장하지 못함.
        - as~ 형식이 MS 권고사항.
- ComputeShader
    - 실제로 VS, PS를 사용하지는 않지만, 셰이더라서 선언은 반드시 해야함
        - VS(NULL), PS(NULL)의 형태로 작성.
    - SetComputeShader(CompileShader(cs_5_0, CS()));
        - cs_5_0 : 셰이더 버전.
        - CS() : 우리가 정의한 함수. 위의  numthreads 개수만큼 thread 생성해서 해당 연산 처리.
        -