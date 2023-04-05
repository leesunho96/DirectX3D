#include "stdafx.h"
#include "AnimationDemo.h"
#include "Writer/Converter.h"

void AnimationDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(20, 0, 0);
	Context::Get()->GetCamera()->Position(1, 36, -85);


	shader = new Shader(L"38_Model.fx");

	Kachujin();


	sky = new CubeSky(L"Environment/GrassCube1024.dds");


}

void AnimationDemo::Update()
{
	sky->Update();

	if (kachujin != NULL) kachujin->Update();
}

void AnimationDemo::Render()
{
	ImGui::SliderFloat3("Animation Demo Direction", direction, -1, +1);
	shader->AsVector("Direction")->SetFloatVector(direction);

	static int pass = 0;
	ImGui::InputInt("Pass2", &pass);
	pass %= 2;



	sky->Render();

	if (kachujin != NULL)
	{
		kachujin->Pass(pass);
		kachujin->Render();
	}
}


void AnimationDemo::Kachujin()
{
	kachujin = new ModelAnimator(shader);
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadClip(L"Kachujin/Sword And Shield Idle");
	kachujin->ReadClip(L"Kachujin/Sword And Shield Run");
	kachujin->ReadClip(L"Kachujin/Sword And Shield Slash");
	kachujin->ReadClip(L"Kachujin/Salsa Dancing");

	kachujin->GetTransform()->Position(0, 0, -30);
	kachujin->GetTransform()->Scale(0.05, 0.05, 0.05);
}
