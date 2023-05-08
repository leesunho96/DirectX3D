#include "00_Global.fx"
#include "00_Render.fx"

float3 Direction = float3(-1, -1, 1);

float4 PS(MeshOutput input) : SV_Target
{
    float3 normal = normalize(input.Normal);
    float3 light = -Direction;

    return DiffuseMap.Sample(LinearSampler, input.Uv) * dot(light, normal);
}

technique11 T0
{
	P_VP(P0, VS_Mesh, PS)
    P_VP(P1, VS_Model, PS)
    P_VP(P2, VS_Animation, PS)
}