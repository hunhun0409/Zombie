#include "Framework.h"

BaseballBat::BaseballBat() : Model("Sword")
{
	tag = "Sword";

	collider = new BoxCollider();
	collider->SetTag("SwordCollider");
	collider->SetParent(this);
	collider->Load();
}

BaseballBat::~BaseballBat()
{
	delete collider;
}

void BaseballBat::Update()
{
	UpdateWorld();
	collider->UpdateWorld();
}

void BaseballBat::Render()
{
	Model::Render();
	collider->Render();
}

void BaseballBat::GUIRender()
{
	Model::GUIRender();
	collider->GUIRender();
}
