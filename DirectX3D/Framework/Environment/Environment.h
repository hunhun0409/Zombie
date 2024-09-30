#pragma once

class Environment : public Singleton<Environment>
{
private:
    friend class Singleton;

    Environment();
    ~Environment();

public:

    void Update();
    void GUIRender();

    void Set();
    void PostSet();

    void SetViewport(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);
    void SetPerspective();
    void SetOrthographic();

    LightBuffer::Light* AddLight();
    void RemoveLight(LightBuffer::Light* light);

    Camera* GetMainCamera() { return mainCamera; }
    Matrix GetProjection() { return perspective; }
    LightBuffer::Light* GetLight(UINT index) { return &lightBuffer->Get().lights[index]; }

private:
    void CreateProjection();
    void CreateState();

private:
    ProjectionBuffer* projectionBuffer;       
    LightBuffer* lightBuffer;

    ViewBuffer* uiViewBuffer;
    
    SamplerState* samplerState;
    RasterizerState* rasterizerState[2];
    BlendState* blendState[2];
    DepthStencilState* depthStencilState[2];

    Camera* mainCamera;

    D3D11_VIEWPORT viewport;

    bool isWireMode = false;

    Matrix perspective;
    Matrix orthographic;

    float nearPlane, farPlane;
};