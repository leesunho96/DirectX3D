#include "stdafx.h"
#include "MeshDemo.h"



void MeshDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 0, -10);
	CreateMesh();
	sDirection = shader->AsVector("Direction");
	

}

void MeshDemo::Destroy()
{
	SafeDelete(shader);
#ifdef RENDERMESHQUAD
	SafeDelete(meshQuad);
#endif

#ifdef RENDERMESHCUBE
	SafeDelete(meshCube);
#endif
#ifdef RENDERMESHGRID
	SafeDelete(meshGrid);
#endif

#ifdef RENDERMESHCYLINDER
	for (size_t i = 0; i < 10; i++)
	{
		SafeDelete(meshCylinder[i]);
	}

#endif

#ifdef RENDERMESHSPHERE

	for (size_t i = 0; i < 10; i++)
	{
		SafeDelete(meshSphere[i]);
	}


#endif
}

void MeshDemo::Update()
{


	ImGui::SliderFloat3("Light Direction", direction, -1, 1);
	

#ifdef RENDERMESHQUAD

	static Vector3 scale = Vector3(1, 1, 1);
	ImGui::SliderFloat3("Scale", scale, 0, _MAX_INT_DIG);
	meshQuad->Scale(scale);	
	meshQuad->Update();

#endif

#ifdef RENDERMESHCUBE
//	meshCube->Scale(scale);
	meshCube->Update();
#endif

#ifdef RENDERMESHGRID
	meshGrid->Update();
#endif


#ifdef RENDERMESHCYLINDER

	for (size_t i = 0; i < 10; i++)
	{
		meshCylinder[i]->Update();
	}
#endif

#ifdef RENDERMESHSPHERE
	for (size_t i = 0; i < 10; i++)
	{
		meshSphere[i]->Update();
	}

#endif
}

void MeshDemo::Render()
{

	// shader�� ���� �Ҵ�� rendering ������ �Ҵ��ϴ°��� ����.
	// ������ ���̴��� ����ϴ� ��쵵 ���� �� �� �����Ƿ�, �� ��ü�� ������ ������ �ش� �������� �ʿ��� ������ �Է� ��, ������ �ϴ°��� ȿ����.
	sDirection->SetFloatVector(direction);

#ifdef RENDERMESHQUAD
	meshQuad->Render();
#endif

#ifdef RENDERMESHCUBE
	meshCube->Render();
#endif

#ifdef RENDERMESHGRID

	static int pass = 1;
	ImGui::InputInt("pass", &pass);
	pass %= 2;
	meshGrid->Pass(pass);
	meshGrid->Render();

#endif

#ifdef RENDERMESHCYLINDER
	for (size_t i = 0; i < 10; i++)
	{
		meshCylinder[i]->Render();
	}

#endif

#ifdef RENDERMESHSPHERE

	for (size_t i = 0; i < 10; i++)
	{
		meshSphere[i]->Render();
	}

#endif
}

void MeshDemo::CreateMesh()
{

	shader = new Shader(L"25_Mesh.fx");

#ifdef RENDERMESHQUAD

	meshQuad = new MeshQuad(shader);
	meshQuad->Scale(2, 2, 2);
	meshQuad->DiffuseMap(L"Box.png");

#endif

#ifdef RENDERMESHCUBE

	meshCube = new MeshCube(shader);
	meshCube->GetTransform()->Position(0, 5, 0);
	meshCube->GetTransform()->Scale(20, 10, 20);
	meshCube->DiffuseMap(L"Stones.png");


#endif

#ifdef RENDERMESHGRID
	meshGrid = new MeshGrid(shader, 100, 100);
	meshGrid->GetTransform()->Scale(30, 30, 30);
	meshGrid->DiffuseMap(L"Floor.png");

#endif

#ifdef RENDERMESHCYLINDER

	for (size_t i = 0; i < 5; i++)
	{
		meshCylinder[2 * i] = new MeshCylinder(shader, 0.5f, 3.0f, 20, 20);
		meshCylinder[2 * i]->GetTransform()->Position(-30, 6, -15 * (float)i - 15.0f);
		meshCylinder[2 * i]->GetTransform()->Scale(5, 5, 5);
		meshCylinder[2 * i] -> DiffuseMap(L"Bricks.png");


		meshCylinder[2 * i + 1] = new MeshCylinder(shader, 0.5f, 3.0f, 20, 20);
		meshCylinder[2 * i + 1]->GetTransform()->Position(30, 6, -15 * (float)i - 15.0f);
		meshCylinder[2 * i + 1]->GetTransform()->Scale(5, 5, 5);
		meshCylinder[2 * i + 1]->DiffuseMap(L"Bricks.png");
	}


#endif


#ifdef RENDERMESHSPHERE
	for (size_t i = 0; i < 5; i++)
	{
		meshSphere[2 * i] = new MeshSphere(shader, 0.5f);
		meshSphere[2 * i]->GetTransform() -> Position(-30, 15.5, -15 * (float)i - 15.0f);
		meshSphere[2 * i]->GetTransform()->Scale(5, 5, 5);
		meshSphere[2 * i]->DiffuseMap(L"Wall.png");



		meshSphere[2 * i + 1] = new MeshSphere(shader, 0.5f);
		meshSphere[2 * i + 1]->GetTransform() ->Position(30, 15.5, -15 * (float)i - 15.0f);
		meshSphere[2 * i + 1]->GetTransform()->Scale(5, 5, 5);
		meshSphere[2 * i + 1]->DiffuseMap(L"Wall.png");
	}

#endif


};
