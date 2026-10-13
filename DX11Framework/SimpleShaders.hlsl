cbuffer ConstantBuffer : register(b0)
{
    float4x4 Projection;
    float4x4 View;
    float4x4 World;
    struct Material
    {
        float4 Diffuse;
        float4 Ambient;
    } mat;
    struct Light
    {
        float4 Diffuse;
        float4 Ambient;
        float3 Direction;
    } light;
    float timeSinceStart;
}

struct VS_Out
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 color : COLOR;
    float4 posW : WORLD_POSITION;
};

VS_Out VS_main(float3 Position : POSITION, float3 Normal : NORMAL)
{   
    VS_Out output = (VS_Out)0;

    float4 Pos4 = float4(Position, 1.0f);
    output.position = mul(Pos4, World);
    float4 posW = output.position;
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
    
    float3 normal = normalize(Normal);
    normal = mul(normal, World);
    output.normal = normal;
    
    return output;
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    float4 diffuseAmount = dot(normalize(input.normal), light.Direction);
    float4 diff = saturate(diffuseAmount * (mat.Diffuse * -light.Diffuse));
  
    float4 final = diff + mat.Ambient;
    
    return float4(final);
}