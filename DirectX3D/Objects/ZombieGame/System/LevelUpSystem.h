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


private:
	//bool isActive = false;
	LevelUpPanel* levelUpPanel;
	int level = 1;
};
