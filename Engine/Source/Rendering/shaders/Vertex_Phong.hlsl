cbuffer buffer
{
    matrix model;
    matrix modelViewProj;
    float3 camPos;
};

struct VSOut
{
    float2 TexCoord : TexCoord;
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float3 worldPos : Position0;
    float3 camPos: Position1;
};

VSOut main(float3 pos : POSITION, float2 TexCoord : TexCoord, float3 normal : NORMAL)
{
    VSOut vso;
    vso.worldPos = (float3) mul(float4(pos, 1.0f), model);
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
    vso.TexCoord = TexCoord;
    vso.normal = mul(normal, (float3x3) model);
    vso.camPos = camPos;
    return vso;
}