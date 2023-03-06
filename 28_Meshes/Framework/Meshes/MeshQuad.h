#pragma once


class MeshQuad : public Mesh
{
public:

	MeshQuad(Shader* shader);
	virtual ~MeshQuad();




private:
	void Create() override;

};