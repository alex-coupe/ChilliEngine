cbuffer buffer
{
    matrix model;
    matrix modelViewProj;
};

struct VSOut
{
    float2 TexCoord : TexCoord;
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float3 camPos : Position;
};

VSOut main(float3 pos : POSITION, float2 TexCoord : TexCoord, float3 normal : NORMAL)
{
    VSOut vso;
    vso.camPos = (float3) mul(float4(pos, 1.0f), model);
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
    vso.TexCoord = TexCoord;
    vso.normal = mul(normal, (float3x3) model);
    return vso;
}