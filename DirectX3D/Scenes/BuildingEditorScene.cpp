#include "Framework.h"
#include "BuildingEditorScene.h"

BuildingEditorScene::BuildingEditorScene()
{
	InstanceObjectManager::Get()->Add("House1", "House1");
}

BuildingEditorScene::~BuildingEditorScene()
{
}

void BuildingEditorScene::Update()
{
	if (KEY_DOWN('P'))
	{
		InstanceObjectManager::Get()->Spawn("House1", {0,0,0});
	}

	InstanceObjectManager::Get()->Update();
}

void BuildingEditorScene::PreRender()
{
}

void BuildingEditorScene::Render()
{
	InstanceObjectManager::Get()->Render();
}

void BuildingEditorScene::PostRender()
{
}

void BuildingEditorScene::GUIRender()
{
	InstanceObjectManager::Get()->GUIRender();
}
