#include "Framework.h"
#include "ZombieScene.h"

ZombieScene::ZombieScene()
{
	Timer::Get()->SetLockFPS(80);

	Audio::Get()->Add("BG", "Sounds/BG/wind_forest.wav", true, true);
	
	ParticleManager::Get()->Add("BloodExplode", "TextData/Particle/BloodExplode.fx", 20);
	
	DataManager::Get();
	LevelUpSystem::Get();
	
	Environment::Get()->GetLight(0)->color = { 0.4f, 0.2f, 0.2f, 1.0f };

	tutorialPanel = new Panel(L"Textures/UI/tutorial.png");
	tutorialPanel->Show({ CENTER_X, CENTER_Y, 0 });
	tutorialPanel->UpdateWorld();

}

ZombieScene::~ZombieScene()
{
	delete terrain;
	
	delete tutorialPanel;


	ParticleManager::Delete();
	InstanceCharacterManager::Delete();
	InstanceObjectManager::Delete();
	LevelUpSystem::Delete();
	StageManager::Delete();
	DataManager::Delete();
}

void ZombieScene::Update()
{
	if (isTutorial)
	{
		stopTime -= DELTA;
		if (stopTime <= 0)
		{
			Timer::Get()->SetDeltaScale(0.0f);
		}
		if (KEY_DOWN(VK_RBUTTON))
		{
			isTutorial = false;
			Timer::Get()->SetDeltaScale(1.0f);
			tutorialPanel->Hide();
		}
	}

	if (KEY_DOWN('X'))
	{
		PlayerController::Get()->Possess(player);
	}
	if (KEY_DOWN('C'))
	{
		PlayerController::Get()->Possess(player2);

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
	//player2->Update();


	InstanceObjectManager::Get()->Update();
	InstanceCharacterManager::Get()->Update();
	ParticleManager::Get()->Update();
	ProjectileManager::Get()->Update();
	ColliderManager::Get()->Update();

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

	InstanceObjectManager::Get()->Render();
	InstanceCharacterManager::Get()->Render();
	ProjectileManager::Get()->Render();

	ParticleManager::Get()->Render();

	//ColliderManager::Get()->Render();
	SkillManager::Get()->Render();
	player->Render();
	//player2->Render();
}

void ZombieScene::PostRender()
{
	player->PostRender();
	LevelUpSystem::Get()->Render();
	StageManager::Get()->Render();
	tutorialPanel->Render();
}

void ZombieScene::GUIRender()
{
	/*StageManager::Get()->GUIRender();
	ColliderManager::Get()->GUIRender();
	InstanceCharacterManager::Get()->GUIRender();*/


	//InstanceObjectManager::Get()->GUIRender();

	//player->GUIRender();

	//SkillManager::Get()->GUIRender();

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

	player2 = new Knight();
	player2->Pos() = { 256, 0, 276 };
	player2->GetCollider();

	PlayerController::Get()->Possess(player);

	SkillManager::Get();
	SkillManager::Get()->SetOwner(player);

	StageManager::Get();

	InstanceCharacterManager::Get()->Add("ZombieWoman", "ZombieWoman");
	InstanceCharacterManager::Get()->Add("ZombieMutant", "ZombieMutant");
	InstanceCharacterManager::Get()->SetTarget(player);
	InstanceCharacterManager::Get()->SetAstar(aStar);
	InstanceCharacterManager::Get()->SetTerrain(terrain);

	InstanceObjectManager::Get()->Add("exp", "sphere");
	
	Observer::Get()->ExcuteParamEvent("ExpSetTarget", player);

	Audio::Get()->Play("BG", 0.5f);

}

void ZombieScene::End()
{
	ColliderManager::Get()->Clear();
	SkillManager::Get()->Clear();

	InstanceCharacterManager::Get()->Remove("ZombieWoman");
	InstanceCharacterManager::Get()->Remove("ZombieMutant");

	qt->Clear();
	delete qt;

	delete aStar;
	delete player;
	//delete player2;
}
