#include "Framework.h"

Weapon::Weapon(string name)
{
	assert(name.length() != 0);
	
	mesh = new Model(name);
	mesh->SetTag(name);
}

Weapon::~Weapon()
{
	delete mesh;
}

void Weapon::Update()
{
	mesh->UpdateWorld();
}

void Weapon::Render()
{
	if (!mesh->Active())return;
	mesh->Render();
}

void Weapon::GUIRender()
{
	mesh->GUIRender();
}
