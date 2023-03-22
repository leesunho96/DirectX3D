# Chap31_CBUFFER

FrameWork::Buffers.h

- D3D11_Buffer_Desc
    - Desc.Usage를 이용하여 해당 버퍼에 대한 접근 권한 설정.
    - D3D11_USAGE_IMMUTABLE :  외부에서 접근 불가능(CPU, GPU 모두 수정 불가능하며, 읽기만 가능.) : 해당 버퍼는 Rendering 용으로만 사용가능. 가장 속도가 빠르다.
    - D3D11_USAGE_DYNAMIC : CPU에서는 수정 가능하지만, GPU에서는 수정 불가능 : CPU에서 정점 제어 가능. MAP 이용하여 GPU로 값 복사. 위 케이스의 경우 D3D11_Buffer_Desc: CPUAccessFlags = true로 해야 함. 그렇지 않으면 immutable 처리.
    - D3D11_USAGE_DEFAULT : CPU는 읽기 전용, GPU는 수정 가능. CPU에서 처리 필요시 UpdateSubResource 함수 호출하여 처리 가능하지만, 예외적인 기능. UpdateSubResource는 GPU에 값을 복사하기 위해 Lock이용한 처리 → 해당 Lock에 대한 시점 처리 불가능, 해당 함수는 느리며, 주소 제어 불가능 하기 때문에 Map사용.
    - D3D11_USAGE_STGING : CPU, GPU 모두 값 수정 가능 → 가장 느리다. D3D11_Buffer_Desc::CPUAccessFlags를 D3D11_CPU_ACCESS_READ(GPU에서 CPU로 데이터 전송 가능) | D3D11_CPU_ACCESS_WRITE(CPU에서 GPU로 데이터 전송 가능.) 처리.
    
- DeviceContext→Map( ID3D11Resource *pResource, UINT Subresource,  D3D11_MAP MapType, UINT MapFlags,*  D3D11_MAPPED_SUBRESOURCE *pMappedResource)
    - pResource의 주소가 pMappedResource로 복사됨. 그 Mapping 형태는 D3D11_MAP 으로 정의 → GPU의 buffer가 복사된 값의 시작주소가 pMappedSubResource에 반환됨.
    - 반환된 값 pMappedResource를 이용하여 memcpy시 원하는 값 읽어 들이기 가능.
    - e.g. memcpy(subResource.pData, data, dataSize); // data를 subResource.pData로 dataSize만큼 복사 → 현재 원하는 data를 subResource.pData로 복사함. GPU에 값 복사해넣음.
    - 해당 경우에는 MapType → D3D11_MAP_WRITE_DISCARD로 사용.
    - 반대로 memcpy(data, subResource.pData, dataSize)시 GPU의 값을 CPU로 읽어들임.
    - 해당 경우에는 D3D11_MAP_WRITE 사용.
    - 이 함수는 GPU에 해당 영역이 묶여 버림. 따라서 사용 후, 바로 Unmap 해주지 않으면 프로그램 터짐.