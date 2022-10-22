cbuffer lightBuffer : register(b0)
{
    float3 lightPos;
    float3 lightAmbient;
    float3 lightDiffuse;
    float3 lightSpecular;
};

cbuffer objectBuffer : register(b1)
{
    float3 objAmbient;
    float3 objDiffuse;
    float3 objSpecular;
    float shininess;
}

float4 main(float2 TexCoord : TexCoord, float4 pos : SV_Position, float3 normal : NORMAL, float3 worldPos : Position, float3 camPos : Position1) : SV_TARGET
{
    float3 ambient = objAmbient * lightAmbient;
    
    float3 lightDir = normalize(lightPos - worldPos);
    float diff = max(dot(normal, lightDir), 0.0);
    float3 diffuse = (diff * objDiffuse) * lightDiffuse;
    
    float3 viewDir = normalize(camPos - worldPos);
    float3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    float3 specular = (objSpecular * spec) * lightSpecular;
    
    float3 result = ambient + diffuse + specular;
    return float4(result.rgb, 1.0f);
}