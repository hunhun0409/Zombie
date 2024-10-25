#include "Framework.h"
#include "Collision2DScene.h"

Collision2DScene::Collision2DScene()
{
	DataManager::Get();
	SkillManager::Get();

	rect = new RectCollider(Vector3(WIN_WIDTH-2, WIN_HEIGHT-2, 0));
	rect->Pos() = { CENTER_X, CENTER_Y };
	rect->UpdateWorld();

	circle = new CircleCollider(20.0f);

	panel = new LevelUpPanel();
	panel2 = new GameOverPanel();

	UIManager::Get()->Add("LevelUpPanel", panel, Vector3(CENTER_X, CENTER_Y, 0));

	StageManager::Get();
}

Collision2DScene::~Collision2DScene()
{
	delete rect;
	delete circle;
}

void Collision2DScene::Update()
{
	if (rect->IsPointCollision(mousePos))
	{
		rect->GetMaterial()->GetData().diffuse = { 1, 0, 0, 1 };
	}
	else
	{
		rect->GetMaterial()->GetData().diffuse = { 0, 1, 0, 1 };
	}

	if (rect->IsCollision(circle))
	{
		rect->GetMaterial()->GetData().diffuse = { 1, 0, 0, 1 };
		circle->GetMaterial()->GetData().diffuse = { 1, 0, 0, 1 };
	}
	else
	{
		rect->GetMaterial()->GetData().diffuse = { 0, 1, 0, 1 };
		circle->GetMaterial()->GetData().diffuse = { 0, 1, 0, 1 };
	}

	circle->Pos() = mousePos;
	circle->UpdateWorld();

	if (KEY_DOWN('G'))
	{
		panel2->Show();
	}
	panel2->Update();

	if (KEY_DOWN(VK_RBUTTON))
	{
		
		//LevelUpSystem::Get()->LevelUp();
	}
	LevelUpSystem::Get()->Update();

	StageManager::Get()->Update();
	//UIManager::Get()->Update();
}

void Collision2DScene::PreRender()
{
}

void Collision2DScene::Render()
{
}

void Collision2DScene::PostRender()
{
	rect->Render();
	circle->Render();

	StageManager::Get()->Render();
	panel2->Render();

	//UIManager::Get()->Render();
	LevelUpSystem::Get()->Render();

	
}

void Collision2DScene::GUIRender()
{
	panel2->GUIRender();

	//UIManager::Get()->GUIRender();
}
