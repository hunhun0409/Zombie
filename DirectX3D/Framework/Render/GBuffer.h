#pragma once

class GBuffer
{
public:
    GBuffer();
    ~GBuffer();
          
    void SetMultiRenderTarget();
    void PostRender();    
    void SetSRVs();

private:
    RenderTarget* diffuseRender;
    RenderTarget* specularRender;
    RenderTarget* normalRender;
    RenderTarget* ambientRender;
    RenderTarget* emissiveRender;

    DepthStencil* depthStencil;

    RenderTarget* rtvs[5];

    class Quad* quads[6];
    ID3D11ShaderResourceView* srvs[6];
};