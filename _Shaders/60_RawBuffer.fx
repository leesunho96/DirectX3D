#include "00_Size.fx"

//ByteAddressBuffer input;
RWByteAddressBuffer Output;

struct Group
{
    uint3 GroupID;
    uint3 GroupThreadID;
    uint3 DispatchThreadID;
    uint  GroupIndex;
};

struct ComputeInput
{
    uint3 GroupID : SV_GroupID;
    uint3 GroupThreadID : SV_GroupThreadID;
    uint3 DispatchThreadID : SV_DispatchThreadID;
    uint GroupIndex : SV_GroupIndex;
};

[numthreads(10, 3, 8)]
void CS(ComputeInput input)
{
    Group group;
    group.GroupID = asuint(input.GroupID);
    group.GroupThreadID = asuint(input.GroupThreadID);
    group.DispatchThreadID = asuint(input.DispatchThreadID);
    group.GroupIndex = asuint(input.GroupIndex);

    uint index = input.GroupIndex;

    uint outAddress = index * UINT3_SIZE * 4;

    Output.Store3(outAddress + 0, asuint(group.GroupID)); // 0 ~ 11 byte
    Output.Store3(outAddress + UINT3_SIZE, asuint(group.GroupThreadID)); // 12 ~ 23 byte
    Output.Store3(outAddress + UINT3_SIZE * 2, asuint(group.DispatchThreadID)); // 24 ~ 35 byte
    Output.Store(outAddress + UINT3_SIZE * 3, asuint(group.GroupIndex)); //36 ~ 39 byte.

}

technique11 T0
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);

        SetComputeShader(CompileShader(cs_5_0, CS()));

    }
}