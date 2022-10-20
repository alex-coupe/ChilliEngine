Texture2D tex;
SamplerState samp;

cbuffer buffer
{
	float4 color;
};

float4 main(float2 TexCoord: TexCoord) : SV_TARGET
{
	float4 colors = tex.Sample(samp,TexCoord);
    return float4(colors.r * color.r, colors.g * color.g, colors.b * color.b, colors.a * color.a);
}