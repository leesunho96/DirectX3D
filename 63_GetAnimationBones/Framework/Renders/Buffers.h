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


	// 생성할 버퍼
	ID3D11Resource* input = nullptr;
	// Input 버퍼에 대한 SRV
	ID3D11ShaderResourceView* srv;


	// 연산 결과가 저장될 버퍼
	ID3D11Resource* output = nullptr;

	// 연산 결과 버퍼에 대한 SRV
	// 프로그래머가 저장하는 방식에 따라 버퍼가 저장됨 -> UnorderedAccessView.
	// 비 정렬 데이터, uav 는 반드시 default여야만 함.
	// default : cpu에서 접근 불가능.
	ID3D11UnorderedAccessView* uav = nullptr;

	// output을 복사 하여 데이터 접근
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

	// RawBuffer로 Shader로 넘길 데이터. 
	void* inputData;


	UINT inputByte;
	UINT outputByte;



};




#endif


///////////////////////////////////////////////////////////////////////////////

class TextureBuffer : public CsResource
{
public:
	TextureBuffer(ID3D11Texture2D* src);
	~TextureBuffer();

private:
	void CreateSRV() override;

	void CreateOutput() override;
	void CreateUAV() override;
	void CreateInput() override;
	void CreateResult() override;

public:
	UINT Width() { return width; }
	UINT Height() { return height; }
	UINT ArraySize() { return arraySize; }

	ID3D11Texture2D* Output() { return (ID3D11Texture2D*)output; }
	ID3D11ShaderResourceView* OutputSRV() { return outputSRV; }

private:
	UINT width, height, arraySize;
	DXGI_FORMAT format;

	ID3D11ShaderResourceView* outputSRV;
};

//////////////////////////////////////////////////////////

class StructuredBuffer : public CsResource
{
public:
	StructuredBuffer(void* inputData, UINT inputStride, UINT inputCount, UINT outputStride = 0, UINT outputCount = 0);
	~StructuredBuffer();

private:
	void CreateInput() override;
	void CreateSRV() override;

	void CreateOutput() override;
	void CreateUAV() override;

	void CreateResult() override;

public:
	UINT InputByteWidth() { return inputStride * inputCount; }
	UINT OutputByteWidth() { return outputStride * outputCount; }

	void CopyToInput(void* data);
	void CopyFromOutput(void* data);

private:
	void* inputData;

	UINT inputStride;
	UINT inputCount;

	UINT outputStride;
	UINT outputCount;
};