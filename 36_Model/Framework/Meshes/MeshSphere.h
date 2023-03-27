#pragma once


class MeshSphere : public Mesh
{
public:

	MeshSphere(Shader* shader,float radius, UINT stackCount = 20, UINT sliceCount = 20);
	virtual ~MeshSphere();




private:
	void Create() override;


	float radius;

	UINT stackCount;
	UINT sliceCount;
	
};