#pragma once

class ColliderManager : public Singleton<ColliderManager>
{
private:
	const float CHECK_INTERVAL = 0.05f;
private:
	friend class Singleton;
	ColliderManager() = default;
	//ColliderManager(QuadTree* quadTree);
	~ColliderManager() = default;

public:
	void Update();
	void Render();
	void GUIRender();
	void Add(Collider* collider);
	void Remove(Collider* collider);

	void UpdateColliderState(Collider* collider);

	void Clear();

	void SetQuadTree(QuadTree* quadTree) { this->quadTree = quadTree; }

	void SetQuadTreeMode(bool value) { quadtreeCheck = value; }



private:
	void CheckCollisions();
	void HandleCollision(Collider* a, Collider* b);

private:
	QuadTree* quadTree = nullptr;
	unordered_map<Collider*, bool> totalColliders;
	bool quadtreeCheck = true;

	float checkInterval = 0.0f;

	int collisionCheckCount = 0;
};
