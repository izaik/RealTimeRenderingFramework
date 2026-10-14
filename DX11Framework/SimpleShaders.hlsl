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
    struct Light
    {
        float4 Diffuse;
        float4 Ambient;
        float4 Specular;
        float3 Direction;
    } light;
    float timeSinceStart;
    float3 camPos;
}

struct VS_Out
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float4 worldPos : WORLD_POSITION;
};

VS_Out VS_main(float3 Position : POSITION, float3 Normal : NORMAL)
{   
    VS_Out output = (VS_Out)0;

    float4 Pos4 = float4(Position, 1.0f);
    output.position = mul(Pos4, World);
    float4 worldPos = output.position;
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
    
    float3 normal = normalize(Normal);
    output.normal = mul(float4(normal, 0), World);
    
    return output;
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    float4 ambient = light.Ambient * mat.Ambient;
    
    float4 normal = normalize(input.normal);
    
    float4 potentialDiffuse = mat.Diffuse * light.Diffuse;
    float3 lightDir = normalize(-light.Direction - input.worldPos.xyz);
    float diffuseAmount = max(dot(normal.xyz, lightDir), 0.0);
    float4 diffuse = potentialDiffuse * diffuseAmount;
    
    float3 camDir = normalize(camPos - input.worldPos.xyz);
    float3 reflectDir = reflect(-lightDir, normal.xyz);
    float specularAmount = pow(max(dot(camDir, reflectDir), 0.0), mat.Shininess);
    float4 specular = light.Specular * (mat.Specular * specularAmount);
    
    float4 color = ambient + diffuse + specular;
    
    return color;
}