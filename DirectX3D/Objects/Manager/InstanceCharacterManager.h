#pragma once

class InstanceCharacterManager : public Singleton<InstanceCharacterManager>
{
private:
	const UINT SIZE = 20;
	const float SPAWN_RANGE = 100.0f;
private:
	friend class Singleton;
	InstanceCharacterManager() = default;
	~InstanceCharacterManager();

public:
	void Update();
	void Render();
	void PostRender();
	void GUIRender();

	void SetTarget(Character* target);

	void Add(string key, string name);

	void Remove(string key);
	void Clear();

	void Spawn(string key, UINT spawnAmount = 1);
	void Spawn(string key, Vector3 pos, UINT spawnAmount = 1);

	void Collision(Collider* collider, float damage);
	//void Collision(Collider* collider, float damage, set<Collider*>& overlappedCollider);

	void SetAstar(AStar* aStar);
	void SetTerrain(Terrain* terrain);

	
private:
	void ApplySeperation();

private:
	unordered_map<string, ModelAnimatorInstancing*>::iterator iter;
	unordered_map<string, ModelAnimatorInstancing*> totalInstancies;

	typedef vector<InstanceCharacter*> InstanceCharacters;
	unordered_map<string, InstanceCharacters> totalCharacters;

	Character* target;

	float time = 0.0f;

	bool isChanged = false;

	AStar* aStar;
	Terrain* terrain;
};