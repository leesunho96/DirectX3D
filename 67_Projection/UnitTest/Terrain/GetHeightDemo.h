#pragma once
#include "Systems/IExecute.h"

class GetHeightDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {}
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override {}

private:
	Shader* shader;
	Terrain* terrain;


	Shader* triShader;
	Vector3 position = Vector3(0, 0, 0);
	ID3D11Buffer* vertexBuffer;
};