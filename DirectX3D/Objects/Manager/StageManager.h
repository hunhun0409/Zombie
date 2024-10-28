#pragma once

class StageManager : public Singleton<StageManager> // 좀비 스폰수, 스탯, 시간 관리
{
private:
	const float LEVEL_UP_TIME = 10.0f;

	const float INITIAL_DAMAGE = 10.0f;
	const float INITIAL_HP = 100.0f;
private:
	friend class Singleton;
	StageManager();
	~StageManager();

public:
	void Update();
	void Render();
	void GUIRender();

	void Spawn();

	void GameOver();
	void LevelUp();

private:

	int level = 1;
	int min;
	int sec;

	float defaultSpawnTime = 10.0f;
	float spawnRate;
	float spawnTime = 0.0f;

	GameOverPanel* gameOverPanel;


public:
	float gameTime = 0.0f;
	float levelUpTime = 0.0f;
	int killCount = 0;

	float zombieHp = INITIAL_HP;
	float zombieDamage = INITIAL_DAMAGE;
};
