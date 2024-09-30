#include "Framework.h"
#include "ShadowScene.h"

ShadowScene::ShadowScene()
{
	forest = new Model("Forest");
	player = new Mannequin();

	shadow = new Shadow();

	LightBuffer::Light* light = Environment::Get()->GetLight(0);
	light->type = 1;
	light->pos = { 0, 10, 5 };
	light->range = 2000.0f;

}

ShadowScene::~ShadowScene()
{
	delete forest;
	delete player;
}

void ShadowScene::Update()
{
	player->Update();
}

void ShadowScene::PreRender()
{
	shadow->SetRenderTarget();

	//forest->SetShader(L"Light/DepthMap.hlsl");
	player->SetShader(L"Light/DepthMap.hlsl");
	player->GetWeapon()->SetShader(L"Light/DepthMap.hlsl");

	//forest->Render();
	player->Render();
}

void ShadowScene::Render()
{
	shadow->SetRender();

	forest->SetShader(L"Light/Shadow.hlsl");
	player->SetShader(L"Light/Shadow.hlsl");
	player->GetWeapon()->SetShader(L"Light/Shadow.hlsl");

	forest->Render();
	player->Render();
}

void ShadowScene::PostRender()
{
	shadow->PostRender();
}

void ShadowScene::GUIRender()
{
	shadow->GUIRender();
	player->GUIRender();
}
