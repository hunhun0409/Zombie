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
	levelUpPanel->Update();
}

void LevelUpSystem::Render()
{
	levelUpPanel->Render();
}

void LevelUpSystem::LevelUp()
{
	level++;
	Show();
}

void LevelUpSystem::Show()
{
	vector<Skill*> skills = SkillManager::Get()->GetThreeRandomSkills();
	levelUpPanel->SetSkills(skills);
	levelUpPanel->Show();
}

void LevelUpSystem::Hide()
{
	levelUpPanel->Hide();
}

