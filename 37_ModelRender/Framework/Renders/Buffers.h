#pragma once

/*
 * �Ϲ������� ID3D11Buffer�� ��ӹ޾Ƽ� �����Ͽ��� ������, ���Ǹ� ����
 * Refer class ���·� ����.
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

	// data�� ���� ������ ���� ����. ������ ���°� ��� ������ ������ �𸣱� ������ void*
	void* data;
	UINT count;
	UINT stride; // ������ ũ��
	UINT slot;

	// cpu/gpu ����ʿ��� �ٷ��� ����.
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