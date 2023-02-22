#include "stdafx.h"
#include "MeshDemo.h"



void MeshDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 0, -10);

	shader = new Shader(L"25_Mesh.fx");



#ifdef RENDERMESHQUAD

	meshQuad = new MeshQuad(shader);
	meshQuad->Scale(2, 2, 2);
	meshQuad->DiffuseMap(L"Box.png");

#endif

#ifdef RENDERMESHCUBE

	meshCube = new MeshCube(shader);
	meshCube->Position(0, 5, 0);
	meshCube->Scale(20, 10, 20);
	meshCube->DiffuseMap(L"Stones.png");
	

#endif

#ifdef RENDERMESHGRID
	meshGrid = new MeshGrid(shader, 100, 100);	
	meshGrid->DiffuseMap(L"Floor.png");

#endif

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
};