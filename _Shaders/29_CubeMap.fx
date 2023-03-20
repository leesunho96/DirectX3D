//zz#include "00_Global.fx"

//TextureCube CubeMap;

//struct VertexOutput
//{
//    float4 Position : SV_Position;
//    float3 oPosition : Position1;
//    float3 Normal : Normal;
//};

//VertexOutput VS(VertexTextureNormal input)
//{
//    VertexOutput output;
    
//    output.oPosition = input.Position.xyz;
    
//    input.Position.x += cos(Time) * 3.0f;
//    output.Position = WorldPosition(input.Position);
//    output.Position = ViewProjection(output.Position);
//    output.Normal = WorldNormal(input.Normal);
    
//    return output;
//}

//float4 PS(VertexOutput input) : SV_Target
//{
//    float4 color = float4(sin(Time * 3), 1, 1, 1);
    
//    return CubeMap.Sample(LinearSampler, input.oPosition) * color;
//}

//technique11 T0
//{
//    P_VP(P0, VS, PS)
//    P_RS_VP(P1, FillMode_WireFrame, VS, PS)
//}



matrix World;
matrix View;
matrix Projection;

TextureCube CubeMap;


struct VertexInput
{
    float4 Position : Position;
    float2 UV : Uv;
    float3 Normal : Normal;


};


struct VertexOutput
{
    float4 Position : SV_Position; // SV_Position = position1 УыБо
    float3 oPosition : Position1;
    float2 UV : Uv;
    float3 Normal : Normal;
    
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.oPosition = input.Position.xyz;

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
    return CubeMap.Sample(LinearSampler, input.oPosition);
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