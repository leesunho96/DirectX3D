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

	// shader�� ���� �Ҵ�� rendering ������ �Ҵ��ϴ°��� ����.
	// ������ ���̴��� ����ϴ� ��쵵 ���� �� �� �����Ƿ�, �� ��ü�� ������ ������ �ش� �������� �ʿ��� ������ �Է� ��, ������ �ϴ°��� ȿ����.
	sDirection->SetFloatVector(direction);
	meshQuad->Render();
}