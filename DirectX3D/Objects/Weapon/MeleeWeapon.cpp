#include "Framework.h"

MeleeWeapon::MeleeWeapon(string name)
	:Weapon(name)
{
	collider = new CapsuleCollider();
	collider->SetTag(name + "_collider");
	collider->Update();
	collider->SetActive(false);
	collider->SetOwner(this);

	start = new SphereCollider();
	start->SetTag(name + "_start");
	end = new SphereCollider();
	end->SetTag(name + "_end");

	collider->SetParent(mesh);
	start->SetParent(mesh);
	end->SetParent(mesh);

	trail = new Trail(L"textures/effect/trail.png", start, end, 20, 50.0f);
	trail->GetMaterial()->GetData().diffuse = { 0.3f, 0.3f, 0.3f, 1.0f };
	trail->SetActive(false);

	Load();

	ColliderManager::Get()->Add(collider);
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

	/*if (collider->Active())
	{
		InstanceCharacterManager::Get()->Collision(collider, damage, overlappedColliders);
	}*/
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
	trail->Reset();
}

void MeleeWeapon::DisableAttack()
{
	collider->SetActive(false);
	trail->SetActive(false);
	overlappedColliders.clear();
}

void MeleeWeapon::Load()
{
	mesh->Load();
	collider->Load();
	start->Load();
	end->Load();
}

void MeleeWeapon::OnCollision(Collider* other)
{
	string collideName = other->GetTag();

	if (collideName.find("Zombie") != string::npos)
	{
		InstanceZombie* zombie = dynamic_cast<InstanceZombie*>(other->Owner());
		if (zombie == nullptr) return;

		zombie->TakeDamage(50);
		overlappedColliders.insert(other);
	}
}
