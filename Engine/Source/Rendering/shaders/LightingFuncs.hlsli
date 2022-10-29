struct dirLight
{
    float3 direction;
    float3 ambient;
    float3 diffuse;
    float3 specular;
};

struct lightCount
{
    int dirLightCount;
    int pointLightCount;
    int spotLightCount;
};

struct pointLight
{
    float3 position;
    float3 ambient;
    float3 diffuse;
    float3 specular;
    float attlinear;
    float attconstant;
    float attquadratic;
};

struct spotLight
{
    float3 position;
    float3 direction;
    float3 ambient;
    float3 diffuse;
    float3 specular;
    float attlinear;
    float attconstant;
    float attquadratic;
    float cutOff;
    float outerCutOff;
};

struct objMaterial
{
    float3 diffuse;
    float3 specular;
    float shininess;
};

float4 CalcSpotLight(float3 normal, spotLight spotlight, float3 viewDir, float3 worldPos,objMaterial objMat)
{
    float3 lightDir = normalize(spotlight.position - worldPos);
    float theta = dot(lightDir, normalize(-spotlight.direction));
    
    float diff = saturate(dot(normalize(normal), lightDir));
    
    float3 reflectDir = reflect(-lightDir, normalize(normal));
    float spec = pow(saturate(dot(reflectDir, viewDir)), objMat.shininess);
    
    float distance = length(spotlight.position - worldPos);
    float attenuation = 1.0 / (spotlight.attconstant + spotlight.attlinear * distance+spotlight.attquadratic * (distance * distance));

    float epsilon = spotlight.cutOff - spotlight.outerCutOff;
    float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0);
    
    float3 ambient = spotlight.ambient * objMat.diffuse;
    float3 diffuse = spotlight.diffuse * diff * objMat.diffuse;
    float3 specular = spotlight.specular * spec * objMat.specular;
    diffuse *= intensity;
    specular *= intensity;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return float4((ambient + diffuse + specular), 1.0f);
}

float4 CalcDirectionalLight(float3 normal, dirLight dirLight, float3 viewDir, objMaterial objMat)
{
    float3 lightDir = normalize(-dirLight.direction);
    float3 ambient = objMat.diffuse * dirLight.ambient;
    float3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    float3 diffuse = diff * objMat.diffuse * dirLight.diffuse;
    
    float3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(saturate(dot(reflectDir, viewDir)), objMat.shininess);
    float3 specular = objMat.specular * spec * dirLight.specular;
    
    float3 result = ambient + diffuse + specular;
    return float4(result.rgb, 1.0f);
}

float4 CalcPointLight(float3 normal, pointLight pointLight, float3 viewDir, float3 worldPos, objMaterial objMat)
{
    float3 lightDir = normalize(pointLight.position - worldPos);
    float distance = length(pointLight.position-worldPos);
    float attenuation = 1.0 / (pointLight.attconstant + pointLight.attlinear * distance + pointLight.attquadratic * (distance * distance));
    float3 ambient = objMat.diffuse * pointLight.ambient;
    float3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    float3 diffuse = diff * objMat.diffuse * pointLight.diffuse;
    
    float3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(saturate(dot(reflectDir, viewDir)), objMat.shininess);
    float3 specular = objMat.specular * spec * pointLight.specular;
    
    float3 result = ambient + diffuse + specular;
    result *= attenuation;
    return float4(result, 1.0f);
}