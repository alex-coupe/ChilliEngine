cbuffer buffer
{
	float4 color;
}; 

float4 main(float4 Color:Color) : SV_TARGET
{
	return color;
}