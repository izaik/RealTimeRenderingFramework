Texture2D diffuseTex : register(t0);
Texture2D specularTex : register(t1);
Texture2D normalTex : register(t2);
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
    float2 texCoord : TEXCOORD;
    float3x3 TBN : TBN;
};

VS_Out VS_main(float3 Position : POSITION, float3 Normal : NORMAL, float2 TexCoord : TEXCOORD, float4 Tangent : TANGENT)
{   
    VS_Out output = (VS_Out)0;

    float4 Pos4 = float4(Position, 1.0f);
    output.position = mul(Pos4, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
    
    output.worldPos = mul(Pos4, World);
    output.texCoord = float2(TexCoord.x, -TexCoord.y);
    
    // Transform input Normal and Tangent to worldspace
    float3 N = normalize(mul(Normal, (float3x3) WorldInvTranspose)); //use the transposed inverse of the world matrix here to ensure tangent remains perpendicular to normal
    float3 T = normalize(mul(Tangent.xyz, (float3x3) World));
    // Reorthogonalize T with respect to N (gram-schmidt stuff)
    T = normalize(T - dot(N, T) * N);
    // Compute bitangent
    float3 B = cross(N, T) * Tangent.w; //multiply by handedness value stored in Tangent.w component

    output.TBN = float3x3(T, B, N);
    
    return output;
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    //get color from texture
    float4 texColor = diffuseTex.Sample(anisotropicSampler, input.texCoord);
    float4 texNormal = normalTex.Sample(anisotropicSampler, input.texCoord);
    
    //convert from range 0 - 1 to -1 to 1
    float3 normal = texNormal.xyz * 2 - 1;
    //convert normal from tangent space to worldspace
    normal = normalize(mul(normal, input.TBN));
    
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
    float specularAmount = pow(max(dot(camDir, reflectDir), 0.0), Shininess);
    float4 specular = light.Specular * (texSpecular * specularAmount);
    
    float4 color = ambient + diffuse + specular;
    
    return color;
}