//#include "00_Global.fx"
// input of meshes
struct VertexMesh
{
    float4 Position : Position;
    float2 Uv : Uv;
    float3 Normal : Normal;

    matrix Transform : InstTransform;
};




//////////////////////////////////////////////////////////////////////////////////
#define VS_GENERATE\
output.oPosition = input.Position;\
output.Position = WorldPosition(input.Position);\
output.Position = ViewProjection(output.Position);\
output.Normal = WorldNormal(input.Normal);\
output.Uv = input.Uv;


//////////////////////////////////////////////////////////////////////////////////
///// 입력 매개변수 inout -> 입출력 모두 사용. default : in.
void SetMeshWorld(inout matrix world, VertexMesh input)
{
    world = input.Transform;
}


MeshOutput VS_Mesh(VertexMesh input)
{
    MeshOutput output;    

    SetMeshWorld(World, input);
    VS_GENERATE

    return output;
}
//////////////////////////////////////////////////////////////////////////////////
struct VertexModel
{
    float4 Position : Position;
    float2 Uv : Uv;
    float3 Normal : Normal;
    float3 Tangent : Tangent;
    float4 BlendIndices : BlendIndices;
    float4 BlendWeights : BlendWeights;

    matrix Transform : InstTransform;
    uint instanceID : SV_InstanceID;
};

Texture2DArray TransformsMap;
#define MAX_MODEL_TRANSFORMS 250

cbuffer CB_Bone
{   
    uint BoneIndex;
};

void SetModelWorld(inout matrix world, VertexModel input)
{
	// world = mul(BoneTransforms[BoneIndex], world);
    float4 m0 = TransformsMap.Load(int4(BoneIndex * 4 + 0, input.instanceID, 0, 0));
    float4 m1 = TransformsMap.Load(int4(BoneIndex * 4 + 1, input.instanceID, 0, 0));
    float4 m2 = TransformsMap.Load(int4(BoneIndex * 4 + 2, input.instanceID, 0, 0));
    float4 m3 = TransformsMap.Load(int4(BoneIndex * 4 + 3, input.instanceID, 0, 0));

    matrix transform = matrix(m0, m1, m2, m3);
    world = mul(transform, input.Transform);
}

MeshOutput VS_Model(VertexModel input)
{
    MeshOutput output;
    SetModelWorld(World, input);
    VS_GENERATE

    return output;
}