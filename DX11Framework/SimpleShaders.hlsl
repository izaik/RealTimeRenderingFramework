Texture2D diffuseTex : register(t0);
Texture2D specularTex : register(t1);
SamplerState anisotropicSampler : register(s0);
cbuffer ConstantBuffer : register(b0)
{
    float4x4 Projection;
    float4x4 View;
    float4x4 World;
    struct Material
    {
        float4 Diffuse;
        float4 Ambient;
        float4 Specular;
        float Shininess;
    } mat;
    float3 camPos;
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
    float4 normal : NORMAL;
    float4 worldPos : WORLD_POSITION;
    float2 texCoord : TEXCOORD;
};

VS_Out VS_main(float3 Position : POSITION, float3 Normal : NORMAL, float2 TexCoord : TEXCOORD)
{   
    VS_Out output = (VS_Out)0;

    float4 Pos4 = float4(Position, 1.0f);
    output.position = mul(Pos4, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
    
    output.worldPos = mul(Pos4, World);
    
    float3 normal = normalize(Normal);
    output.normal = mul(float4(normal, 0), World);
    
    output.texCoord = TexCoord;
    
    return output;
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    //get color from texture
    float4 texColor = diffuseTex.Sample(anisotropicSampler, input.texCoord);
    //renormalize normal after interpolation
    float4 normal = normalize(input.normal);
    
    //ambient lighting
    float4 ambient = light.Ambient * texColor;
    
    //diffuse lighting
    float4 potentialDiffuse = texColor * light.Diffuse;
    float3 lightDir = normalize(-light.Direction - input.worldPos.xyz);
    float diffuseAmount = max(dot(normal.xyz, lightDir), 0.0);
    float4 diffuse = potentialDiffuse * diffuseAmount;
    
    //get specular from texture
    float4 texSpecular = specularTex.Sample(anisotropicSampler, input.texCoord);
    //specular lighting
    float3 camDir = normalize(camPos - input.worldPos.xyz);
    float3 reflectDir = reflect(-lightDir, normal.xyz);
    float specularAmount = pow(max(dot(camDir, reflectDir), 0.0), mat.Shininess);
    float4 specular = light.Specular * (texSpecular * specularAmount);
    
    float4 color = ambient + diffuse + specular;
    
    return color;
}