#include "LightingFuncs.hlsli"
#define MAX_POINTLIGHT_COUNT 4
#define MAX_SPOTLIGHT_COUNT 4

cbuffer objectBuffer : register(b0)
{
    float3 objDiffuse;
    float3 objSpecular;
    float shininess;
}

cbuffer lightBuffer : register(b1)
{
    lightCount lightcount;
    dirLight dirlight;
    pointLight pointlights[MAX_POINTLIGHT_COUNT];
    spotLight spotlights[MAX_SPOTLIGHT_COUNT];
};

float4 main(float2 TexCoord : TexCoord, float4 pos : SV_Position, float3 normal : NORMAL, float3 worldPos : Position, float3 camPos : Position1) : SV_TARGET
{
    float3 viewDir = normalize(camPos - worldPos);
    float4 result = float4(0.0f, 0.0f, 0.0f, 1.0f);
    objMaterial objMat;
    objMat.diffuse = objDiffuse;
    objMat.shininess = shininess;
    objMat.specular = objSpecular;
    
    if (lightcount.dirLightCount > 0)      
        result += CalcDirectionalLight(normal, dirlight, viewDir, objMat);
    
    int pointLightCount = min(lightcount.pointLightCount, MAX_POINTLIGHT_COUNT);
    for (int i = 0; i < pointLightCount; i++)  
        result += CalcPointLight(normal, pointlights[i], viewDir, worldPos, objMat);
  
    int spotLightCount = min(lightcount.spotLightCount, MAX_SPOTLIGHT_COUNT);
    for (int j = 0; j < spotLightCount; j++) 
        result += CalcSpotLight(normal, spotlights[j], viewDir, worldPos, objMat);
    
    return result;
}