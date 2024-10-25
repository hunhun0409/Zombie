#include "Framework.h"

HpUpgrade::HpUpgrade()
	:PassiveSkill("hp", "hp", L"Textures/UI/HPIcon.png", 0, 10)
{
}

void HpUpgrade::Activate()
{
	PassiveSkill::Activate();
}

void HpUpgrade::Deactivate()
{
	PassiveSkill::Deactivate();
}

void HpUpgrade::LevelUp()
{
	PassiveSkill::LevelUp();

	effectValue = baseEffectValue + effectPerLevel * level;
	Character::StatusUpgradeInfo& status = owner->GetUpgradeStatus();
	status.upgradeHp = effectValue;
	owner->UpdateStatus();
}

void HpUpgrade::Update()
{
	PassiveSkill::Update();
}

void HpUpgrade::Render()
{
	PassiveSkill::Render();
}
