#include "stdafx.h"
#include "Converter.h"

Converter::Converter()
{
	importer = new Assimp::Importer();
}

Converter::~Converter()
{
	SafeDelete(importer);
}

void Converter::ReadFile()
{
	this->file = L"../../_Assets/" + file;


	/*
	 * 일반적으로 DX는 왼손 좌표계, 횡우선,
	 * 대부분의 3d Graphic program : 오른손 좌표계, 열우선
	 *
	 * -> 왼손 좌표계로 변경해주는 매크로 : aiProcess_ConvertToLeftHanded,
	 * -> 횡우선으로 변경은 추후 전치행렬 사용
	 *
	 * aiProcess_Trialgulate : 삼각형 기준 데이터로 반환. 그 외에 복셀 등 여러가지 케이스 존재.
	 *
	 * aiProcess_GenUVCoords : 삼각형에 맞는 UV 좌표 반환
	 * aiProcess_GenNormals : Normal도 삼각형에 맞는 UV 좌표 반환
	 *
	 */
	scene = importer->ReadFile
	(
		String::ToString(this->file),
		aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_GenNormals | aiProcess_CalcTangentSpace

	);

	assert(scene != NULL);
}
