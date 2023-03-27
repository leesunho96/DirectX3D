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
	 * �Ϲ������� DX�� �޼� ��ǥ��, Ⱦ�켱,
	 * ��κ��� 3d Graphic program : ������ ��ǥ��, ���켱
	 *
	 * -> �޼� ��ǥ��� �������ִ� ��ũ�� : aiProcess_ConvertToLeftHanded,
	 * -> Ⱦ�켱���� ������ ���� ��ġ��� ���
	 *
	 * aiProcess_Trialgulate : �ﰢ�� ���� �����ͷ� ��ȯ. �� �ܿ� ���� �� �������� ���̽� ����.
	 *
	 * aiProcess_GenUVCoords : �ﰢ���� �´� UV ��ǥ ��ȯ
	 * aiProcess_GenNormals : Normal�� �ﰢ���� �´� UV ��ǥ ��ȯ
	 *
	 */
	scene = importer->ReadFile
	(
		String::ToString(this->file),
		aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_GenNormals | aiProcess_CalcTangentSpace

	);

	assert(scene != NULL);
}
