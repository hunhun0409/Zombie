#include "Framework.h"
#include "ZombieScene.h"

ZombieScene::ZombieScene()
{
	Timer::Get()->SetLockFPS(60);

	Audio::Get()->Add("BG", "Sounds/BG/wind_forest.wav", true, true);
	
	ParticleManager::Get()->Add("BloodExplode", "TextData/Particle/BloodExplode.fx", 20);
	
	DataManager::Get();
	LevelUpSystem::Get();
	
	Environment::Get()->GetLight(0)->color = { 0.4f, 0.2f, 0.2f, 1.0f };

}

ZombieScene::~ZombieScene()
{
	delete terrain;
	delete player;


	delete aStar;
	delete qt;

	ParticleManager::Delete();
	InstanceCharacterManager::Delete();
	InstanceObjectManager::Delete();
	LevelUpSystem::Delete();
	StageManager::Delete();
	DataManager::Delete();
}

void ZombieScene::Update()
{
	if (KEY_DOWN('L'))
	{
		player->LevelUp();

	}
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


	InstanceObjectManager::Get()->Update();
	InstanceCharacterManager::Get()->Update();
	ParticleManager::Get()->Update();
	ProjectileManager::Get()->Update();
	ColliderManager::Get()->Update();
	UIManager::Get()->Update();

	SkillManager::Get()->Update();
	LevelUpSystem::Get()->Update();
	StageManager::Get()->Update();

}

void ZombieScene::PreRender()
{
}

void ZombieScene::Render()
{
	terrain->Render();
	//aStar->Render();

	InstanceObjectManager::Get()->Render();
	InstanceCharacterManager::Get()->Render();
	ProjectileManager::Get()->Render();

	ParticleManager::Get()->Render();

	ColliderManager::Get()->Render();
	SkillManager::Get()->Render();
	player->Render();
}

void ZombieScene::PostRender()
{
	player->PostRender();
	UIManager::Get()->Render();
	LevelUpSystem::Get()->Render();
	StageManager::Get()->Render();
}

void ZombieScene::GUIRender()
{
	StageManager::Get()->GUIRender();
	//InstanceObjectManager::Get()->GUIRender();
	ColliderManager::Get()->GUIRender();
	//player->GUIRender();
	//SkillManager::Get()->GUIRender();
	InstanceCharacterManager::Get()->GUIRender();
	//ProjectileManager::Get()->GUIRender();
}

void ZombieScene::Start()
{
	terrain = new Terrain();
	
	//Ãæµ¹
	qt = new QuadTree(Vector3(0, 0, 0), Vector3(512, 50, 512));
	ColliderManager::Get()->SetQuadTree(qt);

	aStar = new AStar(50, 50);
	aStar->SetNode(terrain);

	player = new Knight();
	player->Pos() = { 256, 0, 256 };
	player->GetCollider();

	PlayerController::Get()->Possess(player);

	SkillManager::Get();
	SkillManager::Get()->SetOwner(player);

	CAM->SetTarget(player);
	CAM->TargetOptionLoad("PlayerCamera");
	CAM->LookAtTarget();

	player->SetCamera(CAM);

	StageManager::Get();

	InstanceCharacterManager::Get()->Add("ZombieWoman", "ZombieWoman");
	InstanceCharacterManager::Get()->Add("ZombieMutant", "ZombieMutant");
	InstanceCharacterManager::Get()->SetTarget(player);
	InstanceCharacterManager::Get()->SetAstar(aStar);
	InstanceCharacterManager::Get()->SetTerrain(terrain);

	InstanceObjectManager::Get()->Add("exp", "sphere");
	
	Observer::Get()->ExcuteParamEvent("ExpSetTarget", player);

	Panel* panel = new LevelUpPanel();
	UIManager::Get()->Add("LevelUpPanel", panel, Vector3(CENTER_X, CENTER_Y, 0));

	Audio::Get()->Play("BG", 0.5f);
}

void ZombieScene::End()
{
	//InstanceCharacterManager::Get()->Remove("ZombieWoman");
	//InstanceCharacterManager::Get()->Remove("ZombieMutant");
	//
	//InstanceObjectManager::Get()->Remove("exp");

	ColliderManager::Get()->Clear();
	SkillManager::Get()->Clear();

	qt->Clear();

	UIManager::Get()->Remove("LevelUpPanel");
}
