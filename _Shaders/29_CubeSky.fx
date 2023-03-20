//#include "00_Global.fx"
//#include "00_Light.fx"
//#include "00_Render.fx"

////struct VertexOutput
////{
////    float4 Position : SV_Position;
////    float3 oPosition : Position1;
////};

////VertexOutput VS(Vertex input)
////{
////    VertexOutput output;
    
////    output.oPosition = input.Position.xyz;
    
////    output.Position = WorldPosition(input.Position);
////    output.Position = ViewProjection(output.Position);
    
////    return output;
////}

////float4 PS(VertexOutput input) : SV_Target
////{
////    return SkyCubeMap.Sample(LinearSampler, input.oPosition);
////}

//float4 PS(MeshOutput input) : SV_Target
//{
//    return SkyCubeMap.Sample(LinearSampler, input.oPosition);
//}

//technique11 T0
//{
//    P_RS_DSS_VP(P0, FrontCounterClockwise_True, DepthEnable_False, VS_Mesh, PS)
//    //P_RS_DSS_VP(P0, FrontCounterClockwise_True, DepthEnable_False, VS, PS)

//    //pass P0
//    //{
//    //    SetRasterizerState(FrontCounterClockWise_True);
//    //    SetDepthStencilState(DepthEnbale_False, 0);

//    //    SetVertexShader(CompileShader(vs_5_0, VS()));
//    //    SetPixelShader(CompileShader(ps_5_0, PS()));
//    //}
//}


matrix World;
matrix View;
matrix Projection;

TextureCube SkyCubeMap;


struct VertexInput
{
    float4 Position : Position;
    float2 UV : Uv;
    float3 Normal : Normal;


};


struct VertexOutput
{
    float4 Position : SV_Position; // SV_Position = position1 ���
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

RasterizerState FrontCounterClockWise_True
{
    FrontCounterClockWise = True;
};

DepthStencilState DepthEnable_False
{
    DepthEnable = false;
};


float4 PS(VertexOutput input) : SV_Target
{
    return SkyCubeMap.Sample(LinearSampler, input.oPosition);
}


technique11 T0
{
    pass P0
    {
        SetRasterizerState(FrontCounterClockWise_True);
        SetDepthStencilState(DepthEnable_False, 0);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}