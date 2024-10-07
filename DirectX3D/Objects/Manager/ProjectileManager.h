#pragma once

class ProjectileManager : public Singleton<ProjectileManager>
{
private:
	const UINT SIZE = 100;
private:
	friend class Singleton;
	ProjectileManager() = default;
	~ProjectileManager();

public:
	void Update();
	void Render();
	void GUIRender();

	bool Shoot(string name, Vector3 startPos, Vector3 dir, float speed, float maxLifeTime = 10.0f);
	
	void Add(string key, string name);
	void Remove(string key);
	void Clear();

	void SetScale(string key, Vector3 scale);
	void SetColliderScale(string key, Vector3 scale);

	bool IsCollision(Collider* collider);

private:
	void Save(string key);
	void Load(string key);

private:
	unordered_map<string, ModelInstancing*>::iterator iter;
	unordered_map<string, ModelInstancing*> totalInstancies;

	typedef vector<Projectile*> Projectiles;
	unordered_map<string, Projectiles> totalProjectiles;

	unordered_map<string, Transform*> totalModelTransforms;
	unordered_map<string, Transform*> totalColliderTransforms;

	bool isChanged = false;
};
