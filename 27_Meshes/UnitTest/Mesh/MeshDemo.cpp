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

	// shader에 변수 할당시 rendering 직전에 할당하는것이 좋다.
	// 동일한 쉐이더를 사용하는 경우도 존재 할 수 있으므로, 한 객체의 렌더링 직전에 해당 렌더링에 필요한 변수들 입력 후, 렌더링 하는것이 효율적.
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