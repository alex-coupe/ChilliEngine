cbuffer buffer
{
	matrix transform;
};

struct VSOut
{
	float2 TexCoord: TexCoord;
	float4 pos: SV_POSITION;
};

VSOut main(float3 pos : POSITION, float2 TexCoord : TexCoord)
{
	VSOut vso;
	vso.pos = mul(float4(pos,1.0f),transform);
	vso.TexCoord = TexCoord;
	return vso;
}