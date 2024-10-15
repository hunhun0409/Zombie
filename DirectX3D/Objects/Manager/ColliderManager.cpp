#include "Framework.h"

ColliderManager::ColliderManager(QuadTree* quadTree)
	:quadTree(quadTree)
{
}

void ColliderManager::Update()
{
	quadTree->Update();
	CheckCollisions();
}

void ColliderManager::Add(Collider* collider)
{
	quadTree->Insert(collider);
}

void ColliderManager::Remove(Collider* collider)
{
	quadTree->Remove(collider);
}

void ColliderManager::CheckCollisions()
{
	for (Collider* collider : totalColliders)
	{
		if (!collider->Active()) continue;

		vector<Collider*> potentialColliders = quadTree->GetPotentialColliders(collider);

		for (Collider* other : potentialColliders)
		{
			if (!other->Active() || other == collider) continue;

			if (collider->IsCollision(other))
			{
				HandleCollision(collider, other);
			}
		}
	}
}

void ColliderManager::HandleCollision(Collider* a, Collider* b)
{
	// Collision response logic
	a->OnCollision(b);
	b->OnCollision(a);

	// You might want to notify game objects or trigger events here
	// For example:
	// a->GetGameObject()->OnCollision(b->GetGameObject());
	// b->GetGameObject()->OnCollision(a->GetGameObject());
}
