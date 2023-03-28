#include "Framework.h"
#include "Model/ModelMesh.h"

ModelBone::ModelBone()
{

}

ModelBone::~ModelBone()
{

}

///////////////////////////////////////////////////////////////////////
ModelMesh::ModelMesh()
{
	boneBuffer = new ConstantBuffer(&boneDesc, sizeof(BoneDesc));
}

ModelMesh::~ModelMesh()
{
	SafeDelete(transform);
	SafeDelete(perFrame);

	//SafeDelete(material);

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	SafeDelete(boneBuffer);

}

void ModelMesh::Binding(Model* model)
{
	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(Model::ModelVertex));
	indexBuffer = new IndexBuffer(indices, indexCount);
}

void ModelMesh::SetShader(Shader* shader)
{
	this->shader = shader;

	SafeDelete(transform);
	transform = new Transform(shader);

	SafeDelete(perFrame);
	perFrame = new PerFrame(shader);

	sBoneBuffer = shader->AsConstantBuffer("CB_Bone");
}

void ModelMesh::Update()
{
	boneDesc.Index = boneIndex;

	perFrame->Update();
	transform->Update();


}

void ModelMesh::Render()
{
	boneBuffer->Render();
	sBoneBuffer->SetConstantBuffer(boneBuffer->Buffer());

	perFrame->Render();
	transform->Render();
	//material->Render();

	vertexBuffer->Render();
	indexBuffer->Render();

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->DrawIndexed(0, pass, indexCount);

}

void ModelMesh::Transforms(Matrix* transform)
{
	memcpy(boneDesc.Transform, transform, sizeof(Matrix) * MAX_MODEL_TRANSFORMS);
}

void ModelMesh::SetTransform(Transform* trasnform)
{
	this->transform->Set(transform);
}
