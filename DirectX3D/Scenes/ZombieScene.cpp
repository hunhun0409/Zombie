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

	ParticleManager::Delete();
	InstanceCharacterManager::Delete();
	InstanceObjectManager::Delete();
	UIManager::Delete();
}

void ZombieScene::Update()
{
	if (KEY_DOWN('P'))
	{
		for (int i = 0; i < 1; i++)
		{
			InstanceCharacterManager::Get()->Spawn("ZombieWoman", 1);
			InstanceCharacterManager::Get()->Spawn("ZombieMutant", 1);
		}
	}
	aStar->Update();
	player->Update();
	skill->Update();

	InstanceObjectManager::Get()->Update();
	InstanceCharacterManager::Get()->Update();
	ParticleManager::Get()->Update();
	ProjectileManager::Get()->Update();
	ColliderManager::Get()->Update();
	UIManager::Get()->Update();
}

void ZombieScene::PreRender()
{
}

void ZombieScene::Render()
{
	terrain->Render();
	//aStar->Render();
	skill->Render();

	InstanceObjectManager::Get()->Render();
	InstanceCharacterManager::Get()->Render();
	ParticleManager::Get()->Render();
	ProjectileManager::Get()->Render();

	//ColliderManager::Get()->Render();

	player->Render();
}

void ZombieScene::PostRender()
{
	player->PostRender();
	UIManager::Get()->Render();
}

void ZombieScene::GUIRender()
{
	//ColliderManager::Get()->GUIRender();
	//player->GUIRender();
	InstanceObjectManager::Get()->GUIRender();
	//InstanceCharacterManager::Get()->GUIRender();
	//ProjectileManager::Get()->GUIRender();
}

void ZombieScene::Start()
{
	terrain = new Terrain();
	
	qt = new QuadTree(Vector3(0, 0, 0), Vector3(512, 50, 512));
	ColliderManager::Get()->SetQuadTree(qt);

	aStar = new AStar(50, 50);
	aStar->SetNode(terrain);

	player = new Knight();
	player->Pos() = { 256, 0, 256 };
	player->GetCollider();

	skill = new ObitalRifle();
	skill->SetOwner(player);
	
	PlayerController::Get()->Possess(player);

	CAM->SetTarget(player);
	CAM->TargetOptionLoad("PlayerCamera");
	CAM->LookAtTarget();

	player->SetCamera(CAM);

	InstanceObjectManager::Get()->Add("exp", "sphere");

	InstanceCharacterManager::Get()->Add("ZombieWoman", "ZombieWoman");
	InstanceCharacterManager::Get()->Add("ZombieMutant", "ZombieMutant");
	InstanceCharacterManager::Get()->SetTarget(player);
	InstanceCharacterManager::Get()->SetAstar(aStar);
	InstanceCharacterManager::Get()->SetTerrain(terrain);
	
	Observer::Get()->ExcuteParamEvent("ExpSetTarget", player);

	Panel* panel = new LevelUpPanel();
	UIManager::Get()->Add("LevelUpPanel", panel, Vector3(CENTER_X, CENTER_Y, 0));
}

void ZombieScene::End()
{
	InstanceCharacterManager::Get()->Remove("ZombieWoman");
	InstanceCharacterManager::Get()->Remove("ZombieMutant");

	InstanceObjectManager::Get()->Remove("exp");

	ColliderManager::Get()->Clear();

	UIManager::Get()->Remove("LevelUpPanel");
}
