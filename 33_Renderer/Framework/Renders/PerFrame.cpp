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

	// _11, 12, 13 : ī�޶��� X����
	// _21, 22, 23 : ī�޶��� Y����
	// _31, 32, 33 : ī�޶��� Z����.
	// ViewInverse�� _41, _42, _43�� Camera�� ���� ���� ��ġ
	desc.View = Context::Get()->View();
	D3DXMatrixInverse(&desc.ViewInverse, NULL, &desc.View);

	desc.Projection = Context::Get()->Projection();
	desc.VP = desc.View * desc.Projection;

	buffer->Render();
	sBuffer->SetConstantBuffer(buffer->Buffer());
}
