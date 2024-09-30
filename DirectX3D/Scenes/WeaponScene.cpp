#include "Framework.h"
#include "WeaponScene.h"

WeaponScene::WeaponScene()
{
	sword = new MeleeWeapon("sword");
}

WeaponScene::~WeaponScene()
{
	delete sword;
}

void WeaponScene::Update()
{
	sword->Update();
}

void WeaponScene::PreRender()
{
}

void WeaponScene::Render()
{
	sword->Render();
}

void WeaponScene::PostRender()
{
}

void WeaponScene::GUIRender()
{
	sword->GUIRender();
}
