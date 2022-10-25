#include "LightingFuncs.hlsli"

cbuffer lightBuffer : register(b0)
{
    float3 lightVector;
    float3 lightAmbient;
    float3 lightDiffuse;
    float3 lightSpecular;
};

cbuffer objectBuffer : register(b1)
{
    float3 objDiffuse;
    float3 objSpecular;
    float shininess;
}

float4 main(float2 TexCoord : TexCoord, float4 pos : SV_Position, float3 normal : NORMAL, float3 worldPos : Position, float3 camPos : Position1) : SV_TARGET
{
    float3 viewDir = normalize(camPos - worldPos);
    
    objMaterial objMat;
    objMat.diffuse = objDiffuse;
    objMat.shininess = shininess;
    objMat.specular = objSpecular;
    
    lightObj light;
    light.vec = lightVector;
    light.ambient = lightAmbient;
    light.diffuse = lightDiffuse;
    light.specular = lightSpecular;
    
    return CalcPointLight(normal, light, viewDir, worldPos, objMat);
}