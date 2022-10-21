Texture2D tex;
SamplerState samp;

cbuffer buffer
{
    float3 lightPos;
    float3 lightColor;
    float4 objectColor;
    float3 viewPos;
};

float4 main(float2 TexCoord : TexCoord, float4 pos : SV_Position, float3 normal : NORMAL, float3 camPos : Position) : SV_TARGET
{
    float ambientStrength = 0.2;
    float3 ambient = ambientStrength * lightColor;
    
    float3 lightDir = normalize(lightPos - camPos);
    float diff = max(dot(normal, lightDir), 0.0);
    float3 diffuse = diff * lightColor;
    
    float specularStrength = 0.5;
    float3 viewDir = normalize(viewPos - camPos);
    float3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float3 specular = specularStrength * spec * lightColor;
    
    float4 objectCol = tex.Sample(samp, TexCoord) * objectColor;
    return float4((ambient + diffuse +specular) * objectCol.rgb, objectColor.a);
}