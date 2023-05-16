# Chap 62 TextureBuffer

- TextureBuffer→Texture2DArray
    - srvDesc.ViewDimention = D3D11_SRV_DIMENTION_TEXTURE2DARRAY
    - srvDesc.Texture2DArray.ArraySize = arraySize;

- Output
    - output.srv.bindFlag = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE

- TextureBuffer                            →
- ID3D11ShaderResourceView* outputSRV;
    - output.bindFlag에 D3D11_BIND_SHADER_RESOURCE 포함되어있기 때문.
    - TextureBuffer의 output의 경우 이 output이 타 shader의 입력으로 주어지는 등의 접근을 하기 때문에 위와 같이 선언.

- CSResource
- ID3D11Resource* input;
- ID3D11ShaderResourceView* srv;
- ID3D11Resource* output;
- ID3D11UnorderedAccessView* uav;
- ID3D11Resource* result;

- Render2D
    - OrthoOffCenterLH사용 → 직교투영, 중심점을 임의로 사용.
    - 좌표계
        - perspective(원근좌표계) : 원근법 적용. 멀리있는 액터는 작게, 가까이 있는 액터는 크게.
        - Orthography(직교좌표계) : 원근법 무시, 모든 액터를 액터 크기 그대로 렌더.
            - OrthoOffCenterLH : 중심점 임의로 설정(매개변수로 left, right, height, bottom,depth 필요)
            - OrthoLH : 크기를 이용하여 중심점 설정.
            
- 우리가 Render한 Compute.png : 600 X 400 pixel.
    - threadgroup 하나의 최대 thread수 : 1024 == 32 X 32.
    - 한 threadGroup당 32 X 32픽셀의 영역만큼 처리 가능.
    - [numthread(32, 32, 1)] → 하나의 ThreadGroup.
        - ThreadGroup의 Width = ceilf(width / 32) → int(600 / 32) + 1
        - ThreadGroup의 Height = ceilf(Height / 32) → int(450 / 32) + 1
        - ThreaGroup의 Depth = arraySIze
            - → total Thread 수 : ThreadGroupNum(Width X Height X ArraySize) X ThreadGroup’s thread num(32 X 32 X 1).
            - 일부 일하지 않는 thread도 생성. 부족해서 처리하지 않는것보다는 낫다.
            
- 흑백화면 방법 :
    - 
    
    ```glsl
    Texture2DArray<float4> Input;
    RWTexture2DArray<float4> Output;
    
    [numthreads(32, 32, 1)]
    void CS(uint3 id : SV_DispatchThreadID)
    {
        // x, y, z : pixel 위치. z : mipmap Level.
        float4 color = Input.Load(int4(id, 0));
        //Output[id] = 1.0f - color;
    
    		// 흑백화.
        Output[id] = (color.r + color.g + color.b) / 3.0f;    
    }
    ```