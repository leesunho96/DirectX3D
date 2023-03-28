#include "Framework.h"
#include "Buffers.h";

VertexBuffer::VertexBuffer(void* data, UINT count, UINT stride, UINT slot, bool bCpuWrite, bool bGpuWrite)
	: data(data), count(count), stride(stride), slot(slot), bCpuWrite(bCpuWrite), bGpuWrite(bGpuWrite)
{
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = stride * count;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;


		
		if(bCpuWrite == false && bGpuWrite == false)
		{
			// D3D11_USAGE_IMMUTABLE : 외부에서 접근 불가능(CPU, GPU 모두 읽기만 가능. 수정 불가능). 해당 버퍼는 Render용으로만 사용.
			// USage에 따라 속도가 다름 Immutable가 속도가 가장 빠름.
			// Immutable : 상수 취급.
			desc.Usage = D3D11_USAGE_IMMUTABLE;
		}
		else if(bCpuWrite == true && bGpuWrite == false)
		{
			// D3D11_USAGE_DYNAMIC
			// immutable보다 느리지만, cpu에서 쓰기 가능, GPU는 읽기만 가능.
			// GPU 읽기 가능 : Render용으로만 접근 가능.
			// CPU에서 정점 제어 가능. Map 이용하여 GPU로 데이터 복사.
			desc.Usage = D3D11_USAGE_DYNAMIC;
			// 해당 플래그 없으면 immutable와 동일하게 처리.
			desc.CPUAccessFlags = true;
		}
		else if (bCpuWrite == false && bGpuWrite == true)
		{
			// D3D11_USAGE_DEFALUT : CPU는 읽기만 가능, GPU는 읽기 쓰기 모두 가능
			// CPU 쓰기 처리 필요시 예외적으로 -> UpdateSubResource 로만 처리 가능.
			// UpdateSubResource : GPU에 값 복사하기 위하여 Lock 걸고, 복사 후, Lock 해제. -> 시점 제어 불가능
			// 상대적으로 UpdateSubResource 느림, 주소 제어 불가능.- > Map 사용.
			desc.Usage = D3D11_USAGE_DEFAULT;
		}
		else
		{
			// D3D11_USAGE_STAGING 
			// GPU, CPU 모두 read/write 가능 -> 가장 느림
			desc.Usage = D3D11_USAGE_STAGING;
			// D3D11_CPU_ACCESS_READ : Gpu에서 Cpu로 데이터 전송 가능. D3D11_CPU_ACCCESS_WRITE : CPU에서 GPU로 데이터 전송 가능
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

		}

		D3D11_SUBRESOURCE_DATA subResource = {0 };
		subResource.pSysMem = data;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &buffer));
	}
}

VertexBuffer::~VertexBuffer()
{
	SafeRelease(buffer);
}

void VertexBuffer::Render()
{
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(slot, 1, &buffer, &stride, &offset);
}

IndexBuffer::IndexBuffer(void* data, UINT count)
	: data(data), count(count)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	// Index -> UINT. 그 외의 type가 사용될 일이 없음. Index니까.
	desc.ByteWidth = sizeof(UINT) * count;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	// Index buffer는 변경될일이 없음 -> Immutable.
	desc.Usage = D3D11_USAGE_IMMUTABLE;


	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = data;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &buffer));
}

IndexBuffer::~IndexBuffer()
{
	SafeRelease(buffer);
}

void IndexBuffer::Render()
{
	D3D::GetDC()->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
}




/*
 * Constant BUffer
 *  Shader->AsMatrix("World")->SetMatrix(~);
 *	-> Shader->AsMatrix("World") ->ID3DX11EffectShaderResourceVariable
 *	-> shader에서 matrix World; 형태로 정의
 *	해당 전역 변수들은 CBuffer $Global의 영역으로 들어감.
 *	cBuffer $Global
 *	{
 *	world
 *	color
 *	......
 *	}
 *
 *	-> 전역 변수의 형태로 저장.
 *  -> World 수정 시도시 Color 도 수정. 속도 느려짐.
 *
 *	Shader에서는 CBuffer DX에서는 Constant Buffer.
 *	HLSL에서는 CBuffer는 Register 형태로 구역 번호 할당.
 *	EFFECT에서는 해당 구역번호는 없음.
 *
 *	HLSL에서는 CPU에서 복사된 값을 변경 하지 못함 : CBuffer라 불렀음.
 *	Effect에서는 변경 가능.
 */

///////////////////////////////////////////////////////////////////////////////

ConstantBuffer::ConstantBuffer(void* data, UINT dataSize) : data(data), dataSize(dataSize)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));


	desc.ByteWidth = dataSize;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC; // CPU 쓰기 가능, GPU 읽기 가능.
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

	Check(D3D::GetDevice()->CreateBuffer(&desc, NULL, &buffer));
}

ConstantBuffer::~ConstantBuffer()
{
	SafeRelease(buffer);
}

void ConstantBuffer::Render()
{
	D3D11_MAPPED_SUBRESOURCE subResource;

	// GPU에 해당 버퍼 구역이 묶여버림. -> 타 객체는 버퍼 접근 불가능. -> unmap 하지 않으면 rendering시도 접근 불가능
	// Map시 GPU의 buffer가 복사된 값의 시작 주소가 subResource에 복사되어 반환됨.
	// D3D11_MAP_WRITE -> GPU에 CPU의 값 복사. GPU에 값 입력
	// D3D11_MAP_READ  -> CPU에 GPU의 값 복사. GPU의 값을 읽어들임
	D3D::GetDC()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		// 현재는 값 GPU로 넘겨줌 -> subResource.pData에 data를 dataSize만큼 복사.
		memcpy(subResource.pData, data, dataSize);
		// 만약 GPU의 값을 읽어들이고 싶으면 memcpy(data, subResource.pData, dataSize);
	}
	D3D::GetDC()->Unmap(buffer, 0);
}