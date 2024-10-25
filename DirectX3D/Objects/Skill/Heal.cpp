#include "Framework.h"

Heal::Heal()
	:ActiveSkill("heal", "heal", L"Textures/UI/HPIcon.png", 50, 0)
{
}

void Heal::Update()
{
	ActiveSkill::Update();
}

void Heal::Render()
{
}

void Heal::GUIRender()
{
	ActiveSkill::GUIRender();
}

void Heal::Activate()
{
	ActiveSkill::Activate();
}

void Heal::Deactivate()
{
	ActiveSkill::Deactivate();
}

void Heal::LevelUp()
{
	owner->Heal(50);
}
