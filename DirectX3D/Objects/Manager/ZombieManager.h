#pragma once

class ZombieManager : public Singleton<ZombieManager>
{
private:
	const UINT SIZE = 30;
	const float SPAWN_TIME = 2.0f;
	const float SPAWN_RANGE = 100.0f;
public:
	ZombieManager();
	~ZombieManager();

	void Update();
	void Render();
	void PostRender();

	void SetTarget(Transform* target);


private:
	void Spawn();

private:
	ModelAnimatorInstancing* zombieInstancing;
	vector<Zombie*> zombies;

	Transform* target;

	float time = 0.0f;
};

