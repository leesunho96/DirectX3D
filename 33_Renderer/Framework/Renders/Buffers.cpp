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
			// D3D11_USAGE_IMMUTABLE : �ܺο��� ���� �Ұ���(CPU, GPU ��� �б⸸ ����. ���� �Ұ���). �ش� ���۴� Render�����θ� ���.
			// USage�� ���� �ӵ��� �ٸ� Immutable�� �ӵ��� ���� ����.
			// Immutable : ��� ���.
			desc.Usage = D3D11_USAGE_IMMUTABLE;
		}
		else if(bCpuWrite == true && bGpuWrite == false)
		{
			// D3D11_USAGE_DYNAMIC
			// immutable���� ��������, cpu���� ���� ����, GPU�� �б⸸ ����.
			// GPU �б� ���� : Render�����θ� ���� ����.
			// CPU���� ���� ���� ����. Map �̿��Ͽ� GPU�� ������ ����.
			desc.Usage = D3D11_USAGE_DYNAMIC;
			// �ش� �÷��� ������ immutable�� �����ϰ� ó��.
			desc.CPUAccessFlags = true;
		}
		else if (bCpuWrite == false && bGpuWrite == true)
		{
			// D3D11_USAGE_DEFALUT : CPU�� �б⸸ ����, GPU�� �б� ���� ��� ����
			// CPU ���� ó�� �ʿ�� ���������� -> UpdateSubResource �θ� ó�� ����.
			// UpdateSubResource : GPU�� �� �����ϱ� ���Ͽ� Lock �ɰ�, ���� ��, Lock ����. -> ���� ���� �Ұ���
			// ��������� UpdateSubResource ����, �ּ� ���� �Ұ���.- > Map ���.
			desc.Usage = D3D11_USAGE_DEFAULT;
		}
		else
		{
			// D3D11_USAGE_STAGING 
			// GPU, CPU ��� read/write ���� -> ���� ����
			desc.Usage = D3D11_USAGE_STAGING;
			// D3D11_CPU_ACCESS_READ : Gpu���� Cpu�� ������ ���� ����. D3D11_CPU_ACCCESS_WRITE : CPU���� GPU�� ������ ���� ����
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

	// Index -> UINT. �� ���� type�� ���� ���� ����. Index�ϱ�.
	desc.ByteWidth = sizeof(UINT) * count;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	// Index buffer�� ��������� ���� -> Immutable.
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
 *	-> shader���� matrix World; ���·� ����
 *	�ش� ���� �������� CBuffer $Global�� �������� ��.
 *	cBuffer $Global
 *	{
 *	world
 *	color
 *	......
 *	}
 *
 *	-> ���� ������ ���·� ����.
 *  -> World ���� �õ��� Color �� ����. �ӵ� ������.
 *
 *	Shader������ CBuffer DX������ Constant Buffer.
 *	HLSL������ CBuffer�� Register ���·� ���� ��ȣ �Ҵ�.
 *	EFFECT������ �ش� ������ȣ�� ����.
 *
 *	HLSL������ CPU���� ����� ���� ���� ���� ���� : CBuffer�� �ҷ���.
 *	Effect������ ���� ����.
 */

///////////////////////////////////////////////////////////////////////////////

ConstantBuffer::ConstantBuffer(void* data, UINT dataSize) : data(data), dataSize(dataSize)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));


	desc.ByteWidth = dataSize;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC; // CPU ���� ����, GPU �б� ����.
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

	// GPU�� �ش� ���� ������ ��������. -> Ÿ ��ü�� ���� ���� �Ұ���. -> unmap ���� ������ rendering�õ� ���� �Ұ���
	// Map�� GPU�� buffer�� ����� ���� ���� �ּҰ� subResource�� ����Ǿ� ��ȯ��.
	// D3D11_MAP_WRITE -> GPU�� CPU�� �� ����. GPU�� �� �Է�
	// D3D11_MAP_READ  -> CPU�� GPU�� �� ����. GPU�� ���� �о����
	D3D::GetDC()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		// ����� �� GPU�� �Ѱ��� -> subResource.pData�� data�� dataSize��ŭ ����.
		memcpy(subResource.pData, data, dataSize);
		// ���� GPU�� ���� �о���̰� ������ memcpy(data, subResource.pData, dataSize);
	}
	D3D::GetDC()->Unmap(buffer, 0);
}