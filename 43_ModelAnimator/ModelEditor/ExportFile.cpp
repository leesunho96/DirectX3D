#include "stdafx.h"
#include "ExportFile.h"
#include "Writer/Converter.h"

void ExportFile::Initialize()
{
	//Airplane();
	//Tower();
	//Tank();
	Kachujin();
}

void ExportFile::Airplane()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"B787/Airplane.fbx");
	conv->ExportMesh(L"B787/Airplane");
	conv->ExportMaterial(L"B787/Airplane");
	SafeDelete(conv);
}

void ExportFile::Tower()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Tower/Tower.fbx");
	conv->ExportMesh(L"Tower/Tower");
	conv->ExportMaterial(L"Tower/Tower");
	SafeDelete(conv);
}

void ExportFile::Tank()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Tank/Tank.fbx");
	conv->ExportMesh(L"Tank/Tank");

	/*
	 * Tank의 경우 외장 별도 파일 사용.
	 * 이려한 경우 우리가 개별적으로 file 복사하여 사용.
	 *
	 */
	conv->ExportMaterial(L"Tank/Tank", false);
	SafeDelete(conv);
}

void ExportFile::Kachujin()
{
	Converter* conv = nullptr;
	{
		conv = new Converter();
		conv->ReadFile(L"Kachujin/Mesh.fbx");
		conv->ExportMesh(L"Kachujin/Mesh");
		conv->ExportMaterial(L"Kachujin/Mesh");
		SafeDelete(conv);
	}
	{
		conv = new Converter();
		conv->ReadFile(L"Kachujin/Sword And Shield Idle.fbx");
		SafeDelete(conv);


		//conv = new Converter();
		//conv->ReadFile(L"Kachujin/Sword And Shield Run.fbx");
		//SafeDelete(conv);

		//conv = new Converter();
		//conv->ReadFile(L"Kachujin/Sword And Shield Slash.fbx");
		//SafeDelete(conv);

		//conv = new Converter();
		//conv->ReadFile(L"Kachujin/Salsa Dancing.fbx");
		//SafeDelete(conv);
	}


	//vector<wstring> clipNames;
//conv->ClipList(&clipNames);
}
