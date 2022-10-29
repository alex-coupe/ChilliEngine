#include "LightingFuncs.hlsli"

cbuffer objectBuffer : register(b0)
{
    float3 objDiffuse;
    float3 objSpecular;
    float shininess;
}

cbuffer dirLightBuffer : register(b1)
{
    float3 dirlightDir;
    float3 dirlightAmbient;
    float3 dirlightDiffuse;
    float3 dirlightSpecular;
};

cbuffer pointLightBuffer : register(b2)
{
    float3 pointlightPos;
    float3 pointlightAmbient;
    float3 pointlightDiffuse;
    float3 pointlightSpecular;
    float pointlin;
    float pointquadratic;
    float pointconstant;
    
}

cbuffer spotlightBuffer : register(b3)
{
    float3 spotPos;
    float3 spotDir;
    float3 spotAmbient;
    float3 spotDiffuse;
    float3 spotSpecular;
    float spotlin;
    float spotquadratic;
    float spotconstant;
    float spotcutOff;
    float spotouterCutOff;
}

cbuffer lightDataBuffer : register(b4)
{
    int numDirLights;
    int numPointLights;
    int numSpotLights;
}

float4 main(float2 TexCoord : TexCoord, float4 pos : SV_Position, float3 normal : NORMAL, float3 worldPos : Position, float3 camPos : Position1) : SV_TARGET
{
    float3 viewDir = normalize(camPos - worldPos);
    float4 result = float4(0.0f, 0.0f, 0.0f, 1.0f);
    objMaterial objMat;
    objMat.diffuse = objDiffuse;
    objMat.shininess = shininess;
    objMat.specular = objSpecular;
    
    if (numDirLights > 0)
    {
        dirLight dirlight;
        dirlight.direction = dirlightDir;
        dirlight.ambient = dirlightAmbient;
        dirlight.diffuse = dirlightDiffuse;
        dirlight.specular = dirlightSpecular;
        
        result += CalcDirectionalLight(normal, dirlight, viewDir, objMat);
    }
    
    if (numPointLights > 0)
    {
        pointLight pointlight;
        pointlight.position = pointlightPos;
        pointlight.ambient = pointlightAmbient;
        pointlight.diffuse = pointlightDiffuse;
        pointlight.specular = pointlightSpecular;
        pointlight.attlinear = pointlin;
        pointlight.attconstant = pointconstant;
        pointlight.attquadratic = pointquadratic;
        
        result += CalcPointLight(normal, pointlight, viewDir, worldPos, objMat);
    }
    if (numSpotLights > 0)
    {
        spotLight spotlight;
        spotlight.position = spotPos;
        spotlight.direction = spotDir;
        spotlight.ambient = spotAmbient;
        spotlight.diffuse = spotDiffuse;
        spotlight.specular = spotSpecular;
        spotlight.attlinear = spotlin;
        spotlight.attconstant = spotconstant;
        spotlight.attquadratic = spotquadratic;
        spotlight.cutOff = spotcutOff;
        spotlight.outerCutOff = spotouterCutOff;
        
        result += CalcSpotLight(normal, spotlight, viewDir, worldPos, objMat);
    }
    return result;
}