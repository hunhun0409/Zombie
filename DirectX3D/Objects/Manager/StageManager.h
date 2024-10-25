#pragma once

class StageManager : public Singleton<StageManager> // ���� ������, ����, �ð� ����
{
private:
	friend class Singleton;
	StageManager() = default;
	~StageManager() = default;

public:
	void Update();
	void Render();

	void Spawn();

	

private:
	float gameTime = 0.0f;

	int level;
	int min;
	int sec;

	float defaultSpawnTime = 10.0f;
	float spawnRate;
	float spawnTime = 0.0f;

public:
	int killCount = 0;
};
