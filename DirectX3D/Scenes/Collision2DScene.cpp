#include "Framework.h"
#include "Collision2DScene.h"

Collision2DScene::Collision2DScene()
{
	rect = new RectCollider(Vector3(WIN_WIDTH-2, WIN_HEIGHT-2, 0));
	rect->Pos() = { CENTER_X, CENTER_Y };
	rect->UpdateWorld();

	circle = new CircleCollider(20.0f);
	
	btn = new Button(L"Textures/Color/White.png");
	btn->SetTag("button");
	btn->Load();
	//btn->Pos() = { CENTER_X,CENTER_Y };


	panel = new Panel(L"Textures/karina.jpg");
	panel->Pos() = { CENTER_X,CENTER_Y };
	panel->Scale() *= 0.2f;
	panel->AddButton("button", btn, { 0, 10, 0 });
	panel->SetTag("panel");
	
}

Collision2DScene::~Collision2DScene()
{
	delete rect;
	delete circle;

	delete btn;
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

	btn->Update();
	panel->Update();

	if (KEY_DOWN(VK_RBUTTON))
	{
		panel->Show(mousePos);
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
	btn->Render();
}

void Collision2DScene::GUIRender()
{
	panel->GUIRender();
	btn->GUIRender();
}
