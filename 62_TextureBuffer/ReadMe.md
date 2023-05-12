# Chap 61 ThreadGroup

[SV_GroupID - Win32 apps](https://learn.microsoft.com/ko-kr/windows/win32/direct3dhlsl/sv-groupid)

- 해당 링크에 ThreadGroup에 대한 설명.

- 하나의 Thread Group
    - in shader  [threadnum(x, y, z)]
    - in DX : Shader→Dispatch(x, y, z);
    - 하나의 thread group의 thread 수 : x * y * z;
        - 한 threadGroup의 최대 개수 : 1024개.
        - 1024개를 넘어서면 thread group 분할.
- e.g.
    - ThreadGroup : 2.
    - ThreadGroup : (3, 4, 5)
    - total thread num = 2 * 3 * 4 * 5개.

- Shader에 Data Input
    - ByteAddressBuffer에 저장.
    - BYteAddressBuffer.Load(address)를 이용하여 사용.
        - address : 원하는 값의 주소.
            - 연산을 통해 찾을 것.
    - e.g
        
        ```cpp
        struct Input
        	{
        		float Value = 0.0f;
        	};
        
        	Input* input = new Input[count];
        
        	for (UINT i = 0; i < count; i++)
        	{
        		input[i].Value = Math::Random(0.0f, 10000.0f);
        	}
        
         // 입력할 버퍼 생성, 해당 버퍼와 위의 원하는 입력값 bind
        	RawBuffer* rawBuffer = new RawBuffer(input, sizeof(Input) * count, sizeof(Output) * count);
        
        	// shader에 생성된 Input에 해당 값 push
        	shader->AsSRV("Input")->SetResource(rawBuffer->SRV());
          // Shader에 생성된 Output과 DX의 UAV(output에 대한 unorderedaccessview) bind.
        	shader->AsUAV("Output")->SetUnorderedAccessView(rawBuffer->UAV());
        
        	// DirectCompute 시작.
        	// 셰이더에 주어진 연산 시작.
        // 완료 후, shader의 output을 rawbuffer의 output에 push.
        // 모든 연산 완료 후 기존 코드로 제어권 전달.
        	shader->Dispatch(0, 0,GroupSize, 1, 1);
        
        	Output* output = new Output[count];
        
        // 반환된 output을 result에 복사해둠. 접근 권한 설정 문제
        // unorderedaccessview 는 cpu access 불가(D3D11_USAGE_DEFAULT) 설정해놓음
        // 속도 문제
        // 따라서 해당 버퍼를 복사해서 사용.
        	rawBuffer->CopyFromOutput(output);
        ```
        
        - output을 복사하는 result 버퍼 생성의 장점(일반적으로 staging buffer라 부름)
            - 가장 효율적인 방법은 아니지만 경우에 따라 장점 존재.
        1. Readback: If you need to retrieve the contents of a GPU buffer frequently or at specific points in your program, using a staging buffer allows you to copy the data to CPU-accessible memory and perform the necessary operations.
        2. Debugging and analysis: When debugging or analyzing the data processed by the GPU, using a staging buffer enables you to inspect the buffer contents on the CPU side.
        3. Asynchronous data transfers: In some cases, you may want to overlap GPU computations with CPU data processing. Using a staging buffer can facilitate asynchronous transfers and minimize synchronization points between the CPU and GPU.
    - in shader code
        
        ```glsl
        ByteAddressBuffer Input;
        RWByteAddressBuffer Output;
        
        struct Group
        {
            uint3 GroupID;
            uint3 GroupThreadID;
            uint3 DispatchThreadID;
            uint  GroupIndex;
            float RetVal;
        };
        
        struct ComputeInput
        {
            uint3 GroupID : SV_GroupID;
            uint3 GroupThreadID : SV_GroupThreadID;
            uint3 DispatchThreadID : SV_DispatchThreadID;
            uint GroupIndex : SV_GroupIndex;
        };
        
        [numthreads(10, 3, 8)]
        void CS(ComputeInput input)
        {
            Group group;
            group.GroupID = asuint(input.GroupID);
            group.GroupThreadID = asuint(input.GroupThreadID);
            group.DispatchThreadID = asuint(input.DispatchThreadID);
            group.GroupIndex = asuint(input.GroupIndex);    
        
            // 10, 8,, 3 from numthreads
            uint index = input.GroupID.x * 10 * 3 * 8 + input.GroupIndex;
            uint inAddress = index * FLOAT_SIZE;
            // outAddress = 변수의 개수 * 4. Output = uint3 x 3 + uint + float -> 11 * 4byte
            uint outAddress = index * UINT_SIZE * 11;
        			// 
            float inputVal = asfloat(Input.Load(inAddress));
            group.RetVal = inputVal;
        
            Output.Store3(outAddress + 0, asuint(group.GroupID)); // 0 ~ 11 byte
            Output.Store3(outAddress + UINT3_SIZE, asuint(group.GroupThreadID)); // 12 ~ 23 byte
            Output.Store3(outAddress + UINT3_SIZE * 2, asuint(group.DispatchThreadID)); // 24 ~ 35 byte
            Output.Store(outAddress + UINT3_SIZE * 3, asuint(group.GroupIndex)); //36 ~ 39 byte.
            Output.Store(outAddress + UINT3_SIZE * 3 + UINT_SIZE, asuint(group.RetVal)); // 40 ~ 43Byte
        
        }
        
        technique11 T0
        {
            pass P0
            {
                SetVertexShader(NULL);
                SetPixelShader(NULL);
        
                SetComputeShader(CompileShader(cs_5_0, CS()));
        
            }
        }
        ```
        
    - ByteAddressBuffer Input; → CPU에서 입력받은 버퍼.
    - casting(Input.Load(inputaddress))
        - → 입력받은 데이터의 원하는 index 주소를 이용하여 불러옴.
    - Output.Store(outAddress + UINT3_SIZE * 3 + UINT_SIZE, asuint(group.RetVal)); // 40 ~ 43Byte
        - 우리가 원하는 반환값은 float. 그러나 float 사용시 0반환. 반환형에 관계없이 asuint 캐스팅 필요.