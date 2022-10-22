cbuffer lightBuffer : register(b0)
{
    float3 lightPos;
    float3 lightColor;
    float3 viewPos;
};

cbuffer objectBuffer : register(b1)
{
    float3 objAmbient;
    float3 objDiffuse;
    float3 objSpecular;
    float shininess;
};

float4 main(float2 TexCoord : TexCoord, float4 pos : SV_Position, float3 normal : NORMAL, float3 camPos : Position) : SV_TARGET
{
    float3 ambient = objAmbient * lightColor;
    
    float3 lightDir = normalize(lightPos - camPos);
    float diff = max(dot(normal, lightDir), 0.0);
    float3 diffuse = (diff * objDiffuse) * lightColor;
    
    float3 viewDir = normalize(viewPos - camPos);
    float3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    float3 specular = (objSpecular * spec) * lightColor;
    
    float3 result = ambient + diffuse + specular;
    return float4(result.rgb, 1.0f);
}