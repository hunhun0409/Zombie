#include "Framework.h"

MeleeWeapon::MeleeWeapon(string name)
	:Weapon(name)
{
	collider = new CapsuleCollider();
	collider->SetTag(name + "_collider");
	collider->Load();
	collider->Update();
	collider->SetActive(true);

	start = new SphereCollider();
	start->SetTag(name + "_start");
	start->Load();
	end = new SphereCollider();
	end->SetTag(name + "_end");
	end->Load();

	collider->SetParent(mesh);
	start->SetParent(mesh);
	end->SetParent(mesh);

	trail = new Trail(L"textures/effect/trail.png", start, end, 20, 50.0f);
	trail->GetMaterial()->GetData().diffuse = { 0.3f, 0.3f, 0.3f, 1.0f };
	trail->SetActive(false);
}

MeleeWeapon::~MeleeWeapon()
{
	delete collider;
	delete start;
	delete end;
	delete trail;
}

void MeleeWeapon::Update()
{
	Weapon::Update();

	collider->Update();
	start->UpdateWorld();
	end->UpdateWorld();
	trail->Update();

	if (collider->Active())
	{
		InstanceCharacterManager::Get()->Collision(collider);
	}
}

void MeleeWeapon::Render()
{
	Weapon::Render();

	collider->Render();
	start->Render();
	end->Render();
	trail->Render();
}

void MeleeWeapon::GUIRender()
{
	Weapon::GUIRender();

	collider->GUIRender();
	start->GUIRender();
	end->GUIRender();
}

void MeleeWeapon::EnableAttack()
{
	collider->SetActive(true);
	trail->SetActive(true);
}

void MeleeWeapon::DisableAttack()
{
	collider->SetActive(false);
	trail->SetActive(false);
}

void MeleeWeapon::Load()
{
	mesh->Load();
	collider->Load();
	start->Load();
	end->Load();
}

