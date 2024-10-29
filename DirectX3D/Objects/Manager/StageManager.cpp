#include "Framework.h"


StageManager::StageManager()
{
	gameOverPanel = new GameOverPanel();
	Audio::Get()->Add("ZombieSound", "Sounds/Zombie/zombieSound.wav");
}

StageManager::~StageManager()
{
	delete gameOverPanel;
}

void StageManager::Update()
{
	gameTime += DELTA;
	levelUpTime += DELTA;

	if (levelUpTime >= LEVEL_UP_TIME)
	{
		LevelUp();
		levelUpTime -= LEVEL_UP_TIME;
	}
	

	min = gameTime / 60;
	sec = (int)gameTime % 60;

	spawnRate = defaultSpawnTime / level;

	spawnTime -= DELTA;
	if (spawnTime <= 0)
	{
		spawnTime += spawnRate;
		Spawn();
	}

	gameOverPanel->Update();
}

void StageManager::Render()
{
	Float2 boxSize;
	boxSize.x = 50;
	boxSize.y = 50;

	Float2 pos;
	pos.x = CENTER_X;
	pos.y = WIN_HEIGHT - 70;

	string sMin;
	if (min < 10)
	{
		sMin = "0" + to_string(min);
	}
	else
	{
		sMin = to_string(min);
	}
	string sColon = " : ";

	string sSec;
	if (sec < 10)
	{
		sSec = "0" + to_string(sec);

	}
	else
	{
		sSec = to_string(sec);
	}

	Font::Get()->SetStyle("ButtonFontDescription");
	Font::Get()->SetColor("White");
	Font::Get()->RenderText(sMin, Float2(pos.x - 20, pos.y), boxSize);
	Font::Get()->RenderText(sColon, pos, boxSize);
	Font::Get()->RenderText(sSec, Float2(pos.x + 20, pos.y), boxSize);
	pos.x = WIN_WIDTH - 200;

	string sLevel = "Level : " + to_string(level);
	boxSize.x = 200;
	Font::Get()->RenderText(sLevel, pos, boxSize);

	string sKill = "Kill Count : " + to_string(killCount);
	Font::Get()->RenderText(sKill, Float2(pos.x - 200, pos.y), boxSize);

	gameOverPanel->Render();
}

void StageManager::GUIRender()
{
	ImGui::Text("Zombie Hp : %.1f", zombieHp);
	ImGui::Text("Zombie Damage : %.1f", zombieDamage);

}

void StageManager::Spawn()
{
	InstanceCharacterManager::Get()->Spawn("ZombieWoman", 1 + level);
	InstanceCharacterManager::Get()->Spawn("ZombieMutant", 1 + level);

	Audio::Get()->Play("ZombieSound", 1.0f);
}

void StageManager::GameOver()
{
	gameOverPanel->Show();
}

void StageManager::LevelUp()
{
	level++;

	zombieDamage = INITIAL_DAMAGE * (1.0f + level * 0.2f);
	InstanceCharacterManager::Get()->SetDamage(zombieDamage);

	zombieHp = INITIAL_HP * (1.0f + level * 0.1f);
	InstanceCharacterManager::Get()->SetHP(zombieHp);
}
