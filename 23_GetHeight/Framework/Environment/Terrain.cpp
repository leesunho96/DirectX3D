#include "Framework.h"
#include "Terrain.h"

Terrain::Terrain(Shader* shader, wstring heightFile)
	: shader(shader)
{
	heightMap = new Texture(heightFile);

	CreateVertexData();
	CreateIndexData();
	CreateNormalData();
	CreateBuffer();
}

Terrain::~Terrain()
{
	SafeDelete(heightMap);

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);
}

void Terrain::Update()
{

	static Vector3 direction = Vector3(-1, -1, 1);
	ImGui::SliderFloat3("Direction", direction, -1, 1);
	shader->AsVector("Direction")->SetFloatVector(direction);

	ImGui::DragInt("Pass", &pass, 1, 0, 1);
	
	Matrix world;
	D3DXMatrixIdentity(&world);



	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
	
	


}

void Terrain::Render()
{
	//for (int i = 0; i < vertexCount; i++)
	//{
	//	Vector3 start = vertices[i].Position;
	//	Vector3 end = vertices[i].Normal * 2 + start;

	//	DebugLine::Get()->RenderLine(start, end);
	//}


	UINT stride = sizeof(TerrainVertex);
	UINT offset = 0;

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);


	shader->DrawIndexed(0, (UINT)pass, indexCount);
}

float Terrain::GetHeight(Vector3& position)
{
	UINT x = (UINT)position.x;
	UINT y = (UINT)position.y;
	UINT z = (UINT)position.z;

	if(x < 0 || x > width) return FLT_MIN;
	if(z < 0 || z > height) return FLT_MIN;

	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + x + 1;
	index[3] = width * (z + 1) + x + 1;


	Vector3 v[4];
	for (size_t i = 0; i < 4; i++)
	{
		v[i] = vertices[index[i]].Position;
	}

	float ddx = (position.x - v[0].x) / 1.0f;
	float ddz = (position.z - v[0].z) / 1.0f;

	Vector3 result;


	if(ddx + ddz <= 1.0f)
		result = v[0] + (v[2] - v[0]) * ddx + (v[1] - v[0]) * ddz;
	else
	{
		ddx = 1.0f - ddx;
		ddz = 1.0f - ddz;

		result = v[3] +( v[1] - v[3]) * ddx + (v[2] - v[1]) * ddz;
	}

	return result.y;

}

float Terrain::GetHeight(float x, float y, float z)
{
	return GetHeight(Vector3(x, y, z));
}

void Terrain::CreateVertexData()
{
	vector<Color> heights;
	heightMap->ReadPixel(DXGI_FORMAT_R8G8B8A8_UNORM, &heights);
	

	width = heightMap->GetWidth();
	height = heightMap->GetHeight();


	vertexCount = width * height;
	vertices = new TerrainVertex[vertexCount];
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index = width * z + x;
			UINT pixel = width * (height - 1 - z) + x;

			vertices[index].Position.x = (float)x;
			vertices[index].Position.y = heights[pixel].r * 255.0f / 10.0f;
			vertices[index].Position.z = (float)z;
		}
	}
}

void Terrain::CreateIndexData()
{
	indexCount = (width - 1) * (height - 1) * 6;
	indices = new UINT[indexCount];

	UINT index = 0;
	for (UINT y = 0; y < height - 1; y++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices[index + 0] = width * y + x;
			indices[index + 1] = width * (y + 1) + x;
			indices[index + 2] = width * y + x + 1;
			indices[index + 3] = width * y + x + 1;
			indices[index + 4] = width * (y + 1) + x;
			indices[index + 5] = width * (y + 1) + x + 1;

			index += 6;
		}
	}
}

void Terrain::CreateNormalData()
{
	for (UINT i = 0; i < indexCount / 3; i++)
	{

		// 어떠한 사각형이던지 첫번쨰 정점의 위치 반환.
		UINT idx0 = indices[i * 3];
		// 사각형 정점 1
		UINT idx1 = indices[i * 3 + 1];
		// 사각형 정점 2
		UINT idx2 = indices[i * 3 + 2];

		VertexNormal v0 = vertices[idx0];
		VertexNormal v1 = vertices[idx1];
		VertexNormal v2 = vertices[idx2];

		Vector3 a = v1.Position - v0.Position;
		Vector3 b = v2.Position - v0.Position;

		Vector3 normal;
		D3DXVec3Cross(&normal, &a, &b);



		// 각 정점에 정규화된 노멀들을 모두 누적시킴.
		// 실제 노멀벡터를 사용할 떄에는 해당 벡터들을 정규화하여 사용.
		vertices[idx0].Normal += normal;
		vertices[idx1].Normal += normal;
		vertices[idx2].Normal += normal;	
	}

	for(UINT i = 0; i < vertexCount; i++)
	{
		D3DXVec3Normalize(&vertices[i].Normal, &vertices[i].Normal);
	}
}

void Terrain::CreateBuffer()
{
	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(TerrainVertex) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	//Create Index Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = indices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &indexBuffer));
	}
}
