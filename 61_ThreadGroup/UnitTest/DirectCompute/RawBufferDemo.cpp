#include "stdafx.h"
#include "RawBufferDemo.h"

void RawBufferDemo::Initialize()
{
	Shader* shader = new Shader(L"60_RawBuffer.fx");
	constexpr int ThreadGroupX = 10;
	constexpr int ThreadGroupY = 8;
	constexpr int ThreadGroupZ = 3;
	constexpr int GroupSize = 2;



	// Thread GroupÀÇ ÃÑ °¹¼ö. ÇÏ³ªÀÇ Thread Group¿¡ ÃÑ 240°³ÀÇ thread ¼öÇà.  X, Y, ZÀÇ °³¼öÀÇ °ö
	UINT count = ThreadGroupX * ThreadGroupY * ThreadGroupZ * GroupSize;

	struct Output
	{
		UINT GroupID[3];
		UINT GroupThreadID[3];
		UINT DispatchThreadID[3];
		UINT GroupIndex;
		float RetVal;
	};


	struct Input
	{
		float Value = 0.0f;
	};

	Input* input = new Input[count];

	for (UINT i = 0; i < count; i++)
	{
		input[i].Value = Math::Random(0.0f, 10000.0f);
	}


	//RawBuffer* rawBuffer = new RawBuffer(nullptr, 0, sizeof(Output) * count );
	RawBuffer* rawBuffer = new RawBuffer(input, sizeof(Input) * count, sizeof(Output) * count);

	shader->AsSRV("Input")->SetResource(rawBuffer->SRV());
	shader->AsUAV("Output")->SetUnorderedAccessView(rawBuffer->UAV());

	shader->Dispatch(0, 0,GroupSize, 1, 1);

	Output* output = new Output[count];
	rawBuffer->CopyFromOutput(output);

	FILE* file;
	fopen_s(&file, "../Raw.csv", "w");


	fprintf
	(
		file, 
		"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
		"ThreadCount",
		"GroupID_0", "GroupID_1", "GroupID_2",
		"GroupThreadID_0", "GroupThreadID_1","GroupThreadID_2",
		"GroupDispatchThreadID_0", "GroupDispatchThreadID_1","GroupDispatchThreadID_2",
		"GroupIndex"
	);


	for (UINT i = 0; i < count; i++)
	{
		Output temp = output[i];
		fprintf
		(
			file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f\n",
			i,
			temp.GroupID[0], temp.GroupID[1], temp.GroupID[2],
			temp.GroupThreadID[0], temp.GroupThreadID[1], temp.GroupThreadID[2],
			temp.DispatchThreadID[0], temp.DispatchThreadID[1], temp.DispatchThreadID[2],
			temp.GroupIndex, temp.RetVal
		);
	}
	fclose(file);
}

void RawBufferDemo::Update()
{

}

void RawBufferDemo::Render()
{

}
