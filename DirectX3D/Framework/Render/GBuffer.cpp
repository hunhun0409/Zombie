#include "Framework.h"

GBuffer::GBuffer()
{
    diffuseRender = new RenderTarget();
    specularRender = new RenderTarget();
    normalRender = new RenderTarget();
    ambientRender = new RenderTarget();
    emissiveRender = new RenderTarget();

    depthStencil = new DepthStencil(WIN_WIDTH, WIN_HEIGHT, true);

    rtvs[0] = diffuseRender;
    rtvs[1] = specularRender;
    rtvs[2] = normalRender;
    rtvs[3] = ambientRender;
    rtvs[4] = emissiveRender;

    srvs[0] = depthStencil->GetSRV();
    srvs[1] = diffuseRender->GetSRV();
    srvs[2] = specularRender->GetSRV();
    srvs[3] = normalRender->GetSRV();
    srvs[4] = ambientRender->GetSRV();
    srvs[5] = emissiveRender->GetSRV();

    FOR(6)
    {
        quads[i] = new Quad(Vector2(200, 200));
        quads[i]->Pos() = { 100 + (float)i * 200, 100, 0 };
        Texture* texture = Texture::Add(L"G" + to_wstring(i), srvs[i]);
        quads[i]->GetMaterial()->SetDiffuseMap(texture);
        quads[i]->UpdateWorld();
    }
}

GBuffer::~GBuffer()
{
    delete diffuseRender;
    delete specularRender;
    delete normalRender;
    delete ambientRender;
    delete emissiveRender;
    delete depthStencil;

    for (Quad* quad : quads)
        delete quad;
}

void GBuffer::PostRender()
{
    for (Quad* quad : quads)
        quad->Render();
}

void GBuffer::SetSRVs()
{
    FOR(6)
    {
        DC->PSSetShaderResources(10 + i, 1, &srvs[i]);
    }
}

void GBuffer::SetMultiRenderTarget()
{
    RenderTarget::SetMulti(rtvs, 5, depthStencil);
}
