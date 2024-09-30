#include "Framework.h"
#include "ZombieScene.h"

ZombieScene::ZombieScene()
{
	InstanceCharacterManager::Get()->Add("ZombieWoman", "ZombieWoman");
	InstanceCharacterManager::Get()->Add("ZombieMutant", "ZombieMutant");

	Audio::Get()->Add("BG", "Sounds/BG/wind_forest.wav", true, true);
	Audio::Get()->Add("move", "Sounds/Zombie/move.wav", false, false, true);

	Audio::Get()->Play("BG", 0.1f);

	ParticleManager::Get()->Add("BloodExplode", "TextData/Particle/BloodExplode.fx", 20);
}

ZombieScene::~ZombieScene()
{
	delete terrain;
	delete player;

	InstanceCharacterManager::Delete();
}

void ZombieScene::Update()
{
	if (KEY_DOWN(VK_LBUTTON))
	{
		InstanceCharacterManager::Get()->Spawn("ZombieWoman", 1);
		InstanceCharacterManager::Get()->Spawn("ZombieMutant", 1);
	}
	player->Update();

	InstanceCharacterManager::Get()->Update();
	ParticleManager::Get()->Update();

}

void ZombieScene::PreRender()
{
}

void ZombieScene::Render()
{
	terrain->Render();

	InstanceCharacterManager::Get()->Render();
	ParticleManager::Get()->Render();

	player->Render();
}

void ZombieScene::PostRender()
{
}

void ZombieScene::GUIRender()
{
	player->GUIRender();
	InstanceCharacterManager::Get()->GUIRender();
}

void ZombieScene::Start()
{
	terrain = new Terrain("test");

	player = new Knight();
	player->Pos() = { 100, 0, 100 };

	PlayerController::Get()->Possess(player);

	CAM->SetTarget(player);
	CAM->TargetOptionLoad("PlayerCamera");
	CAM->LookAtTarget();

	player->SetCamera(CAM);

	InstanceCharacterManager::Get()->SetTarget(player);
}

void ZombieScene::End()
{
}
