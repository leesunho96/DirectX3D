#include "stdafx.h"
#include "MeshDemo.h"

void MeshDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 0, -10);


	shader = new Shader(L"25_Mesh.fx");
	
	meshQuad = new MeshQuad(shader);
	meshQuad->Scale(2, 2, 2);
	meshQuad->DiffuseMap(L"Box.png");


	sDirection = shader->AsVector("Direction");
	

}

void MeshDemo::Destroy()
{
	SafeDelete(shader);

}

void MeshDemo::Update()
{
	static Vector3 scale = Vector3(1, 1, 1);

	

	ImGui::SliderFloat3("Scale", scale, 0, _MAX_INT_DIG);

	meshQuad->Scale(scale);	
	meshQuad->Update();


	
}

void MeshDemo::Render()
{

	// shader에 변수 할당시 rendering 직전에 할당하는것이 좋다.
	// 동일한 쉐이더를 사용하는 경우도 존재 할 수 있으므로, 한 객체의 렌더링 직전에 해당 렌더링에 필요한 변수들 입력 후, 렌더링 하는것이 효율적.
	sDirection->SetFloatVector(direction);
	meshQuad->Render();
}