cbuffer ConstantBuffer : register(b0)
{
    float4x4 Projection;
    float4x4 View;
    float4x4 World;
    float timeSinceStart;
}

struct VS_Out
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 posW : WORLD_POSITION;
};

VS_Out VS_main(float3 Position : POSITION, float3 Normal : NORMAL)
{   
    VS_Out output = (VS_Out)0;

    float4 Pos4 = float4(Position, 1.0f);
    output.position = mul(Pos4, World);
    output.position.y += sin(timeSinceStart);
    output.posW = output.position.xyz;
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
    
    output.normal = Normal;
    
    return output;
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    return float4(input.posW * 0.5f + 0.5f, 1);
}