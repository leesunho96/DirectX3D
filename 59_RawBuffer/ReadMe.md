# Chap 59 Raw Buffer

- Raw Buffer
    - ‘Raw’ : 동일한 크기의 바이트가 형식에 관계없이 순서대로 쌓여있음을 의미
        - 사용자가 해당 데이터를 의도에 따라 관리
    - 동일한 크기의 byte가 순서대로 쌓여있는 버퍼를 의미.

- ID3D11 Resource
    - Texture, Buffer 등 자원 객체는 모두 해당 인터페이스 상속.
    - Raw Buffer/ Structured Buffer 모두 해당 인터페이스 상속.

- 
    
    ```cpp
    class RawBuffer
    {
    
    protected:
    	ID3D11Resource* input, output;// input : 입력 데이터, output : 연산 완료 후 데이터.
    
    	ID3D11ShaderResourceView * srv; // for input
    	ID3D11UnorderedAccessView * uav; // for output
    
    	ID3D11Resource* result // output은 CPU에서 접근 불가능 세팅 -> output을 복사하여 해당 값 접근.
    }
    ```
    
    - ID3D11UnorderedAccessView
        - 프로그래머의 자의적인 데이터 사용
            - 프로그램은 해당 데이터에 대한 형식을 모름.
            - 비 정렬 데이터.
            - uav는 반드시 형식 Default.
                - 그 외의 타입은 생성 실패.
            - 따라서 CPU에서 접근 불가능
                - → 별도의 Recource 생성하여 기존 값 복사 후, 이용.
        
        ```cpp
        
        /*
        	해당 클래스는 output 객체를 참조하는 ID3D11UnorderedAccessView*가
        	CPU 접근 불가이기 때문에 생성하는 Output을 복사하여 저장하는 ID3D11Recource* result
        	생성.
        */
        RawBuffer::CreateResult()
        {
        ...
        
        D3D11_BUFFER_DESC desc;
        	
        ...
        desc.BindFlags = D3D11_USAGE_DEFAULT; 
        	// == 0, CPU 접근 불가, GPU 만 접근 가능.
        	// 이 외의 타입으로 생성시 연결 실패.
        
        	
        }
        ```
        
- C~DESC
    - 구조체의 초기값을 미리 자동 세팅 시켜놓은 구조체.
        - 모든 DESC 파일은 해당 파일 소유.

- 모든 DESC를 소유한 객체
    - .GetDesc() → 해당 객체가 소유한 DESC 반환.