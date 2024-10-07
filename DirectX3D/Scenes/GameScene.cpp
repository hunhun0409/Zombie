#include "Framework.h"
#include "GameScene.h"

GameScene::GameScene()
{
	forest = new Model("Forest");
	//forest->SetShader(L"Light/Light.hlsl");
	forest->SetTag("Forest");
	forest->UpdateWorld();
	//forest->SetShader(L"PostEffect/Fog.hlsl");

	fogBuffer = new FogBuffer();

	

	rs[0] = new RasterizerState();
	rs[1] = new RasterizerState();
	rs[1]->CullMode(D3D11_CULL_NONE);

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->AlphaToCoverage(true);

	skybox = new Skybox(L"Textures/skybox/Night.png");

}

GameScene::~GameScene()
{
	delete forest;
	

	delete blendState[0];
	delete blendState[1];

	

	delete skybox;
	delete fogBuffer;
}

void GameScene::Update()
{
	naruto->Update();

	if (KEY_DOWN('1'))
	{
		InstanceCharacterManager::Get()->Spawn("ZombieWoman", 1);
	}

	if (KEY_DOWN('2'))
	{
		ParticleManager::Get()->Play("BloodExplode", naruto->GlobalPos() + Vector3(0, 10, 0));
	}
	
	ProjectileManager::Get()->Update();
	InstanceCharacterManager::Get()->Update();
	ParticleManager::Get()->Update();
}

void GameScene::PreRender()
{
}

void GameScene::Render()
{
	skybox->Render();

	fogBuffer->SetPS(10);

	naruto->Render();

	//rs[1]->SetState();
	blendState[1]->SetState();

	forest->Render();

	//rs[0]->SetState();
	blendState[0]->SetState();

	ProjectileManager::Get()->Render();
	InstanceCharacterManager::Get()->Render();
	ParticleManager::Get()->Render();


}

void GameScene::PostRender()
{
	naruto->PostRender();
}

void GameScene::GUIRender()
{
	forest->GUIRender();
	naruto->GUIRender();
	InstanceCharacterManager::Get()->GUIRender();
	ProjectileManager::Get()->GUIRender();

	//ImGui::SliderFloat("Start", &fogBuffer->Get().start, 0, 1.0f);
	//ImGui::SliderFloat("Random", &fogBuffer->Get().random, 0, 1.0f);
	//ImGui::ColorEdit4("Color", &fogBuffer->Get().color);
}

void GameScene::Start()
{
	naruto = new Naruto();

	CAM->SetTarget(naruto);
	CAM->TargetOptionLoad("PlayerCamera");
	CAM->LookAtTarget();

	PlayerController::Get()->Possess(naruto);

	InstanceCharacterManager::Get()->Add("ZombieWoman", "ZombieWoman");

	//instancing->ReadClip("Dying");
	InstanceCharacterManager::Get()->SetTarget(naruto);

	ParticleManager::Get()->Add("BloodExplode", "TextData/Particle/BloodExplode.fx", 10);

}

void GameScene::End()
{
	delete naruto;

	ProjectileManager::Delete();
	InstanceCharacterManager::Delete();
	ParticleManager::Delete();
}
