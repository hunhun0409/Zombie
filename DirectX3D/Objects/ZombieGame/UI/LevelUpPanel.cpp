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
		data.iconTextureFile = L"Textures/UI/LevelUpArmor.png";
		data.description = "description" + to_string(i);
		data.title = "title" + to_string(i);

		button->SetData(data);
		button->SetEvent(bind(&LevelUpPanel::Hide, this));
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

	Panel::Show(pos);

	for (auto btn : buttons)
	{
		//Todo : 랜덤한 선택지 가져와서 띄운다
	}
}

void LevelUpPanel::Hide()
{
	Panel::Hide();
	//Observer::Get()->ExcuteEvent("LevelUpEnd");
	Timer::Get()->SetDeltaScale(1.0f);

}
