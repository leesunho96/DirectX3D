# Chap46 Animation Transform

- Transform은 cBuffer로 생성하여 셰이더로 전달하기에는 너무 큰 데이터셋.
    - 따라서 Texture화 하여 셰이더로 전달
    - D3D11_TEXTURE2D_DESC desc
        - desc.Width = MAX_MODEL_TRANSFORM * 4;
            - 하나의 픽셀의 최대 크기는 R32G32B32A32 → 16Byte.
            - 하나의 Transform : 4x4 Matrix → 64Byte
        - desc.Height = MAX_MODEL_KEYFRAMES;
        - desc.ArraySize = model→ClipCount();
        - desc.Format : DXGI_FORMAT_R32G32B32A32_FLOAT;
        - desc.Usage = D3D11_USAGE_IMMUTABLE
        - desc.BindFlag : D3D11_BIND_SHADER_RESOURCE
        - desc.MipLevel : 1
        - desc.SampleDesc.Count = 1
    - D3D11_SUBRESOURCE_DATA* subResource = newD3D11_SUBRESOURCE_DATA[model→ClipCount()]
        - subResource[c].pSysMem =
        - subResource[c].SysMemPitch = MAX_MODEL_TRANSFORMS * sizeof(Matrix)
            - 한장의 데이터는 Matrix * Transform의 수.
        - subResource[c].SysMemSlicePitch : desc.Width * desc.Height * 16
- VirtualAlloc
    - 가상 메모리 영역에 데이터를 생성, 할당.
    - VirtualAlloc(address, size, allocationtype, protection)의 형태.
        - address : 해당 메모리 블록이 저장 될 주소.
        - size : 메모리 블록이 할당될 크기.
        - allocationType : MEM_RESERVE(생성)/MEM_COMMIT(메모리 확정.)
    
    ```cpp
    // in winnt.h
    #define MEM_COMMIT                      0x00001000
    #define MEM_RESERVE                     0x00002000
    #define MEM_REPLACE_PLACEHOLDER         0x00004000
    #define MEM_RESERVE_PLACEHOLDER         0x00040000
    #define MEM_RESET                       0x00080000
    #define MEM_TOP_DOWN                    0x00100000
    #define MEM_WRITE_WATCH                 0x00200000
    #define MEM_PHYSICAL                    0x00400000
    #define MEM_ROTATE                      0x00800000
    #define MEM_DIFFERENT_IMAGE_BASE_OK     0x00800000
    #define MEM_RESET_UNDO                  0x01000000
    #define MEM_LARGE_PAGES                 0x20000000
    #define MEM_4MB_PAGES                   0x80000000
    #define MEM_64K_PAGES                   (MEM_LARGE_PAGES | MEM_PHYSICAL)
    
    ```
    
    - protection :  PAGE_READWIRTE
    
    ```cpp
    #define PAGE_NOACCESS           0x01
    #define PAGE_READONLY           0x02
    #define PAGE_READWRITE          0x04
    #define PAGE_WRITECOPY          0x08
    #define PAGE_EXECUTE            0x10
    #define PAGE_EXECUTE_READ       0x20
    #define PAGE_EXECUTE_READWRITE  0x40
    #define PAGE_EXECUTE_WRITECOPY  0x80
    ```
    
    - VirtualFree() : 해제.