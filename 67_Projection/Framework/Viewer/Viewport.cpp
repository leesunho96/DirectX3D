#include "Framework.h"
#include "Viewport.h"

Viewport::Viewport(float width, float height, float x, float y, float minDepth, float maxDepth)
{
	Set(width, height, x, y, minDepth, maxDepth);
}

Viewport::~Viewport()
{
	
}

void Viewport::RSSetViewport()
{
	D3D::GetDC()->RSSetViewports(1, &viewport);
}

void Viewport::Set(float width, float height, float x, float y, float minDepth, float maxDepth)
{
	viewport.TopLeftX = this->x = x;
	viewport.TopLeftY = this->y = y;
	viewport.Width = this->width = width;
	viewport.Height = this->height = height;
	viewport.MinDepth = this->minDepth = minDepth;
	viewport.MaxDepth = this->maxDepth = maxDepth;

	RSSetViewport();
}

void Viewport::Project(Vector3* pOutput, Vector3& source, const Matrix& W, const Matrix& V, const Matrix& P)
{
	Vector3 position = source;

	Matrix wvp = W * V * P;

	// 위치를 공간으로 변환.
	D3DXVec3TransformCoord(pOutput, &position, &wvp);

	pOutput->x = ((pOutput->x + 1.0f) * 0.5f) * width + x;
	pOutput->y = ((- pOutput->y + 1.0f) * 0.5f) * height + y;

}
