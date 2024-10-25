#include "Framework.h"

ArmorUpgrade::ArmorUpgrade()
	:PassiveSkill("armor", "armor", L"Textures/UI/ArmorIcon.png", 0, 10)
{
}

void ArmorUpgrade::Activate()
{
	PassiveSkill::Activate();
}

void ArmorUpgrade::Deactivate()
{
	PassiveSkill::Deactivate();
}

void ArmorUpgrade::LevelUp()
{
	PassiveSkill::LevelUp();

	effectValue = baseEffectValue + effectPerLevel * level;
	Character::StatusUpgradeInfo& status = owner->GetUpgradeStatus();
	status.upgradeArmor = effectValue;
	owner->UpdateStatus();
}

void ArmorUpgrade::Update()
{
	PassiveSkill::Update();
}

void ArmorUpgrade::Render()
{
	PassiveSkill::Render();
}
