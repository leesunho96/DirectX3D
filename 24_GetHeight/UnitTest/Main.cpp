#include "stdafx.h"
#include "Main.h"
#include "Systems/Window.h"

//#include "Vertex/Vertex_Line.h"
//#include "Vertex/Vertex_Line2.h"
//#include "Vertex/TriangleList.h"
//#include "Vertex/RectDemo.h"
//#include "Vertex/UserInterface.h"
//#include "Vertex/WorldDemo3.h"
//#include "Vertex/Index_Demo.h"
//#include "Vertex/GridDemo.h"
//#include "Vertex/RotationDemo.h"
//#include "Mesh/CubeDemo.h"


//#include "Texture/TextureLoadDemo.h"
//#include "Texture/TextureSamplerDemo.h"
//#include "Terrain/HeightMapDemo.h"
//#include "Terrain/NormalMapDemo.h"
//#include "Terrain/NormalMapDemo.h"
//#include "Terrain/GetHeightDemo.h"
#include "Terrain/RayCastDemo.h"

void Main::Initialize()
{
	//Push(new Vertex_Line());
	//Push(new Vertex_Line2());
	//Push(new TriangleList());
	/*Push(new GridDemo());
	Push(new CubeDemo());*/
	//Push(new RotationDemo());
	//Push(new TextureSamplerDemo());

	Push(new RayCastDemo());
}

void Main::Ready()
{

}

void Main::Destroy()
{
	for (IExecute* exe : executes)
	{
		exe->Destroy();
		SafeDelete(exe);
	}
}

void Main::Update()
{
	for (IExecute* exe : executes)
		exe->Update();
}

void Main::PreRender()
{
	for (IExecute* exe : executes)
		exe->PreRender();
}

void Main::Render()
{
	for (IExecute* exe : executes)
		exe->Render();
}

void Main::PostRender()
{
	for (IExecute* exe : executes)
		exe->PostRender();
}

void Main::ResizeScreen()
{
	for (IExecute* exe : executes)
		exe->ResizeScreen();
}

void Main::Push(IExecute * execute)
{
	executes.push_back(execute);

	execute->Initialize();
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR param, int command)
{
	D3DDesc desc;
	desc.AppName = L"D3D Game";
	desc.Instance = instance;
	desc.bFullScreen = false;
	desc.bVsync = false;
	desc.Handle = NULL;
	desc.Width = 1280;
	desc.Height = 720;
	desc.Background = Color(0.3f, 0.3f, 0.3f, 1.0f);
	D3D::SetDesc(desc);

	Main* main = new Main();
	WPARAM wParam = Window::Run(main);

	SafeDelete(main);

	return wParam;
}