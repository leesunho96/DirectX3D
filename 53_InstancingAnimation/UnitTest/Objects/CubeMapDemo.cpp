#include "stdafx.h"
#include "CubeMapDemo.h"



void CubeMapDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 0, -10);
	CreateMesh();
	sDirection = shader->AsVector("Direction");

#ifdef RENDERCUBEMAP
	cubeMapShader = new Shader(L"29_CubeSky.fx");
	cubeMap = new CubeMap(cubeMapShader);
	cubeMap->Texture(L"Environment/Earth.dds");
	cubeMap->GetTransform()->Position(0, 20, 0);
	cubeMap->GetTransform()->Scale(10, 10,10);
#endif 

	
}

void CubeMapDemo::Destroy()
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


#ifdef RENDERCUBEMAP
	SafeDelete(cubeMap);

#endif


#endif
}

void CubeMapDemo::Update()
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

#ifdef RENDERCUBEMAP
	cubeMap->Update();
	static Vector3 cubemapRot = Vector3(0, 0, 0);
	cubemapRot += Vector3(0, 1, 0) * Time::Delta();
	cubeMap->GetTransform()->RotationDegree(cubemapRot);
#endif
}

void CubeMapDemo::Render()
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

#ifdef RENDERCUBEMAP
	cubeMap->Render();
#endif
}

void CubeMapDemo::CreateMesh()
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
		meshSphere[2 * i + 1]->GetTransform()->Position(30, 15.5, -15 * (float)i - 15.0f);
		meshSphere[2 * i + 1]->GetTransform()->Scale(5, 5, 5);
		meshSphere[2 * i + 1]->DiffuseMap(L"Wall.png");
	}

#endif


};
