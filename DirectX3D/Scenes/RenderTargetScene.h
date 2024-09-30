#pragma once

class RenderTargetScene : public Scene
{
public:
    RenderTargetScene();
    ~RenderTargetScene();
        
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    Model* forest;

    RenderTarget* renderTarget;
    DepthStencil* depthStencil;

    Particle* particle;

    Quad* quad;
    FloatValueBuffer* valueBuffer;
};