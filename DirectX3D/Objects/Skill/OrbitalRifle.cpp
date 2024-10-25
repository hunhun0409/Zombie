#include "Framework.h"

OrbitalRifle::OrbitalRifle()
	:ActiveSkill("rifle", "rifle", L"Textures/UI/RifleIcon.png", 10, 5)
{
	pivot = new Transform();

	rifles.reserve(5);
	for (int i = 0; i < 5; i++)
	{
		RangeWeapon* rifle = new RangeWeapon("Rifle", 0.2f);
		rifle->GetMesh()->Pos().x = distance * cos(i * XM_2PI / 5);
		rifle->GetMesh()->Pos().z = distance * sin(i * XM_2PI / 5);
		rifle->GetMesh()->Pos().y = distance * 0.5f;
		rifle->SetProjectile("bullet", to_string(i), 0);
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

		ProjectileManager::Get()->UpdateDamage(rifle->GetProjectileName(), owner->GetFinalStatus().finalAttack * damageRatio);
	}
}

void OrbitalRifle::LevelUp()
{
	ActiveSkill::LevelUp();
	for (int i = 0; i < level; i++)
	{
		rifles[i]->SetActive(true);
	}
}

void OrbitalRifle::UpdateChange()
{
	for (RangeWeapon* rifle : rifles)
	{
		rifle->SetOwner(owner);
		pivot = owner->GetPivot();
		rifle->SetParent(pivot);


		ProjectileManager::Get()->UpdateDamage(rifle->GetProjectileName(), owner->GetFinalStatus().finalAttack * damageRatio);
	}
}

