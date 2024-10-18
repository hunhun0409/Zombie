#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

LightPixelInput VS(VertexUVNormalTangent input)
{
    LightPixelInput output;
    output.pos = mul(input.pos, world);
	
    output.viewPos = invView._41_42_43;
    output.worldPos = output.pos;
	
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;
	
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
	
    return output;
}

Texture2D alphaMap : register(t10);
Texture2D secondMap : register(t11);
Texture2D thirdMap : register(t12);
Texture2D fourthMap : register(t13);

float4 PS(LightPixelInput input) : SV_TARGET
{
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    float4 alpha = alphaMap.Sample(samp, input.uv);
    float4 second = secondMap.Sample(samp, input.uv);
    float4 third = thirdMap.Sample(samp, input.uv);
    float4 fourth = fourthMap.Sample(samp, input.uv);
    
    float totalAlpha = alpha.r + alpha.g + alpha.b + alpha.a;
    albedo = (albedo * alpha.r + second * alpha.g + third * alpha.b + fourth * alpha.a) / totalAlpha;
    //albedo = lerp(albedo, second, alpha.r);
    //albedo = lerp(albedo, third, alpha.g);
    //albedo = lerp(albedo, fourth, alpha.b);
    
    Material material;
    material.normal = NormalMapping(input.tangent,
		input.binormal, input.normal, input.uv);
    material.diffuseColor = albedo;
    material.specularIntensity = specularMap.Sample(samp, input.uv);
    material.viewPos = input.viewPos;
    material.worldPos = input.worldPos;
    
    return CalcLights(input);
    
    //float4 color = CalcDirectional(material, lights[0]);
    //float4 ambient = CalcAmbient(material);
    //float4 emissive = CalcEmissive(material);
	
    //return color + ambient + emissive;
}