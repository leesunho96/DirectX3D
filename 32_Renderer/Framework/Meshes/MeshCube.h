#pragma once


class MeshCube : public Mesh
{
public:

	MeshCube(Shader* shader);
	virtual ~MeshCube();




private:
	void Create() override;

};