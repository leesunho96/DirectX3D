#include "Framework.h"
#include "PerFrame.h"

PerFrame::PerFrame(Shader* shader)
	: shader(shader)
{
	buffer = new ConstantBuffer(&desc, sizeof(Desc));
	sBuffer = shader->AsConstantBuffer("CB_PerFrame");
}

PerFrame::~PerFrame()
{
	SafeDelete(buffer);
}

void PerFrame::Update()
{
	desc.Time = Time::Get()->Running();
}

void PerFrame::Render()
{

	// _11, 12, 13 : 카메라의 X방향
	// _21, 22, 23 : 카메라의 Y방향
	// _31, 32, 33 : 카메라의 Z방향.
	// ViewInverse의 _41, _42, _43은 Camera의 원점 기준 위치
	desc.View = Context::Get()->View();
	D3DXMatrixInverse(&desc.ViewInverse, NULL, &desc.View);

	desc.Projection = Context::Get()->Projection();
	desc.VP = desc.View * desc.Projection;

	buffer->Render();
	sBuffer->SetConstantBuffer(buffer->Buffer());
}
