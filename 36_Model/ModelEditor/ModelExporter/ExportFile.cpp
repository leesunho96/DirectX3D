#include "stdafx.h"
#include "ExportFile.h"
#include "Converter.h"

void ExportFile::Initialize()
{
	Tank();
}

void ExportFile::Tank()
{
	Converter* converter = new Converter();
	converter->ReadFile(L"Tank/Tank.fbx");
	converter->ExportMesh(L"Tank/Tank");

	SafeDelete(converter);
}
