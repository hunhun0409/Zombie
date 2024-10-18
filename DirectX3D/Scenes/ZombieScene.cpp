#include "Framework.h"
#include "ZombieScene.h"

ZombieScene::ZombieScene()
{
	Audio::Get()->Add("BG", "Sounds/BG/wind_forest.wav", true, true);
	Audio::Get()->Add("move", "Sounds/Zombie/move.wav", false, false, true);

	Audio::Get()->Play("BG", 0.1f);

	ParticleManager::Get()->Add("BloodExplode", "TextData/Particle/BloodExplode.fx", 20);

}

ZombieScene::~ZombieScene()
{
	delete terrain;
	delete player;
	delete skill;

	InstanceCharacterManager::Delete();
}

void ZombieScene::Update()
{
	if (KEY_DOWN('P'))
	{
		for (int i = 0; i < 50; i++)
		{
			InstanceCharacterManager::Get()->Spawn("ZombieWoman", 1);
			InstanceCharacterManager::Get()->Spawn("ZombieMutant", 1);
		}
	}
	aStar->Update();
	player->Update();
	skill->Update();

	InstanceCharacterManager::Get()->Update();
	ParticleManager::Get()->Update();
	ProjectileManager::Get()->Update();
	ColliderManager::Get()->Update();
	//ProjectileManager::Get()->IsCollision();

}

void ZombieScene::PreRender()
{
}

void ZombieScene::Render()
{
	terrain->Render();
	aStar->Render();
	skill->Render();

	InstanceCharacterManager::Get()->Render();
	ParticleManager::Get()->Render();
	ProjectileManager::Get()->Render();

	ColliderManager::Get()->Render();

	player->Render();
}

void ZombieScene::PostRender()
{
}

void ZombieScene::GUIRender()
{
	ColliderManager::Get()->GUIRender();

	//player->GUIRender();
	//InstanceCharacterManager::Get()->GUIRender();
	//ProjectileManager::Get()->GUIRender();
}

void ZombieScene::Start()
{
	terrain = new Terrain("test");
	qt = new QuadTree(Vector3(-500, 0, -500), Vector3(500, 50, 500));
	ColliderManager::Get()->SetQuadTree(qt);

	aStar = new AStar(30, 30);
	aStar->SetNode(terrain);

	player = new Knight();
	player->Pos() = { 0, 0, 0 };
	player->GetCollider();

	skill = new ObitalRifle();
	skill->SetOwner(player);
	
	PlayerController::Get()->Possess(player);

	CAM->SetTarget(player);
	CAM->TargetOptionLoad("ZombieSurvivalCamera");
	CAM->LookAtTarget();

	player->SetCamera(CAM);

	InstanceCharacterManager::Get()->Add("ZombieWoman", "ZombieWoman");
	InstanceCharacterManager::Get()->Add("ZombieMutant", "ZombieMutant");
	InstanceCharacterManager::Get()->SetTarget(player);
	InstanceCharacterManager::Get()->SetAstar(aStar);
	InstanceCharacterManager::Get()->SetTerrain(terrain);
	
}

void ZombieScene::End()
{
	InstanceCharacterManager::Get()->Remove("ZombieWoman");
	InstanceCharacterManager::Get()->Remove("ZombieMutant");

	ColliderManager::Get()->Clear();
}
