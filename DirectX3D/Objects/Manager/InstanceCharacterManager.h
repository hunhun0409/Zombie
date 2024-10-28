#pragma once

class InstanceCharacterManager : public Singleton<InstanceCharacterManager>
{
private:
	const UINT SIZE = 36;
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

	void SetAstar(AStar* aStar);
	void SetTerrain(Terrain* terrain);
	void SetShader(wstring file);
	
	void SetHP(float amount);
	void SetDamage(float amount);

private:
	void ApplySeperation();

private:
	unordered_map<string, ModelAnimatorInstancing*>::iterator iter;
	unordered_map<string, ModelAnimatorInstancing*> totalInstancies;

	typedef vector<InstanceCharacter*> InstanceCharacters;
	unordered_map<string, InstanceCharacters> totalCharacters;

	Character* target;

	float time = 0.0f;


	AStar* aStar;
	Terrain* terrain;


	int activeZombieCount = 0;
	int renderingZombieCount = 0;
};