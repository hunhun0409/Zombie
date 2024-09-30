#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"
#include "../Skybox.hlsli"

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
};

static const float2 arrBasePos[4] =
{
	float2(-1.0f, +1.0f),
	float2(+1.0f, +1.0f),
	float2(-1.0f, -1.0f),
	float2(+1.0f, -1.0f)
};		

PixelInput VS(uint vertexID : SV_VertexID)
{
	PixelInput output;
	output.pos = float4(arrBasePos[vertexID].xy, 0.0f, 1.0f);
	output.uv = output.pos.xy;
	
	return output;
}

Texture2D depthTexture : register(t10);
Texture2D diffuseTexture : register(t11);
Texture2D specularTexture : register(t12);
Texture2D normalTexture : register(t13);
Texture2D ambientTexture : register(t14);
Texture2D emissiveTexture : register(t15);

float ConvertDepthToLinear(float depth)
{
    float linearDepth = projection._43 / (depth - projection._33);

    return linearDepth;
}

struct SurfaceData
{
	float linearDepth;
	float3 color;
	float3 normal;
	float specInt;
	float specPow;
    float3 ambient;
    float3 emissive;
};

SurfaceData UnpackGBuffer(int2 location)
{
	SurfaceData output;
	
	int3 location3 = int3(location, 0);
	
	float depth = depthTexture.Load(location3).x;
	output.linearDepth = ConvertDepthToLinear(depth);
	
	output.color = diffuseTexture.Load(location3).rgb;

	float4 specular = specularTexture.Load(location3);
	output.specPow = shininessRange.x + specular.x * shininessRange.y;
	output.specInt = specular.y;
	
	output.normal = normalTexture.Load(location3).xyz;
	output.normal = normalize(output.normal * 2.0f - 1.0f);
	
    output.ambient = ambientTexture.Load(location3).rgb;
    output.emissive = emissiveTexture.Load(location3).rgb;

	return output;
}


float3 CalcWorldPos(float2 uv, float depth)
{
    float4 position;
	
    float2 temp;
    temp.x = 1 / projection._11;
    temp.y = 1 / projection._22;
    position.xy = uv.xy * temp * depth;
    position.z = depth;
    position.w = 1.0f;
	
    return mul(position, invView).xyz;
}

float4 PS(PixelInput input) : SV_TARGET
{
	SurfaceData data = UnpackGBuffer(input.pos.xy);
	
	Material material;
	material.normal = data.normal;		
	material.diffuseColor = float4(data.color, 1.0f);
	material.specularIntensity = data.specInt;
	material.viewPos = invView._41_42_43;
	material.worldPos = CalcWorldPos(input.uv, data.linearDepth);
    
    if (data.linearDepth >= far - 0.0001f)
    {
        return float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
	
	float4 color = 0;
	
	[unroll(MAX_LIGHT)]
	for (int i = 0; i < lightCount; i++)
	{
		[flatten]
		if (!lights[i].active)
			continue;

		[flatten]
		if (lights[i].type == 0)
			color += CalcDirectional(material, lights[i]);
		else if (lights[i].type == 1)
			color += CalcPoint(material, lights[i]);
		else if (lights[i].type == 2)
			color += CalcSpot(material, lights[i]);
		else if (lights[i].type == 3)
			color += CalcCapsule(material, lights[i]);
	}
	
    float4 emissive = float4(data.emissive, 0.0f);
	
    float up = material.normal.y * 0.5f + 0.5f;
    float4 ambient = float4(ambientLight + up * ambientCeil, 1.0f);
    ambient *= material.diffuseColor * float4(data.ambient, 1.0f);
	
    return float4(color.rgb, 1.0f);
}