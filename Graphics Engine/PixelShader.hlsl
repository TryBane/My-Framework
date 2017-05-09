Texture2D Texture;
SamplerState ss;

float4 main(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD ) : SV_TARGET
{
	color = 1.0f,1.0f,1.0f,1.0f;
	return color;
}
