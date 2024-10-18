#include "Framework.h"

Weapon::Weapon(string name)
	:name(name)
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
	if (!mesh->Active()) return;

	mesh->UpdateWorld();
}

void Weapon::Render()
{
	if (!mesh->Active()) return;
	mesh->Render();
}

void Weapon::GUIRender()
{
	mesh->GUIRender();
}

void Weapon::SetActive(bool isActive)
{
	this->isActive = isActive;
	mesh->SetActive(isActive);
}

bool Weapon::Active()
{
	bool active = owner->Active();
	active &= isActive;
	return active;
}
