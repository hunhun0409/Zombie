#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewPos : POSITION0;
    float3 worldPos : POSITION1;
    float4 alpha : ALPHA;
};

PixelInput VS(VertexUVNormalTangentAlpha input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
	
    output.viewPos = invView._41_42_43;
    output.worldPos = output.pos;
	
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;
	
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
	
    output.alpha = input.alpha;
    
    return output;
}

cbuffer BrushBuffer : register(b10)
{
    int type;
    float3 brushPos;
    
    float range;
    float3 brushColor;
}

float4 BrushColor(float3 pos)
{
    if (type == 0)
    {
        float2 direction = pos.xz - brushPos.xz;
        
        float distance = length(direction);
        
        if (distance <= range)
        {
            return float4(brushColor, 0);
        }
    }
    if (type == 1)
    {
        float size = range * 0.5f;
        
        if (abs(pos.x - brushPos.x) <= size
            && abs(pos.z - brushPos.z) <= size)
        {
            return float4(brushColor, 0);
        }

    }
    
    return float4(0, 0, 0, 0);
}

Texture2D secondMap : register(t11);
Texture2D thirdMap : register(t12);
Texture2D fourthMap : register(t13);

Texture2D secondNormalMap : register(t14);
Texture2D thirdNormalMap : register(t15);
Texture2D fourthNormalMap : register(t16);

float4 PS(PixelInput input) : SV_TARGET
{
    float4 albedo = diffuseMap.Sample(samp, input.uv); // 첫 번째 텍스처
    float4 second = secondMap.Sample(samp, input.uv); // 두 번째 텍스처
    float4 third = thirdMap.Sample(samp, input.uv); // 세 번째 텍스처
    float4 fourth = fourthMap.Sample(samp, input.uv); // 네 번째 텍스처
    
    float totalAlpha = input.alpha.r + input.alpha.g + input.alpha.b + input.alpha.a;

    albedo = (albedo * input.alpha.r + second * input.alpha.g + third * input.alpha.b + fourth * input.alpha.a) / totalAlpha;
    
    float3 T = normalize(input.tangent);
    float3 B = normalize(input.binormal);
    float3 N = normalize(input.normal);
    float3x3 TBN = float3x3(T, B, N);
    
    float3 normal = normalMap.Sample(samp, input.uv);
    normal = normalize(mul(normal, TBN));
    float3 secondN = secondNormalMap.Sample(samp, input.uv);
    secondN = normalize(mul(secondN, TBN));
    float3 thirdN = secondNormalMap.Sample(samp, input.uv);
    thirdN = normalize(mul(thirdN, TBN));
    float3 fourthN = secondNormalMap.Sample(samp, input.uv);
    fourthN = normalize(mul(fourthN, TBN));
    
    float3 blendedNormal = normalize(
        normal * input.alpha.r +
        secondN * input.alpha.g +
        thirdN * input.alpha.b +
        fourthN * input.alpha.a
    );
    
    
    Material material;
    material.normal = blendedNormal;
    material.diffuseColor = albedo;
    material.specularIntensity = specularMap.Sample(samp, input.uv);
    material.viewPos = input.viewPos;
    material.worldPos = input.worldPos;
    
    float4 color = CalcDirectional(material, lights[0]);
    float4 ambient = CalcAmbient(material);
    float4 emissive = CalcEmissive(material);
    float4 brush = BrushColor(input.worldPos);
	
    return color + ambient + emissive + brush;
    
}