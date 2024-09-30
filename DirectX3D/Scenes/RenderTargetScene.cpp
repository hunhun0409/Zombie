#include "Framework.h"
#include "RenderTargetScene.h"

RenderTargetScene::RenderTargetScene()
{
    forest = new Model("Forest");

    particle = new Rain();
    particle->Play(Vector3());

    renderTarget = new RenderTarget();
    depthStencil = new DepthStencil();

    quad = new Quad(Vector2(500, 500));
    quad->Pos() = { CENTER_X, CENTER_Y };
    quad->GetMaterial()->SetShader(L"PostEffect/Bloom.hlsl");
    quad->GetMaterial()->SetDiffuseMap(Texture::Add(L"Render", renderTarget->GetSRV()));
    quad->UpdateWorld();

    valueBuffer = new FloatValueBuffer();
    valueBuffer->Get()[0] = 0.9f;
    valueBuffer->Get()[1] = 4.0f;
}

RenderTargetScene::~RenderTargetScene()
{
    delete forest;

    delete renderTarget;
    delete depthStencil;

    delete quad;
    delete valueBuffer;

    delete particle;
}

void RenderTargetScene::Update()
{
    particle->Update();
}

void RenderTargetScene::PreRender()
{

    renderTarget->Set(depthStencil);
    forest->Render();
    particle->Render();
}

void RenderTargetScene::Render()
{    
}

void RenderTargetScene::PostRender()
{
    valueBuffer->SetPS(10);
    quad->Render();

}

void RenderTargetScene::GUIRender()
{
    ImGui::SliderFloat("Threshold", &valueBuffer->Get()[0], 0, 1.0f);
    ImGui::SliderFloat("Value", &valueBuffer->Get()[1], 0, 10);

    particle->GUIRender();
}
