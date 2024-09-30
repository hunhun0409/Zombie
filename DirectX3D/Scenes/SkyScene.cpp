#include "Framework.h"
#include "SkyScene.h"

SkyScene::SkyScene()
{
	skybox = new Skybox(L"Textures/skybox/day_sunless.png");
}

SkyScene::~SkyScene()
{
	delete skybox;
}

void SkyScene::Update()
{
}

void SkyScene::PreRender()
{
}

void SkyScene::Render()
{
	skybox->Render();
}

void SkyScene::PostRender()
{
}

void SkyScene::GUIRender()
{
}
