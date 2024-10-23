#include "Framework.h"

LevelUpPanel::LevelUpPanel()
	: Panel(L"Textures/UI/SkillPanel.png")
{

	for (int i = 0; i < 3; i++)
	{
		Button* button = new Button(L"Textures/UI/SkillSelection.png");
		button->SetTag("SkillButton_" + to_string(i));
		button->Load();
	}
}

LevelUpPanel::~LevelUpPanel()
{
	for (pair<string, Button*> btn : buttons)
	{
		delete btn.second;
	}
}

void LevelUpPanel::Update()
{
}

void LevelUpPanel::Render()
{
}

void LevelUpPanel::Show(Vector3 pos)
{
	Panel::Show(pos);

	for (auto btn : buttons)
	{

	}
}
