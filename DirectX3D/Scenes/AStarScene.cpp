#include "Framework.h"
#include "AStarScene.h"

AStarScene::AStarScene()
{
    terrain = new Terrain();
    aStar = new AStar(30, 30);
    aStar->SetNode(terrain);
    fox = new Fox();
    fox->SetTerrain(terrain);
    fox->SetAStar(aStar);

    fox->Pos() = { 50, 0, 50 };

    InstanceCharacterManager::Get()->Add("ZombieWoman", "ZombieWoman");
    InstanceCharacterManager::Get()->SetAstar(aStar);
    InstanceCharacterManager::Get()->SetTerrain(terrain);
    InstanceCharacterManager::Get()->SetTarget(fox);
}

AStarScene::~AStarScene()
{
    delete terrain;
    delete aStar;
    delete fox;

    InstanceCharacterManager::Delete();
}

void AStarScene::Update()
{
    if (KEY_DOWN(VK_F4))
    {
        SceneManager::Get()->Remove("Start");
        SceneManager::Get()->Add("Game");
    }

    if (KEY_DOWN('S'))
    {
        InstanceCharacterManager::Get()->Spawn("ZombieWoman", Vector3(0,0,0));
    }

    aStar->Update();
    fox->Update();

    InstanceCharacterManager::Get()->Update();
}

void AStarScene::PreRender()
{
}

void AStarScene::Render()
{
    terrain->Render();
    aStar->Render();
    fox->Render();

    InstanceCharacterManager::Get()->Render();
}

void AStarScene::PostRender()
{
}

void AStarScene::GUIRender()
{
    fox->GUIRender();
}
