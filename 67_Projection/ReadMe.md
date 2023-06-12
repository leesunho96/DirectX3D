# Chap 67 Projection

- 3D → 2D 공간.
    - W → V → P 한단계 씩 진행하는 방식이 아닌, W * V * P를 매개변수로 할당하여 한번에 진행하는것이 속도 빠름.
        - Transform * W * V * P (X)
        - Transform * (W * V * P) (O)
    - 화면 해상도 (1280, 720) 가정.
        - NDC 공간 : X(-1 ~ 1), Y(-1 ~ 1)
        - DX2D 공간 : X(-640 ~ 640), Y(-360 ~ 360)
        
        ```cpp
        void Viewport::Project
        (
        	Vector3* pOutput, 
        	Vector3& source, 
        	const Matrix& W, const Matrix& V, const Matrix& P
        )
        {
        	Vector3 position = source;
        	Matrix wvp = W * V * P;
        
        	// Position을 WVP 적용한 값으로 변환하여 pOutput에 저장.
        	// 반환된 값은 NDC 공간 값(화면 전체의 NDC 공간 값).
        	D3DXVec3TransformCoord(pOutput, &position, &wvp);
        
        	pOutput->x = ((pOutput->x + 1.0f) * 0.5f) * width + x;
        	pOutput->y = ((- pOutput->y + 1.0f) * 0.5f) * height + y;
        	pOutput->z = (pOutput->Z * (maxDepth - minDepth)) + minDepth;
        }
        ```