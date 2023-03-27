#pragma once


class MeshGrid : public Mesh
{
public:

	MeshGrid(Shader* shader, float offsetU = 1.0f, float offsetV = 1.0f);
	virtual ~MeshGrid();




private:
	void Create() override;

	float offsetU, offsetV;
};