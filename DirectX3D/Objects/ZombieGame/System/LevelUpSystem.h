#pragma once

class LevelUpSystem : public Singleton<LevelUpSystem>
{
private:
	friend class Singleton;
	LevelUpSystem();
	~LevelUpSystem();

public:
	void Update();
	void Render();
	
	void LevelUp();

	void Show();
	void Hide();


public:
	int level = 1;
private:
	//bool isActive = false;
	LevelUpPanel* levelUpPanel;
	
};
