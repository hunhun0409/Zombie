#include "Framework.h"

ZombieManager::ZombieManager()
{
	zombieInstancing = new ModelAnimatorInstancing("ZombieWoman");
	zombieInstancing->ReadClip("Idle", true);
	zombieInstancing->ReadClip("Walk", true);
	zombieInstancing->ReadClip("Run", true);
	zombieInstancing->ReadClip("Scream");
	zombieInstancing->ReadClip("Attack");
	zombieInstancing->ReadClip("Hit");
	zombieInstancing->ReadClip("Dying");

	zombies.reserve(SIZE);
	FOR(SIZE)
	{
		Transform* transform = zombieInstancing->Add();
		transform->Scale() *= 0.02f;
		transform->SetActive(false);
		Zombie* zombie = new Zombie(transform, zombieInstancing, i);
		zombies.push_back(zombie);
	}
}

ZombieManager::~ZombieManager()
{
	delete zombieInstancing;

	for (Zombie* zombie : zombies)
	{
		delete zombie;
	}
}

void ZombieManager::Update()
{

	time += DELTA;

	if (time > SPAWN_TIME)
	{
		time -= SPAWN_TIME;

		Spawn();
	}

	for (Zombie* zombie : zombies)
	{
		zombie->Update();
	}

	zombieInstancing->Update();
}

void ZombieManager::Render()
{
	for (Zombie* zombie : zombies)
	{
		zombie->Render();
	}

	zombieInstancing->Render();
}

void ZombieManager::PostRender()
{
	for (Zombie* zombie : zombies)
	{
		zombie->PostRender();
	}
}

void ZombieManager::SetTarget(Transform* target)
{
	this->target = target;

	for (Zombie* zombie : zombies)
	{
		zombie->SetTarget(target);
	}
}


void ZombieManager::Spawn()
{
	Vector3 dir;
	dir.x = Random(-1.0f, 1.0f);
	dir.z = Random(-1.0f, 1.0f);

	Vector3 randomPos = target->Pos() + dir.GetNormalized() * SPAWN_RANGE;
	randomPos.y = 0.0f;
	for (Zombie* zombie : zombies)
	{
		if (zombie->GetTransform()->Active() == false)
		{
			zombie->Spawn(randomPos);
			break;
		}
	}
}
