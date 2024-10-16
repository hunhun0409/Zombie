#pragma once

class ColliderManager : public Singleton<ColliderManager>
{
private:
	friend class Singleton;
	ColliderManager() = default;
	//ColliderManager(QuadTree* quadTree);
	~ColliderManager() = default;

public:
	void Update();
	void Render();
	void Add(Collider* collider);
	void Remove(Collider* collider);
	void Clear();

	void SetQuadTree(QuadTree* quadTree) { this->quadTree = quadTree; }

	void SetQuadTreeMode(bool value) { quadtreeCheck = value; }

private:
	void CheckCollisions();
	void HandleCollision(Collider* a, Collider* b);

private:
	QuadTree* quadTree;
	vector<Collider*> totalColliders;

	bool quadtreeCheck = true;
};
