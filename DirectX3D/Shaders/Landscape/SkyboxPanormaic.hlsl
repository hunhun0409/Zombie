#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"
#include "../Skybox.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float3 originPos : POSITION;
};

PixelInput VS(VertexUV input)
{
    PixelInput output;
    
    output.pos.xyz = mul(input.pos.xyz, (float3x3) view);
    output.pos.w = 1.0f;
    output.pos = mul(output.pos, projection);

    output.originPos = input.pos;
    
    return output;
}

float4 PS(PixelInput input) : SV_Target //¹é¹öÆÛ
{
    
    float3 worldDir = normalize(input.originPos);
    float2 uv = EquirectangularToUV(worldDir);
    
    return skyboxTexture.Sample(samp, uv);
}