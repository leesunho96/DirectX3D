#pragma once
#define VIRTUALV 1
//#define CRTPV 2

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

	void* data;
	UINT count;
	UINT stride;
	UINT slot;

	bool bCpuWrite;
	bool bGpuWrite;
};

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

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


///////////////////////////////////////////////////////////////////////////////

#ifdef VIRTUALV


typedef function<void()> FunctionType;

class CsResource 
{
public:
	CsResource();
	virtual ~CsResource();


	virtual void CreateInput() = 0;
	virtual void CreateSRV() = 0;

	virtual void CreateOutput() = 0;
	virtual void CreateUAV() = 0;

	virtual void CreateResult() = 0;

	void CreateBuffer();


public:
	ID3D11ShaderResourceView* SRV() { return srv; }
	ID3D11UnorderedAccessView* UAV() { return uav; }
protected:


	// ������ ����
	ID3D11Resource* input = nullptr;
	// Input ���ۿ� ���� SRV
	ID3D11ShaderResourceView* srv;


	// ���� ����� ����� ����
	ID3D11Resource* output = nullptr;

	// ���� ��� ���ۿ� ���� SRV
	// ���α׷��Ӱ� �����ϴ� ��Ŀ� ���� ���۰� ����� -> UnorderedAccessView.
	// �� ���� ������, uav �� �ݵ�� default���߸� ��.
	// default : cpu���� ���� �Ұ���.
	ID3D11UnorderedAccessView* uav = nullptr;

	// output�� ���� �Ͽ� ������ ����
	ID3D11Resource* result = nullptr;

	vector<FunctionType> functionList;


};


////////////////////////////////////////////////////////////////////////////


class RawBuffer : public CsResource
{
public:
	RawBuffer(void* input, UINT inputByte, UINT outputByte);
	virtual ~RawBuffer();


private:
	virtual void CreateInput() override;
	virtual void CreateSRV() override;

	virtual void CreateOutput() override;
	virtual void CreateUAV() override;

	virtual void CreateResult() override;

public:
	void CopyToInput(void* data);
	void CopyFromOutput(void* data);


private:

	// RawBuffer�� Shader�� �ѱ� ������. 
	void* inputData;


	UINT inputByte;
	UINT outputByte;



};




#endif