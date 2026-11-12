TextureCube skybox : register(t0);
SamplerState anisotropicSampler : register(s0);
cbuffer ConstantBuffer : register(b0)
{
    float4x4 Projection;
    float4x4 View;
    float4x4 World;
    float4x4 WorldInvTranspose;
    float3 camPos;
    float Shininess;
    struct Light
    {
        float4 Diffuse;
        float4 Ambient;
        float4 Specular;
        float3 Direction;
    } light;
    float timeSinceStart;
}

struct VS_Out
{
    float4 position : SV_POSITION;
    float4 worldPos : WORLD_POSITION;
    float3 texCoord : TEXCOORD;
};

VS_Out VS_main(float3 Position : POSITION, float3 Normal : NORMAL, float2 TexCoord : TEXCOORD, float4 Tangent : TANGENT)
{   
    VS_Out output = (VS_Out)0;

    float4 Pos4 = float4(Position, 1.0f);
    output.position = mul(Pos4, World);
    output.position.xyz += camPos.xyz;
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
    
    output.position = output.position.xyww; //set z to 1.0 so depth is always at the far plane
    
    output.worldPos = mul(Pos4, World);
    
    output.texCoord = float3(Position.x, -Position.y, Position.z); //negate y to fix upside down texture may need smarter methods for this in future
    
    return output;
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    //get color from texture
    float4 texColor = skybox.Sample(anisotropicSampler, input.texCoord);
    
    float4 color = texColor;
    
    return color;
}