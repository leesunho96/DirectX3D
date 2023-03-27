#pragma once

/*
 * 일반적으로 ID3D11Buffer를 상속받아서 구현하여야 하지만, 편의를 위해
 * Refer class 형태로 구현.
 *
 *
 */
class VertexBuffer
{
public:
	VertexBuffer(void* data, UINT count, UINT stride, UINT slot = 0, bool bCpuWrite = false, bool bGpuWrite = false);
	~VertexBuffer();

	UINT Count() { return count; }
	UINT Stride() { return stride; }
	ID3D11Buffer* Buffer() { return buffer; }

	void Render();
private:
	ID3D11Buffer* buffer;

	// data는 정점 데이터 소유 예정. 정점의 형태가 어떠한 식으로 들어올지 모르기 때문에 void*
	void* data;
	UINT count;
	UINT stride; // 정점의 크기
	UINT slot;

	// cpu/gpu 어느쪽에서 다룰지 정의.
	bool bCpuWrite;
	bool bGpuWrite;
};


//////////////////////////////////////////////////////////////////////////
class IndexBuffer
{
public:
	IndexBuffer(void* data, UINT count);
	~IndexBuffer();

	UINT Count() { return count; }
	ID3D11Buffer* Buffer() { return buffer; }

	void Render();

private:
	ID3D11Buffer* buffer;

	void* data;
	UINT count;
};


//////////////////////////////////////////////////////////////////////////
class ConstantBuffer
{
public:
	ConstantBuffer(void* data, UINT dataSize);
	~ConstantBuffer();

	ID3D11Buffer* Buffer() { return buffer; }

	void Render();

private:
	ID3D11Buffer* buffer;

	void* data;
	UINT dataSize;
};