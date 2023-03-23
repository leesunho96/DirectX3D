#pragma once


/*
 * 한 프레임당 GPU로 넘어갈 데이터 셋.
 */
class PerFrame
{
public:
	PerFrame(Shader* shader);
	~PerFrame();

	void Update();
	void Render();

private:
	struct Desc
	{
		Matrix View;
		Matrix ViewInverse;
		Matrix Projection;
		Matrix VP;

		Plane Culling[4];
		Plane Clipping;

		float Time;
		float Padding[3];
	} desc;

private:
	Shader* shader;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
};