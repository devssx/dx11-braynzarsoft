//struct VS output PS input
struct POSCOLOR
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

POSCOLOR VS(float4 inPos : POSITION, float4 inColor : COLOR)
{
	POSCOLOR output;

	output.Pos = inPos;
	output.Color = inColor;

	return output;
}

float4 PS(POSCOLOR input) : SV_TARGET
{
	return input.Color;
}