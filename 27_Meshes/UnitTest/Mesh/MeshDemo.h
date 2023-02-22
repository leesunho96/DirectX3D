#pragma once
#include "Systems/IExecute.h"

//#define RENDERMESHQUAD 1
//#define RENDERMESHCUBE 2
#define RENDERMESHGRID 3




class MeshDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {}
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	Shader* shader;

#ifdef RENDERMESHQUAD
	MeshQuad* meshQuad;
#endif


#ifdef RENDERMESHCUBE
	MeshCube* meshCube;

#endif


#ifdef RENDERMESHGRID
	MeshGrid* meshGrid;

#endif


	Vector3 direction = Vector3(-1, -1, 1);


	ID3DX11EffectVectorVariable* sDirection;
};