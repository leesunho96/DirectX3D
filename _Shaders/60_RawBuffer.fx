#include "00_Size.fx"

ByteAddressBuffer Input;
RWByteAddressBuffer Output;

struct Group
{
    uint3 GroupID;
    uint3 GroupThreadID;
    uint3 DispatchThreadID;
    uint  GroupIndex;
    float RetVal;
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
    

    

    // 10, 8,, 3 from numthreads
    uint index = input.GroupID.x * 10 * 3 * 8 + input.GroupIndex;

    uint inAddress = index * FLOAT_SIZE;


    // outAddress = 변수의 개수 * 4. Output = uint3 x 3 + uint + float -> 11 * 4byte
    uint outAddress = index * UINT_SIZE * 11;
    
    float inputVal = asfloat(Input.Load(inAddress));
    group.RetVal = inputVal;



    Output.Store3(outAddress + 0, asuint(group.GroupID)); // 0 ~ 11 byte
    Output.Store3(outAddress + UINT3_SIZE, asuint(group.GroupThreadID)); // 12 ~ 23 byte
    Output.Store3(outAddress + UINT3_SIZE * 2, asuint(group.DispatchThreadID)); // 24 ~ 35 byte
    Output.Store(outAddress + UINT3_SIZE * 3, asuint(group.GroupIndex)); //36 ~ 39 byte.
    Output.Store(outAddress + UINT3_SIZE * 3 + UINT_SIZE, asuint(group.RetVal)); // 40 ~ 43Byte

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