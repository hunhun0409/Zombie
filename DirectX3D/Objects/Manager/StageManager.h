#pragma once

class StageManager : public Singleton<StageManager> // ���� ������, ����, �ð� ����
{
private:
	friend class Singleton;
	StageManager();
	~StageManager();

public:
	void Update();
	void Render();

	void Spawn();

	void GameOver();

private:

	int level;
	int min;
	int sec;

	float defaultSpawnTime = 10.0f;
	float spawnRate;
	float spawnTime = 0.0f;

	GameOverPanel* gameOverPanel;


public:
	float gameTime = 0.0f;
	int killCount = 0;
};
