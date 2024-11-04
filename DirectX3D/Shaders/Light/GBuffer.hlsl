#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

LightPixelInput VS(VertexUVNormalTangentBlend input)
{
	LightPixelInput output;
    matrix transform;
	
	[branch]
    if (animType)
        transform = mul(SkinWorld(input.indices, input.weights), world);
    else
        transform = world;
	
    output.pos = mul(input.pos, transform);
	
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

GBufferOutput PS(LightPixelInput input) : SV_TARGET
{
	float3 albedo = 
		diffuseMap.Sample(samp, input.uv).rgb;
    albedo *= mDiffuse;
	float specularIntencity = 
		specularMap.Sample(samp, input.uv).r;
	float3 normal = 
		NormalMapping(input.tangent, input.binormal, input.normal, input.uv);
    Material material = 
		GetMaterial(input);
    float3 ambient = mAmbient;
    float3 emissive = mEmissive;
	
	return PackGBuffer(albedo, normal, specularIntencity, ambient, emissive);
}