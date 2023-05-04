#pragma once
#include "Systems/IExecute.h"
class ModelRender;
class MeshDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {}
	virtual void Destroy() override {}
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:


private:
	Shader* shader;

	Material* gridMaterial;
	MeshRender* grid;

	Material* cubeMaterial;
	MeshRender* cube;
};