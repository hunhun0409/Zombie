#include "Framework.h"
#include "Collision2DScene.h"

Collision2DScene::Collision2DScene()
{
	rect = new RectCollider(Vector3(WIN_WIDTH-2, WIN_HEIGHT-2, 0));
	rect->Pos() = { CENTER_X, CENTER_Y };
	rect->UpdateWorld();

	circle = new CircleCollider(20.0f);

	panel = new LevelUpPanel();
}

Collision2DScene::~Collision2DScene()
{
	delete rect;
	delete circle;

	delete panel;
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

	panel->Update();

	if (KEY_DOWN(VK_RBUTTON))
	{
		panel->Show();
	}
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

	panel->Render();
}

void Collision2DScene::GUIRender()
{
	panel->GUIRender();
}
