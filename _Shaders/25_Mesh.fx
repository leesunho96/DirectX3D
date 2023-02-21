//#include "00_Global.fx"
//
//float3 Direction;
//
//struct VertexOutput
//{
//    float4 Position : SV_Position;
//    float2 Uv : Uv;
//    float3 Normal : Normal;
//};
//
//VertexOutput VS(VertexTextureNormal input)
//{
//    VertexOutput output;
//    output.Position = WorldPosition(input.Position);
//    output.Position = ViewProjection(output.Position);
//    
//    output.Normal = WorldNormal(input.Normal);
//    
//    output.Uv = input.Uv;
//    
//    return output;
//}
//
//float4 PS(VertexOutput input) : SV_Target
//{
//    float3 normal = normalize(input.Normal);
//    float3 light = -Direction;
//    
//    return DiffuseMap.Sample(LinearSampler, input.Uv) * dot(light, normal);
//}
//
//technique11 T0
//{
//    P_VP(P0, VS, PS)
//    P_RS_VP(P1, FillMode_WireFrame, VS, PS)
//
//    //pass P0
//    //{
//    //    SetVertexShader(CompileShader(vs_5_0, VS()));
//    //    SetPixelShader(CompileShader(ps_5_0, PS()));
//    //}
//
//    //pass P1
//    //{
//    //    SetRasterizerState(FillMode_Wireframe);
//
//    //    SetVertexShader(CompileShader(vs_5_0, VS()));
//    //    SetPixelShader(CompileShader(ps_5_0, PS()));
//    //}
//}


matrix World;
matrix View;
matrix Projection;

Texture2D DiffuseMap;
float3 Direction;


struct VertexInput
{
	float4 Position : Position;
	float2 UV : Uv;
	float3 Normal : Normal;


};


struct VertexOutput
{
	float4 Position : SV_Position;
	float2 UV : Uv;
	float3 Normal : Normal;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Normal = mul(input.Normal, (float3x3) World);
    output.UV = input.UV;

    return output;
}

SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

float4 PS(VertexOutput input) : SV_Target
{

    float3 normal = normalize(input.Normal);
    float3 light = -Direction;
    return DiffuseMap.Sample(LinearSampler, input.UV) * dot(normal, light);
}

RasterizerState FillMode_Wireframe
{
    FillMode = Wireframe;
};

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }

    pass P1
    {
        SetRasterizerState(FillMode_Wireframe);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}