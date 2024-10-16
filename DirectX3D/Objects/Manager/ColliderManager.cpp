#include "Framework.h"

//ColliderManager::ColliderManager(QuadTree* quadTree)
//	:quadTree(quadTree)
//{
//}

void ColliderManager::Update()
{
	quadTree->Update();
	CheckCollisions();
}

void ColliderManager::Render()
{
	quadTree->Render();
}

void ColliderManager::Add(Collider* collider)
{
	totalColliders.push_back(collider);
	quadTree->Insert(collider);
}

void ColliderManager::Remove(Collider* collider)
{
	quadTree->Remove(collider);
}

void ColliderManager::Clear()
{
	quadTree->Clear();
	totalColliders.clear();
}

void ColliderManager::CheckCollisions()
{
	if (quadtreeCheck)
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
	else
	{
		for (Collider* collider1 : totalColliders)
		{
			if (!collider1->Active()) continue;

			for (Collider* collider2 : totalColliders)
			{
				if (!collider2->Active()) continue;

				if (collider1->IsCollision(collider2))
				{
					HandleCollision(collider1, collider2);
				}
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
