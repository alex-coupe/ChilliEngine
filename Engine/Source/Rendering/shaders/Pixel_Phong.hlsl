#include "LightingFuncs.hlsli"

Texture2D diffuseMap;
Texture2D specularMap : register(t1);
SamplerState samp;

cbuffer lightBuffer:register(b0)
{
    float3 lightVector;
    float3 lightAmbient;
    float3 lightDiffuse;
    float3 lightSpecular;
    float lin;
    float quadratic;
    float constant;
};

cbuffer objectBuffer : register(b1)
{
    float3 objDiffuse;
    float3 objSpecular;
    float shininess;
}

float4 main(float2 TexCoord : TexCoord, float4 pos : SV_Position, float3 normal : NORMAL, float3 worldPos : Position, float3 camPos: Position1) : SV_TARGET
{
    float3 viewDir = normalize(camPos - worldPos);
    
    objMaterial objMat;
    objMat.diffuse = objDiffuse * (float3) diffuseMap.Sample(samp, TexCoord);
    objMat.shininess = shininess;
    objMat.specular = objSpecular * (float3) specularMap.Sample(samp, TexCoord);
    
    lightObj light;
    light.vec = lightVector;
    light.ambient = lightAmbient;
    light.diffuse = lightDiffuse;
    light.specular = lightSpecular;
    light.attlinear = lin;
    light.attconstant = constant;
    light.attquadratic = quadratic;
    
    return CalcPointLight(normal, light, viewDir, worldPos, objMat);
}