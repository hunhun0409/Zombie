#include "Framework.h"
#include "WeaponScene.h"

WeaponScene::WeaponScene()
{
	//sword = new MeleeWeapon("sword");

	gun = new RangeWeapon("sniper", 0.1f);
	gun->SetProjectile("bullet", "0", 10);
}

WeaponScene::~WeaponScene()
{
	//delete sword;

	delete gun;
}

void WeaponScene::Update()
{
	if (KEY_DOWN(VK_RBUTTON))
	{
		gun->Shoot(1000, 2);
	}

	gun->Update();

	ProjectileManager::Get()->Update();
	//sword->Update();
}

void WeaponScene::PreRender()
{
}

void WeaponScene::Render()
{
	gun->Render();

	ProjectileManager::Get()->Render();
	//sword->Render();
}

void WeaponScene::PostRender()
{
}

void WeaponScene::GUIRender()
{
	gun->GUIRender();

	ProjectileManager::Get()->GUIRender();
	//sword->GUIRender();
}
