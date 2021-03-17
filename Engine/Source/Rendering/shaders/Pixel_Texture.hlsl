Texture2D tex;
SamplerState samp;

float4 main(float2 TexCoord: TexCoord) : SV_TARGET
{
	return tex.Sample(samp,TexCoord);
}