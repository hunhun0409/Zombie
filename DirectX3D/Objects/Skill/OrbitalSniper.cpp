#include "Framework.h"

OrbitalSniper::OrbitalSniper()
	:ActiveSkill("sniper", "sniper", L"Textures/UI/SniperIcon.png", 10, 5)
{
	
	snipers.reserve(5);
	for (int i = 0; i < 5; i++)
	{
		RangeWeapon* sniper = new RangeWeapon("sniper", 1.0f);
		sniper->GetMesh()->Pos().x = distance * cos(i * XM_2PI / 5);
		sniper->GetMesh()->Pos().z = distance * sin(i * XM_2PI / 5);
		sniper->GetMesh()->Pos().y = distance * 0.25f;
		sniper->SetProjectile("bullet", to_string(i), 0);
		snipers.push_back(sniper);
	}
}

OrbitalSniper::~OrbitalSniper()
{
	for (RangeWeapon* sniper : snipers)
	{
		delete sniper;
	}
}

void OrbitalSniper::Update()
{
	if (!owner->Active()) return;


	for (int i = 0; i < level; i++)
	{

		snipers[i]->Update();
		snipers[i]->Shoot();
	}
}

void OrbitalSniper::Render()
{
	if (!owner->Active()) return;

	for (int i = 0; i < level; i++)
	{
		snipers[i]->Render();
	}
}

void OrbitalSniper::GUIRender()
{
	for (RangeWeapon* sniper : snipers)
	{
		sniper->GUIRender();
	}
}

void OrbitalSniper::Activate()
{
	ActiveSkill::Activate();

	for (int i = 0; i < level; i++)
	{
		snipers[i]->SetActive(true);
	}
}

void OrbitalSniper::Deactivate()
{
	ActiveSkill::Deactivate();

	for (int i = 0; i < level; i++)
	{
		snipers[i]->SetActive(false);
	}
}

void OrbitalSniper::SetOwner(Character* owner)
{
	Skill::SetOwner(owner);
	//pivot->SetParent(owner);

	for (RangeWeapon* sniper : snipers)
	{
		sniper->SetOwner(owner);
		pivot = owner->GetPivot();

		ProjectileManager::Get()->UpdateDamage(sniper->GetProjectileName(), owner->GetFinalStatus().finalAttack * damageRatio);
	}
}

void OrbitalSniper::LevelUp()
{
	ActiveSkill::LevelUp();
	for (int i = 0; i < level; i++)
	{
		snipers[i]->SetActive(true);
	}
}

void OrbitalSniper::UpdateChange()
{
	for (RangeWeapon* sniper : snipers)
	{
		sniper->SetOwner(owner);
		pivot = owner->GetPivot();
		sniper->SetParent(pivot);

		ProjectileManager::Get()->UpdateDamage(sniper->GetProjectileName(), owner->GetFinalStatus().finalAttack * damageRatio);
	}
}

