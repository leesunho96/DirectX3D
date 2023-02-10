# TextureSampler

DirectX 의  TextureSampler는 shader를 사용 할 때 텍스쳐의 샘플을 정의하는 객체이다. 텍스쳐의 비율처리를 위해 사용되는 SamplerState_Filter, 텍스쳐의 영역 외부(여백)를 처리하는 방식을 정의하는 SamplerState_Address, 그 외의 텍스쳐 샘플시 제어를 위해 사용되는 속성들을 정의한다.

이러한 TextureSampler에 의해 사용되는 속성들은 텍스쳐 데이터를 찾고, 각 픽셀의 최종 색상을 얻기 위한 필수적인 연산이다. Rendering Pipeline의 굉장히 중요한 컴포넌트중 하나이며, 씬 내의 texture를 사용한 물체의 최종 모습을 결정하는데 사용된다.

1. SamplerState_FILTER.
    
    일반적으로 비율처리를 위해 사용된다, 확대, 축소, MipMap시 중간값을 처리한다.
    
    1-1. Min(Minification) : 일반적으로 텍스쳐의 크기를 줄이는데 사용된다.
    
    1-2. Mag(Magnification) : Mag는 3d 표면에 기존의 텍스쳐보다 확대하여 큰 크기로 그릴 때 사용하는 방식이다. Mag는 일반적으로 aliasing. blurness를 일으키며, 이러한 현상을 줄이기 위해 filter기술이 필요하다.
    
    - Point Sampling : TextureSampling시 가장 단순한 방식이다. Point는 텍스쳐 내부의 한 픽셀의 색상이 적용될 물체의 표면에 그대로 적용되는 방식이다. 단순하게 확대 등 효과가 필요 할 때, 확장되는 영역에 근처 픽셀의 값을 그대로 복사하는 방식이다.  해당 방식은 비용이 적게 들어가지만, ‘alised ‘ 현상이나, ‘blocky’ 현상을 일으켜 격자모양, 모자이크 형태 등이 보일 확률이 높으며, 멀리서 혹은 하이 앵글에서 바라볼 경우 특히 해당 현상이 자주 일어난다.
    - Anisotropic Sampling : 이미지의 확대시 확대되는 픽셀들을 주변 픽셀의 평균값을 이용하여 결정한다. 비등방성 필터링(MSAA(Multi Sample Anit Aliasing), X2, X4…)에서 X2, X4.. 는 평균값을 내기 위해 사용되는 픽셀의 수이다. X2의 경우 주변 1개(한 픽셀당 총 8개), x4의 경우 주변 24개의 픽셀의 평균값을 이용한다.
    
    1-3. Mip(MipMap) :  mip은 기존 텍스쳐보다 작은 크기의 텍스쳐를 생성하여 카메라가 바라보는    거리에 따라 사용하는 텍스쳐를 다르게 하는 방식이다. 일반적으로 DirectX는 속도 향상을 위해 메모리를 희생하여 하나의 이미지에 대략 8장의 이미지를 저장한다. 해당 이미지는 2의 배수의 픽셀로 저장되며, 카메라의 거리에 따라 사용되는 이미지가 조정된다.
    
    예를 들어 512 x 512의 텍스쳐를 2 x 2, 4 x 4, 8 x 8, …… 256 x 256의 이미지를 저장하는 방식을 사용하여  주로 2^n의 텍스쳐 크기를 주로 이용한다. 
    
    MipMap은 anit-alising시 사용되는 픽셀의 수를 줄임으로서 성능에 향상이 있다.
    
    해당 기술을 활용한 기술의 예로 상용엔진의 LOD(Level Of Detail)이 있다. 그러나 일반적으로 MipMap은 잘 사용되는 기술은 아니며, 주로 Point를 사용한다.
    
    MIN_LINEAR_MAG_POINT_MIP_LINEAR → Min : 선형보간, mag : point, Mip : 선형보간 의 형태로 samplerstate 정의. 이 외에도 수많은 형태로 정의되어 있으며 ‘d3d11.h’에  각 min/mag/mip의 형태에 따라 선언되어 있다.
    
2. SamplerState_Address
    
    일반적으로 UV 좌표가 1 초과일 때 해당 초과된 지점의 여백을 처리하는 방식이다.
    
    대략 5가지의 형태로 처리하는 경우가 많다.
    
    - Sampler_Address_Warp : 반복
    - Sampler_Address_Mirror : 거울. 기존 텍스쳐를 뒤집어서 반복한다.
    - Sampler_Address_Clamp : 각 지점의 마지막 픽셀을 연장한다.
    - Sampler_Address_Border : Border Color로 색칠한다.
    - Sampler_Address_MirrorOnce : 한번만 거울로 뒤집은 형태로 출력하고, 나머지는 Wrap의 형태로 출력한다.