cbuffer buffer
{
	matrix transform;
};

struct VSOut
{
	float2 TexCoord: TexCoord;
	float4 pos: SV_POSITION;
    float3 normal : NORMAL;
};

VSOut main(float3 pos : POSITION, float2 TexCoord : TexCoord, float3 normal: NORMAL)
{
	VSOut vso;
	vso.pos = mul(float4(pos,1.0f),transform);
	vso.TexCoord = TexCoord;
    vso.normal = normal;
	return vso;
}