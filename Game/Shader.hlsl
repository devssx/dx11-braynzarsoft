cbuffer cbPerObject
{
	float4x4 WVP;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

//struct VS output PS input
struct POSCOLOR
{
	float4 Pos :      SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

POSCOLOR VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD)
{
	POSCOLOR output;

	output.Pos = mul(inPos, WVP);
	output.TexCoord = inTexCoord;

	return output;
}

float4 PS(POSCOLOR input) : SV_TARGET
{
	float4 diffuse = ObjTexture.Sample(ObjSamplerState, input.TexCoord);
	clip(diffuse.a - 0.25);
	return diffuse;
}