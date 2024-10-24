#include "Framework.h"

OrbitalRifle::OrbitalRifle()
	:ActiveSkill("OrbitalRifle", "OrbitalRifle", L"Textures/UI/RifleIcon.png", 10, 5)
{
	pivot = new Transform();

	rifles.reserve(5);
	for (int i = 0; i < 5; i++)
	{
		RangeWeapon* rifle = new RangeWeapon("Rifle", 0.2f);
		rifle->SetParent(pivot);
		rifle->GetMesh()->Pos().x = distance * cos(i * XM_2PI / 5);
		rifle->GetMesh()->Pos().z = distance * sin(i * XM_2PI / 5);
		rifle->GetMesh()->Pos().y = distance * 0.5f;
		rifle->SetProjectile("bullet", to_string(i), damage);
		rifles.push_back(rifle);
	}
}

OrbitalRifle::~OrbitalRifle()
{
	for (RangeWeapon* rifle : rifles)
	{
		delete rifle;
	}
}

void OrbitalRifle::Update()
{
	if (!owner->Active()) return;

	pivot->Pos() = owner->GlobalPos();
	pivot->Rot().y += DELTA;
	pivot->UpdateWorld();

	for (int i = 0; i < level; i++)
	{
		rifles[i]->Update();
		rifles[i]->Shoot();
	}
}

void OrbitalRifle::Render()
{
	if (!owner->Active()) return;

	for (int i = 0; i < level; i++)
	{
		rifles[i]->Render();
	}
}

void OrbitalRifle::Activate()
{
	ActiveSkill::Activate();

	for (int i = 0; i < level; i++)
	{
		rifles[i]->SetActive(true);
	}
}

void OrbitalRifle::Deactivate()
{
	ActiveSkill::Deactivate();

	for (int i = 0; i < level; i++)
	{
		rifles[i]->SetActive(false);
	}
}

void OrbitalRifle::SetOwner(Character* owner)
{
	Skill::SetOwner(owner);
	//pivot->SetParent(owner);

	for (RangeWeapon* rifle : rifles)
	{
		rifle->SetOwner(owner);
	}
}
