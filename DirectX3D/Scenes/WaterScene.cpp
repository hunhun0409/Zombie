#include "Framework.h"
#include "WaterScene.h"

WaterScene::WaterScene()
{
	skybox = new Skybox(L"Textures/Skybox/Night.png");

	forest = new Model("Forest");
	
	human = new Human();
	
	terrain = new Terrain("test");

	//refraction = new Refraction(L"Textures/Landscape/Wave.dds");
	water = new Water(L"Textures/Landscape/Wave.dds", 500, 500);
}

WaterScene::~WaterScene()
{
	delete skybox;
	delete forest;
	delete human;
	//delete refraction;
	delete terrain;
	delete water;
}

void WaterScene::Update()
{
	human->Update();
	forest->UpdateWorld();
	//refraction->Update();
	water->Update();
	terrain->UpdateWorld();
}

void WaterScene::PreRender()
{
	//refraction->SetRefraction();
	water->SetRefraction();

	skybox->Render();
	terrain->Render();
	human->Render();

	water->SetReflection();

	skybox->Render();
	terrain->Render();
	human->Render();
}

void WaterScene::Render()
{
	skybox->Render();

	//refraction->SetRender();
	//forest->SetShader(L"Environment/Refraction.hlsl");
	//forest->Render();

	terrain->Render();
	water->Render();

	human->Render();
}

void WaterScene::PostRender()
{
	//refraction->PostRender();
}

void WaterScene::GUIRender()
{
	//forest->GUIRender();
	terrain->GUIRender();
	human->GUIRender();
	//refraction->GUIRender();
	water->GUIRender();
}
