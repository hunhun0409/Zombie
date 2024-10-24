#include "Framework.h"

LevelUpPanel::LevelUpPanel()
	: Panel(L"Textures/UI/SkillPanel.png")
{
	tag = "LevelUpPanel";
	Load();
	for (int i = 0; i < 3; i++)
	{
		LevelUpButton* button = new LevelUpButton();
		button->SetTag("LevelUpButton_" + to_string(i));

		LevelUpButton::UpgradeData data;
		data.iconTextureFile = L"Textures/UI/ArmorIcon.png";
		data.description = "description" + to_string(i);
		data.title = "title" + to_string(i);

		button->SetData(data);
		if (i == 0)
		{
			button->SetEvent(bind(&LevelUpPanel::Select1, this));

		}
		else if (i == 1)
		{
			button->SetEvent(bind(&LevelUpPanel::Select2, this));
		}
		else
		{
			button->SetEvent(bind(&LevelUpPanel::Select3, this));
		}
		AddButton("LevelUpButton" + to_string(i), button, Vector3());
	}

	for (auto btn : buttons)
	{
		btn.second->Load();
	}
}

void LevelUpPanel::Update()
{
	if (!Active()) return;

	Panel::Update();
}

void LevelUpPanel::Render()
{
	if (!Active()) return;

	Panel::Render();
}

void LevelUpPanel::Show(Vector3 pos)
{
	Timer::Get()->SetDeltaScale(0.0f);

	int idx = 0;
	for (auto btn : buttons)
	{
		LevelUpButton::UpgradeData data;
		Skill* skill = upgradeSkills[idx];
		data.title = skill->Info().name;
		data.iconTextureFile = skill->Info().iconPath;

		DataManager::SkillDescriptionData descriptions;
		DataManager::Get()->GetDescription(skill->Info().id, descriptions);
		data.description = descriptions.descriptions[skill->GetCurrentLevel() + 1];
		static_cast<LevelUpButton*>(btn.second)->SetData(data);
		idx++;
	}

	Panel::Show(pos);
}

void LevelUpPanel::Hide()
{
	Panel::Hide();
	Timer::Get()->SetDeltaScale(1.0f);
}

void LevelUpPanel::Select1()
{
	SkillManager::Get()->LevelUp(upgradeSkills[0]->Info().id);
	Hide();
}

void LevelUpPanel::Select2()
{
	SkillManager::Get()->LevelUp(upgradeSkills[1]->Info().id);

	Hide();
}

void LevelUpPanel::Select3()
{
	SkillManager::Get()->LevelUp(upgradeSkills[2]->Info().id);

	Hide();
}

