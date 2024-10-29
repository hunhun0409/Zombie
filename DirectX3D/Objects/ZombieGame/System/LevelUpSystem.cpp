#include "Framework.h"

LevelUpSystem::LevelUpSystem()
{
	levelUpPanel = new LevelUpPanel();
}

LevelUpSystem::~LevelUpSystem()
{
	delete levelUpPanel;
}

void LevelUpSystem::Update()
{
	if (stack > 0 && isSelecting == false)
	{
		LevelUp();
		stack--;
	}

	levelUpPanel->Update();
}

void LevelUpSystem::Render()
{
	levelUpPanel->Render();
}

void LevelUpSystem::LevelUp()
{
	level++;
	if (isSelecting)
	{
		stack++;
		return;
	}
	
	Show();
}

void LevelUpSystem::Show()
{
	vector<Skill*> skills = SkillManager::Get()->GetThreeRandomSkills();
	levelUpPanel->SetSkills(skills);
	levelUpPanel->Show();

	Timer::Get()->SetDeltaScale(0.0f);
	isSelecting = true;
}

void LevelUpSystem::Hide()
{
	Timer::Get()->SetDeltaScale(1.0f);
	isSelecting = false;
}

