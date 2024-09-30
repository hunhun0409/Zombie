
#define PI 3.14159265


Texture2D skyboxTexture : register(t20);

float2 EquirectangularToUV(float3 dir)
{
    float2 uv;
    
    float theta = asin(dir.y);
    
    float phi = atan2(dir.z, dir.x);
    
    uv.x = (phi + PI) / (2.0 * PI);
    uv.y = 1 - (theta + PI / 2.0) / PI;
    
    return uv;
}
