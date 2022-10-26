struct lightObj
{
    float3 vec;
    float3 ambient;
    float3 diffuse;
    float3 specular;
    float attlinear;
    float attconstant;
    float attquadratic;
};

struct objMaterial
{
    float3 diffuse;
    float3 specular;
    float shininess;
};

float4 CalcLight(float3 lightDir, float3 normal, lightObj light, float3 viewDir, objMaterial objMat)
{
    float3 ambient = objMat.diffuse * light.ambient;
    
    float diff = max(dot(normal, lightDir), 0.0);
    float3 diffuse = diff * objMat.diffuse * light.diffuse;
    
    float3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), objMat.shininess);
    float3 specular = objMat.specular * spec * light.specular;
    
    float3 result = ambient + diffuse + specular;
    return float4(result.rgb, 1.0f);
}


float4 CalcDirectionalLight(float3 normal, lightObj dirLight, float3 viewDir, objMaterial objMat)
{
    float3 lightDir = normalize(-dirLight.vec);
    return CalcLight(lightDir, normal, dirLight, viewDir, objMat);
}

float4 CalcPointLight(float3 normal, lightObj pointLight, float3 viewDir, float3 worldPos, objMaterial objMat)
{
    float3 lightDir = normalize(pointLight.vec - worldPos);
    float distance = length(pointLight.vec-worldPos);
    float attenuation = 1.0 / (pointLight.attconstant + pointLight.attlinear * distance + pointLight.attquadratic * (distance * distance));
    float4 result = CalcLight(lightDir, normal, pointLight, viewDir, objMat);
    result.rgb *= attenuation;
    return result;
    
}