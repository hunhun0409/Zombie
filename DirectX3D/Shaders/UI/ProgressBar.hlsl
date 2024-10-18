#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
};

PixelInput VS(VertexUV input)
{
	PixelInput output;
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	output.uv = input.uv;
	
	return output;
}

cbuffer ValueBuffer : register(b10)
{
	float fillAmount;
}

Texture2D backImage : register(t10);

float4 PS(PixelInput input) : SV_TARGET
{	
	if (input.uv.x < fillAmount)
    {
        float4 albedo = diffuseMap.Sample(samp, input.uv);
        albedo += backImage.Sample(samp, input.uv);
        return albedo;
    }
		

	return backImage.Sample(samp, input.uv);
}